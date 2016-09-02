/*
	A somewhat friendly interface for image processing.
	by Jackie Hunter,05/2000
*/
#ifdef _WIN32
#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
#include <list>
#include <ctype.h>
#include <utils.h>

#include <shell.h>
#include <object.h>

#pragma	comment(linker,	"/base:0x72500000")

#define	FS			  " \t\n"
#define	COMMENT		  ("#")
#define	COMMENT_BEGIN ("/" "*")
#define	COMMENT_END	  ("*" "/")

#define	LBUFFER_SIZE 1024
#define	MAX_ARGUMENTS 10

static int escape(HRTK_SHELL s,	char * cmd);

typedef std::list<variable *> var_list;


#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "librtk.lib")

struct shell : public CRTKObject
{
public:
	shell_command *	cmd;
	var_list variables;
	bool bComment;
	
	shell();
	virtual ~shell();
};

shell::shell()
{
	cmd = 0;
}

shell::~shell()
{
	cmd = 0;
	// remove variables
	var_list::iterator it;
	it = variables.begin();
	while(it != variables.end()){
		delete *it;
		it++;
	}
	variables.clear();
}

void variable::show()
{
	variable * v=this;
	switch(v->type){
	case variable::vt_int:
		utils_trace("\t%s(int)=%d\n",v->name,v->i_val);
		break;
	case variable::vt_char_:
		utils_trace("\t%s(char*)=%s\n",v->name,v->c_val);
		break;
	case variable::vt_float:
		utils_trace("\t%s(float)=%f\n",v->name,v->f_val);
		break;
	case variable::vt_object:
		utils_trace("\t%s(object)=0x%08x\n",v->name,v->o_val);
		break;
	default:
		assert(0);
	}
}

void variable::get(char * buffer, int /*length*/)
{
	variable * v=this;
	switch(v->type){
	case variable::vt_int:
		sprintf(buffer,"%d",v->i_val);
		break;
	case variable::vt_char_:
		sprintf(buffer,"%s",v->c_val);
		break;
	case variable::vt_float:
		sprintf(buffer,"%f",v->name,v->f_val);
		break;
	case variable::vt_object:
		sprintf(buffer,"0x%08x",v->name,v->o_val);
		break;
	default:
		assert(0);
	}
}

int variable::set(const	char * value)
{
	variable * v=this;
	switch(v->type){
	case variable::vt_int:
		v->i_val=atoi(value);
		break;
	case variable::vt_char_:
		strncpy(v->c_val,value,sizeof(v->c_val));
		v->c_val[sizeof(v->c_val) -	1] = 0;
		break;
	case variable::vt_float:
		v->f_val=(float)atof(value);
		break;
	case variable::vt_object:
		if(!strcmp(value,"nothing") ){
			//delete o_val;
			//o_val=NULL;
			shell_del_v(s, this);
		}else{
			utils_error("variable of object type cannot be directly set.\n");
		}
		break;
	default:
		assert(0);
	}
	return 1;
}

RTK_SHELL_API void RTK_API shell_add_v(HRTK_SHELL sh, variable * v)
{
	shell *	s;
	s =	(shell *)sh;
	s->variables.insert(s->variables.end(),v);
}

RTK_SHELL_API void RTK_API shell_del_v(HRTK_SHELL sh, variable * v)
{
	shell *	s =	(shell *)sh;
	var_list::iterator p=std::find(s->variables.begin(),
		s->variables.end(),v);
	if(p !=	s->variables.end() ){
		if(v->type == variable::vt_object){
			// bug?	memory leak	possible
			// delete v->o_val;
		}
		s->variables.erase(p);
	}
}

RTK_SHELL_API variable * RTK_API shell_get_v(
	HRTK_SHELL sh,	
	const char * name
	)
{
	shell *	s =	(shell *)sh;
	variable * v=NULL;
	var_list::iterator p=s->variables.begin();
	int	i=s->variables.size();
	for(;i>0;i--){
		v=*p;
		if(!strcmp(v->name,name)) break;
		p++;
	}
	if(i>0)	return v;
	return NULL;
}

RTK_SHELL_API int RTK_API shell_get_v(HRTK_SHELL sh, const char * name, int * value)
{
	variable * v= shell_get_v(sh, name);
	if(!v) return 0;
	*value=v->i_val;
	return 1;
}

RTK_SHELL_API int RTK_API shell_get_v(HRTK_SHELL sh, const char * name,char	** value)
{
	variable * v = shell_get_v(sh, name);
	if(!v) return 0;
	*value=v->c_val;
	return 1;
}

RTK_SHELL_API int RTK_API shell_get_v(HRTK_SHELL sh, const char * name,float * value)
{
	variable * v = shell_get_v(sh, name);
	if(!v) return 0;
	*value=(float)v->f_val;
	return 1;
}

RTK_SHELL_API int RTK_API shell_get_v(HRTK_SHELL s,	const char * name,void ** value)
{
	variable * v=shell_get_v(s,	name);
	if(!v) return 0;
	*value=v->o_val;
	return 1;
}

