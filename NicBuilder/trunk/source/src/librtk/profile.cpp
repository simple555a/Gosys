/*
  profile support for Non-Win32 platforms
  the procs here also removes the 64kB barrier of INI file under Win9X
  by Jackie, jhunter@tsinghua.org.cn
  History:
  03/20/2001, Jackie, first workable version
*/
#include "precomp.h"
#pragma hdrstop

#ifndef _WIN32
#include <profile.h>

#ifdef HAVE_UTILS_H
#include "utils.h"
#else
#define utils_error
#endif

#ifdef _MSC_VER
#pragma warning (disable : 4786)
#endif

#ifdef __BORLANDC__
#define __USING_STD_NAMES__
#endif

#include <string>
#include <map>
#include <fstream>
#include <iomanip>

using namespace std;

// static const char * db_file="pivdb.dat";

#ifdef __GNUC__
#define strcmpi strcasecmp
#endif

class no_case_string_cmp
{
public:
	bool operator ()(const string &a, const string &b) const
		{
			return strcmpi(a.data(), b.data())<0? true : false;			   
		}
	 
};

typedef map<string,string,no_case_string_cmp> CStringMap;

class CProfile;
class CSection;
class CInternalSection;

class CSection{
public:
	CSection(const char * name=0,CProfile * prof=0);
	// get the index-th entry in this section, and return its
	// value and key
	const char * get_entry(int index,char ** key_name);
	int clear();
	int size();
	bool ok;
protected:
	void * handle;
	CProfile * profile;
};

class LIBLIBRTK_API CProfile{
public :
	CProfile(const char * db_file=0);
	virtual ~CProfile();
public:
	friend class CSection;
	int get_string(
		const char * lpAppName,		 // points to section name
		const char * lpKeyName,		 // points to key name
		const char * lpDefault,		 // points to default string
		char * lpReturnedString, // points to destination buffer
		int nSize				  // size of destination buffer
		);
	int write_string(
		const char * lpAppName,  // pointer to section name
		const char * lpKeyName,  // pointer to key name
		const char * lpString	  // pointer to string to add
		);
	int read_key(
		const char * lpAppName,		 // points to section name
		const char * lpKeyName,		 // points to key name
		char * lpReturnedString, // points to destination buffer
		int nSize				   // size of destination buffer
		);
	int write_key(
		const char * lpAppName,  // pointer to section name
		const char * lpKeyName,  // pointer to key name
		const char * lpString	   // pointer to string to add
		);
	int read_int_key(
		const char * lpAppName,		 // points to section name
		const char * lpKeyName,		 // points to key name
		int * value
		);
	int write_int_key(
		const char * lpAppName,  // pointer to section name
		const char * lpKeyName,  // pointer to key name
		int	 value
		);
	int read_dbl_key(
		const char * lpAppName,		 // points to section name
		const char * lpKeyName,		 // points to key name
		double * value
		);
	int write_dbl_key(
		const char * lpAppName,  // pointer to section name
		const char * lpKeyName,  // pointer to key name
		double value
		);
	int load(const char * db_file="./pivdb.dat");
	int flush(const char * db_file="./pivdb.dat");
	int unload();
	int size();
	CSection * get_section(const char * name);
	int drop_section(const char * name);
	int clear_section(const char * name);
protected:
	void * internal_db;
};

class CInternalSection
{
public:
	string * get_entry(string &);
	int get_entry(int idx,string & key,string & value);
	int get_first_entry(string & key,string & value);
	int get_next_entry(string & key,string & value);
	int add_entry(string &,string &);
	int remove_entry(string &);
	int load(const ifstream & is);
	int save(const ofstream & os);
	int clear();
	int size();
protected:
	CStringMap m_map;
};

typedef map<string,CInternalSection *, no_case_string_cmp> CInternalSectionMap;
typedef CInternalSectionMap CInternalDB;

string * CInternalSection::get_entry(string & key)
{
	CStringMap::iterator p;
	p=m_map.find(key);
	if(p != m_map.end() ){
		return & p->second;
	}
	return 0;
}

int CInternalSection::size()
{
	return m_map.size();
}

int CInternalSection::get_entry(int idx,string & key,string & value)
{
	CStringMap::iterator p;
	p=m_map.begin();
	advance(p,idx);
	if(p != m_map.end()){
		key=p->first;
		value=p->second;
		return 1;
	}
	return 0;
}

int CInternalSection::clear()
{
	m_map.clear();
	return 1;
};

int get_first_entry(string & key,string & value)
{
	return 0;
}