//vlaid_cmds为数组，元素形如{debug,debug,on_debug}
RTK_SHELL_API HRTK_SHELL RTK_API shell_create(
	const shell_command	* valid_cmds
	)
{
	shell *	s;
//	HRTK_SHELL hs;
	

	s =	new	shell;
	if(!s){
		return 0;
	}
	s->cmd = (shell_command*)valid_cmds;
	int	i;
	
	shell_add_v(s, new variable(1,"result"));

	//for(i=0;i<MAX_ARGUMENTS;i++){
	//	char name[3];
	//	sprintf(name,"C%d",i+1);
	//	shell_add_v(s, new variable("",name));
	//}

	//for(i=0;i<MAX_ARGUMENTS;i++){
	//	char name[3];
	//	sprintf(name,"U%d",i+1);
	//	shell_add_v(s, new variable("",name));
	//}

	/*
	char * fname="init.piv";
	batch_execute(fname);
	*/

	return s;
}

static __bool RTK_API execCmd(
	shell * s,
	const char * cmd,
	void * pv,
	int	* code
	)
{
	char * verb;
	char lbuffer[LBUFFER_SIZE];
	CArgs args;

	verb = "";
	//ensure the buffer is large enough
	strncpy(lbuffer,cmd, sizeof(lbuffer));
	lbuffer[sizeof(lbuffer) - 1] = 0;
	//trim the trailing	'\n'
	if(lbuffer[strlen(lbuffer)-1] == '\n'){
		lbuffer[strlen(lbuffer)-1]=0;
	}
	if( !escape(s, lbuffer) ){
		return -1;
	}
	parse_arg_ex(lbuffer, &args, FS);
	verb = args.argv[0];
	if(!args.argc){
		return -1;
	}
	shell_command * c=s->cmd;
	while(c->name){
		if(!strcmp(verb, c->name)){
			*code =	c->shell_proc(lbuffer, pv);
			shell_get_v(s, "result")->i_val=*code;
			break;
		}
		c++;
	}
	if(!c->name){
		utils_error("unknown command:%s\n",	verb);
		return __false;
	}
	return __true;
}

RTK_SHELL_API __bool RTK_API shell_execute_ex(
	HRTK_SHELL sh, 
	const char * cmd,
	void * pv,
	int	* code
	)
{
	CArgs args;
	__bool ret;
	int i;
	
	parse_arg_ex(cmd,&args,";");
	for(i=0;i<args.argc;i++){
		ret=execCmd((shell *)sh,args.argv[i],pv,code);
	}

	return ret;
}

RTK_SHELL_API int RTK_API shell_execute(
	HRTK_SHELL sh,	
	const char * cmd,
	void * pv
	)
{
	int	code;
	return shell_execute_ex(sh, cmd, pv, &code)? code :	0;
}

static int escape(HRTK_SHELL s, char * cmd)
{
	char swap[LBUFFER_SIZE];
	int	i=0,j=0;
	variable * v;

	memset(swap,0,sizeof(swap));
	strcpy(swap,cmd);
	memset(cmd,0,strlen(cmd));
	while(swap[i]){
		if(swap[i]=='$' ){
			//search for '(' and ')'
			if(swap[i+1] != '('){
				utils_error("unknown escape sequence.\n");
				return 0;
			}
			char *p=strchr(swap+i+2,')');
			if(!p){
				utils_error("')' not paired with '('\n");
				return 0;
			}
			*p=0;
			v=shell_get_v(s, swap+i+2);
			if(!v){
				utils_error("no	such variable %s\n",swap+i+2);
				return 0;
			}
			v->get(cmd+j,LBUFFER_SIZE-i);
			i=p-swap+1;
			j=strlen(cmd);
		}else{
			cmd[j]=swap[i];
			i++;
			j++;
		}		 
	}
	cmd[j]=0;	 
	return 1;
}

RTK_SHELL_API int RTK_API shell_set_v(
	HRTK_SHELL sh, 
	const char	* name,	
	const char * value,	
	variable::v_type vt
	)
{
	shell *s = (shell *)sh;
	variable * v = shell_get_v(s, name);
	int	res;
	if(!v)	{
		switch(vt){
		case variable::vt_int:
			v=new variable((int)0,name);
			break;
		case variable::vt_char_:
			v=new variable(value,name);
			break;
		case variable::vt_float:
			v=new variable((double)0.0,name);
			break;
		case variable::vt_object:
			v=new variable((void *)NULL,name);
			break;
		default:
			assert(0);			
		}
		if(!v){
			return -1;
		}
		v->set(value);
		shell_add_v(s, v);
		res=0;
	}else{
		v->set(value);
		res=1;
	}
	return res;
}

RTK_SHELL_API void RTK_API shell_destroy(HRTK_SHELL sh)
{
	delete (shell*)sh;
}