int get_next_entry(string & key,string & value)
{
	return 0;
}

int CInternalSection::add_entry(string & key,string & value)
{
	pair<CStringMap::iterator,bool> ib;
	ib = m_map.insert(CStringMap::value_type((string)key,value));
	if(!ib.second){
		return false;
	}
	return true;
}

int CInternalSection::remove_entry(string & key)
{
	int retval=false;
	CStringMap::iterator p;
	p=m_map.find(key);
	if(p == m_map.end()){
		return false;
	}
	m_map.erase(p);
	return retval;
};

int CInternalSection::load(const ifstream & is)
{
	return 1;
}

int CInternalSection::save(const ofstream & os)
{
	return 1;
};

CSection::CSection(const char * name, CProfile * prof)
{
	this->handle=NULL;

	this->profile=prof;
	if(!prof) return;

	string s_key,v_key,value;
	CInternalDB::iterator p;
	if(!profile->internal_db) return;
	CInternalDB & the_db=*((CInternalDB *)profile->internal_db);


	handle=0;
	ok=false;

	s_key=name;

	p=the_db.find(s_key);
	if(p == the_db.end() ){
		return;
	}
	handle=p->second;
	ok=true;
}

const char * CSection::get_entry(int index,char ** key_name)
{
	char * value=0;
	if(!ok) return 0;
	if(!handle) return 0;
	CStringMap * section=(CStringMap *)handle;
	CStringMap::iterator p=section->begin();
	if(index<0 || index>=(int)section->size()){
		return 0;
	}
	advance(p,index);
	*key_name=(char*)p->first.data();
	value=(char*)p->second.data();
	return value;
}

int CSection::clear()
{
	if(!ok) return false;
	if(!handle) return false;
	CStringMap * section=(CStringMap *)handle;
	section->clear();
	return 1;
};

int CSection::size()
{
	int res=0;
	if(!ok) return false;
	if(!handle) return false;
	CStringMap * section=(CStringMap *)handle;
	res=section->size();
	return res;
}

int CProfile::read_key(
	const char * lpAppName,		 // points to section name
	const char * lpKeyName,		 // points to key name
	char * lpReturnedString, // points to destination buffer
	int nSize				   // size of destination buffer
	)
{
	return get_string(lpAppName,lpKeyName,"",lpReturnedString,nSize);
}

int CProfile::write_key(
	const char * lpAppName,  // pointer to section name
	const char * lpKeyName,  // pointer to key name
	const char * lpString	   // pointer to string to add
	)
{
	return write_string(lpAppName,lpKeyName,lpString);
}

int CProfile::read_int_key(
	const char * lpAppName,		 // points to section name
	const char * lpKeyName,		 // points to key name
	int * value
	){
	char buf[32];
	if( !read_key(lpAppName,lpKeyName,buf,sizeof(buf)) ){
		return 0;
	}
	*value=atoi(buf);
	return 1;
}

int CProfile::write_int_key(
	const char * lpAppName,  // pointer to section name
	const char * lpKeyName,  // pointer to key name
	int	 value
	)
{
	char buf[32];
	sprintf(buf,"%d",value);
	if( !write_key(lpAppName,lpKeyName,buf) ){
		return 0;
	}
	return 1;
}

int CProfile::read_dbl_key(
	const char * lpAppName,		 // points to section name
	const char * lpKeyName,		 // points to key name
	double * value
	)
{
	char buf[32];
	if( !read_key(lpAppName,lpKeyName,buf,sizeof(buf)) ){
		return 0;
	}
	*value=atof(buf);
	return 1;
}

int CProfile::write_dbl_key(
	const char * lpAppName,  // pointer to section name
	const char * lpKeyName,  // pointer to key name
	double value
	)
{
	char buf[32];
	sprintf(buf,"%f",value);
	if( !write_key(lpAppName,lpKeyName,buf) ){
		return 0;
	}
	return 1;
}

int CProfile::load(const char * db_file)
{
	unsigned int iLine,idx;
	char buf[1024];
	string strLine,curr_section,key,val;
	CInternalSection * curr_sec;
	ifstream is;

	if(!internal_db) return 0;
	CInternalDB & the_db=*((CInternalDB*)internal_db);
	
	// !!!!!!!!!!!!!!!!!!!!!!! big error special error !!!!!!!!!!!!!!!!!
	// unload_db();
	unload();
	is.open(db_file);
	if(!is.good()){
		is.close();
		// utils_error("DB file not loaded.\n");
		return 0;
	}
	iLine=0;
	curr_section="";
	curr_sec=0;
	while(!is.eof()){
		iLine++;
		is.getline(buf,sizeof(buf));
		// skip blanks and Tabs
		strLine=buf;
		idx=strLine.find_first_not_of(" \t\n");
		if(idx == string::npos) {
			continue;
		}else if(idx != 0){
			strLine=strLine.substr(idx);
		};
		if(strLine[0]==';'){
			// comment line ,skipped
			continue;
		};
		if(strLine[0]=='['){
			// new section begins
			if(curr_section != ""){
				// end current section
				curr_section="";
			}
			// find new section name
			idx=strLine.find_first_of(']');
			if(idx == string::npos){
				utils_error("Error in %s,line %d,invalid section definition.\n",db_file,iLine);
				continue;
			}
			curr_section=strLine.substr(1,idx-1);
			if(curr_section == ""){
				utils_error("Error in %s,line %d,invalid section definition.\n",db_file,iLine);
				continue;
			}
			// piv_debug("New section begins : '%s'\n",curr_section.substr(1).data());
			curr_sec=new CInternalSection();
			if(!curr_sec){
				utils_error("Low memory");
				return 0;
			}
			pair<CInternalDB::iterator,bool> ib=the_db.insert(CInternalDB::value_type(curr_section,curr_sec));
			if(!ib.second){
				delete curr_sec;
				curr_sec=0;
				utils_error("Error in %s,line %d,duplicated section name or low memory.\n",db_file,iLine);
			}
		}else{
			// piv_debug("\tEntry : %s\n",strLine.data());
			idx=strLine.find_first_of('=');
			if(idx == string::npos){
				utils_error("Error in %s,line %d,invalid entry definition.\n",db_file,iLine);
				continue;
			}
			key=strLine.substr(0,idx);
			val=strLine.substr(idx+1);
			if(curr_sec){
				if(!curr_sec->add_entry(key,val)){
					utils_error("Error in %s,line %d,duplicated entry key or low memory.\n",db_file,iLine);
				};
			}
		}
	};
	is.close();
	return 1;
};

int CProfile::flush(const char * db_file)
{
	if(!internal_db) return 0;
	CInternalDB & the_db=*((CInternalDB*)internal_db);

	CInternalDB::iterator p;
	ofstream os;
	string key,value;
	int i;

	os.open(db_file);
	// os.open("debug.ini");
	if(!os.good()){
		os.close();
		utils_error("Cannot open db file %s, data will not be saved.",db_file);
		return 0;
	}
	p=the_db.begin();
	while(p != the_db.end() ){
		// piv_debug("Saving section '%s' ...\n",p->first.data());
		if(p->second){
			os<<";-------------------------------------------------------------"<<endl;
			os<<'['<<p->first<<']'<<endl;
			// p->second.save(of);
			for(i=0;i<p->second->size();i++){
				p->second->get_entry(i,key,value);
				os<<key<<'='<<value<<endl;
			}
		}
		p++;
	}
	os.close();
	return 1;
};

int CProfile::unload()
{
	if(!internal_db) {
		return 0;
	}	
	CInternalDB & the_db=*((CInternalDB*)internal_db);
	CInternalDB::iterator p;
	p=the_db.begin();
	while(p != the_db.end() ){
		if(p->second){
			delete p->second;
			p->second=0;
		}
		p++;
	}
	the_db.clear();
	return 1;
};

CProfile::CProfile(const char * db_file)
{
	internal_db=new CInternalDB;
	if(db_file){
		// !!!!!!!!!!!!! big error special error !!!!!!!!!!!!!
		// !!!!!!!!!!!!!	   ´ó´íÌØ´í			 !!!!!!!!!!!!!
		// load_db(db_file);
		load(db_file);
	}
};

CProfile::~CProfile()
{
	unload();
	if(internal_db){
		delete (CInternalDB*)internal_db;
		internal_db=0;
	}
};

int CProfile::get_string(
	const char * lpAppName,		 // points to section name
	const char * lpKeyName,		 // points to key name
	const char * lpDefault,		 // points to default string
	char * lpReturnedString, // points to destination buffer
	int nSize				  // size of destination buffer
	)
{
	string s_key,v_key,value;

	CInternalDB::iterator p;
	string * v;

	CInternalDB & the_db=*((CInternalDB *)internal_db);
	if(!internal_db) return 0;

	s_key=lpAppName;
	v_key=lpKeyName;

	p=the_db.find(s_key);
	if(p == the_db.end() ){
		strncpy(lpReturnedString,lpDefault,nSize);
		lpReturnedString[nSize - 1] = 0;
		return 0;
	}
	v=p->second->get_entry(v_key);
	if(!v){
		strncpy(lpReturnedString,lpDefault,nSize);
		lpReturnedString[nSize - 1] = 0;
		return 0;
	}
	strncpy(lpReturnedString,v->data(),nSize);
	lpReturnedString[nSize - 1] = 0;
	return 1;
}

int CProfile::write_string(
	const char * lpAppName,  // pointer to section name
	const char * lpKeyName,  // pointer to key name
	const char * lpString	  // pointer to string to add
	)
{
	CInternalDB & the_db=*((CInternalDB *)internal_db);
	if(!internal_db) return 0;

	string s_key,v_key,value;
	CInternalDB::iterator p;
	string * v;
	CInternalSection * sec;

	s_key=lpAppName;
	v_key=lpKeyName;
	value=lpString;

	p=the_db.find(s_key);
	if(p == the_db.end() ){
		// create a new section
		sec=new CInternalSection();
		if(!sec){
			return 0;
		}
		pair<CInternalDB::iterator,bool> ib=the_db.insert(CInternalDB::value_type(s_key,sec));
		if(!ib.second){
			delete sec;
			return 0;
		}
		// new section created
		sec->add_entry(v_key,value);
		return 1;
	}
	v=p->second->get_entry(v_key);
	if(!v){
		// create a new entry
		if( !p->second->add_entry(v_key,value) ){
			return 0;
		};
		return 1;
	}
	// modify existing entry
	*v=lpString;
	return 1;
}

int CProfile::size()
{
	if(!internal_db) return 0;
	CInternalDB & the_db=*((CInternalDB *)internal_db);
	return the_db.size();
}

CSection * CProfile::get_section(const char * name)
{
	CSection * sec=new CSection(name,this);
	return sec;
}

int CProfile::drop_section(const char * name)
{
	if(!internal_db) return 0;
	CInternalDB & the_db=*((CInternalDB *)internal_db);
	CInternalDB::iterator p;
	p=the_db.find(name);
	if(p==the_db.end()) return 0;
	delete p->second;
	the_db.erase(p);
	return 1;
}

int CProfile::clear_section(const char * name)
{
	if(!internal_db) return 0;
	CInternalDB & the_db=*((CInternalDB *)internal_db);
	CInternalDB::iterator p;
	p=the_db.find(name);
	if(p==the_db.end()) return 0;
	p->second->clear();
	return 1;
}

int read_string_key(
	const char * lpAppName,		 // points to section name
	const char * lpKeyName,		 // points to key name
	const char * lpDefault,		 // points to default string
	char * lpReturnedString, // points to destination buffer
	int nSize,			   // size of destination buffer
	const char * lpFileName		 // points to initialization filename
	)
{
	CProfile profile(lpFileName);
	return profile.get_string(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize);
}

int write_string_key(
	const char * lpAppName,  // pointer to section name
	const char * lpKeyName,  // pointer to key name
	const char * lpString,   // pointer to string to add
	const char * lpFileName  // pointer to initialization filename
	)
{
	CProfile profile(lpFileName);
	return profile.write_string(lpAppName, lpKeyName, lpString);
}

int read_int_key(
	const char * lpAppName,		 // points to section name
	const char * lpKeyName,		 // points to key name
	int value,
	const char * lpFileName		 // points to initialization filename
	)
{
	CProfile profile(lpFileName);
	profile.read_int_key(lpAppName, lpKeyName, &value); 
	return value;
}


int write_int_key(
	const char * lpAppName,  // pointer to section name
	const char * lpKeyName,  // pointer to key name
	int	 value,
	const char * lpFileName		 // points to initialization filename
	)
{
	CProfile profile(lpFileName);
	return profile.write_int_key(lpAppName, lpKeyName, value);	 
}


int read_dbl_key(
	const char * lpAppName,		 // points to section name
	const char * lpKeyName,		 // points to key name
	double * value,
	const char * lpFileName		 // points to initialization filename
	)
{
	CProfile profile(lpFileName);
	return profile.read_dbl_key(lpAppName, lpKeyName, value);	 
}


int write_dbl_key(
	const char * lpAppName,  // pointer to section name
	const char * lpKeyName,  // pointer to key name
	double value,
	const char * lpFileName		 // points to initialization filename
	)
{
	CProfile profile(lpFileName);
	return profile.write_dbl_key(lpAppName, lpKeyName, value);	 
}
#endif	// HAVE_PROFILE_SUPPORT
