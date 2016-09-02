/*
	mathematic expressioin evaluation procedures

	2001/xx/xx Chenjun
	Created 

	2004/2 Jackie
	added `span' function to support interagral operation, 
*/

// Function.cpp : Defines the class behaviors for the application.
//
#if 0
English 	 <--------->	French
value						valoare
#endif

#include "math.h"
#pragma warning (disable : 4251)
#include "expression.h"
#include <ctype.h>
#include "librtdb.h"
#include "utils.h"

#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "librtdb.lib")
#pragma comment(lib, "libutils.lib")

#define _is_valid_id_char(ch)\
	(ch && (isalnum(ch) || strchr(".$_", ch) || (char)ch < 0))

/////////////////////////////////////////////////////////////////////////////////
// Eliberarea memorie ocupate de arbore

void CExpression::elibmem ( arbore a)
{
	if (a==NULL) return;
	if (a->left!=NULL) elibmem(a->left);
	if (a->right!=NULL) elibmem(a->right);
	delete a;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExpression::CExpression()
{
	m_Arbore = NULL;
	m_definitie = "";
	pozitie = 0;
	m_pvariabile = NULL;
	error_code=0;
}

CExpression::CExpression(CMapVariabile * vars)
{
	m_Arbore = NULL;
	m_definitie = "";
	pozitie = 0;
	m_pvariabile = vars;
}

CExpression::~CExpression()
{
	elibmem (m_Arbore);
}

int CExpression::UpdateArbore()
{
	if (m_definitie.IsEmpty())
		return 0;
	elibmem(m_Arbore); // Eliberarea memoriei ocupate de arbore
	m_Arbore = NULL;
	pozitie = 0;
	m_Arbore = expresie();
	if (m_definitie[pozitie]!='\0'){
		elibmem(m_Arbore);
		m_Arbore = NULL;
	}
	
	if (m_Arbore == NULL){
		return pozitie;
	};
	return -1;
}

CExpression::arbore CExpression::expresie()
{
	arbore nod;
	arbore arb1 = termen();
	arbore arb2;
	if	(arb1 == NULL) return NULL;  // In caz de eroare terminate
	
	while ((m_definitie[pozitie]=='-') || (m_definitie[pozitie]=='+')){
		nod=new NOD;
		nod->left=arb1;
		nod->operatie=m_definitie[pozitie];
		pozitie++;
		arb2 = termen();
		nod->right=arb2;
		if	(arb2 == NULL){
			elibmem(nod);
			return NULL;  // In caz de eroare terminate
		}
		arb1 = nod;
	}
	return arb1;
}

CExpression::arbore CExpression::termen()
{
	arbore nod;
	arbore arb1 = putere();
	arbore arb2;
	if	(arb1 == NULL) return NULL;  // In caz de eroare terminate
	
	while ((m_definitie[pozitie]=='*') 
		|| (m_definitie[pozitie]=='/')
		|| (m_definitie[pozitie]=='%')
	){
		nod=new NOD;
		nod->left=arb1;
		nod->operatie=m_definitie[pozitie];
		pozitie++;
		arb2 = putere();
		nod->right=arb2;
		if	(arb2 == NULL){
			elibmem(nod);
			return NULL;  // In caz de eroare terminate
		}
		arb1 = nod;
	}
	return arb1;
}

CExpression::arbore CExpression::putere()
{
	arbore nod = NULL;
	arbore arb1 = logicalOp();
	arbore arb2;
	if	(arb1 == NULL) return NULL;  // In caz de eroare terminate
	
	while (m_definitie[pozitie]=='^'){
		nod=new NOD;
		nod->left=arb1;
		nod->operatie=m_definitie[pozitie];
		pozitie++;
		arb2 = logicalOp();
		nod->right=arb2;
		if	(arb2 == NULL){
			elibmem(nod);
			return NULL;  // In caz de eroare terminate
		}
		arb1 = nod;
	}
	return arb1;
}


CExpression::arbore CExpression::factor()
{
	arbore nod = NULL,nod2 = NULL,left = NULL;
	
	while(m_definitie[pozitie] == ' ' && m_definitie[pozitie] != '\0'){
		pozitie++;
	}
	
	if (m_definitie[pozitie]=='-'){
		nod = new NOD;
		left = new NOD;
		left->right=NULL;
		left->left=NULL;
		left->operatie='@';
		left->valoare=-1;
		nod->left=left;
		nod->operatie='*';
		pozitie++;
		nod->right = expresie();
		if (nod->right == NULL) return NULL;
		return nod;   
	}
	if (m_definitie[pozitie]=='('){
		pozitie++;
		nod = expresie();
		if (nod == NULL) return NULL;
		if (m_definitie[pozitie]!=')'){
			elibmem(nod);
			return NULL;
		}
		pozitie++;
		return nod;
	/*
	}else if (m_definitie[pozitie]=='|'){
		pozitie++;
		nod2 = expresie();
		if (nod2 == NULL) return NULL;
		if (m_definitie[pozitie]!='|'){
			elibmem(nod);
			return NULL;
		}
		nod = new NOD;
		nod->left=nod2;
		nod->right=NULL;
		nod->operatie='|';
		pozitie++;
		return nod;
	*/
	}else return identificator();
	return nod;
}

CExpression::arbore CExpression::identificator()
{
	int poz;
	arbore nod = NULL,nod2 = NULL;
	arbore result = NULL;
	TAG_NAME tn;

	poz=pozitie;
	SkipSpaces();

	if (m_definitie[pozitie]=='\0') result	= NULL;

	if (isdigit(m_definitie[pozitie])){  // Este numar ?
		while  ((isdigit(m_definitie[pozitie]) || (m_definitie[pozitie]=='.'))){
			pozitie++;
		}
		nod = new NOD;
		nod->left = NULL; 
		nod->right = NULL;
		nod->operatie = '@';
		nod->valoare = atof(m_definitie.Mid(poz,pozitie-poz));
		result = nod;
	}else if (_is_valid_id_char(m_definitie[pozitie])){  // Am i an identifier ?
		//if (isalpha(m_definitie[pozitie]) ){	// Am i an identifier ?
		//while  (isalnum(m_definitie[pozitie])) 
		LPCSTR str = m_definitie;
		while  (_is_valid_id_char(str[pozitie])) {
			pozitie++;
		}
		
		CString id = m_definitie.Mid(poz,pozitie-poz);
		CString nid = id;
		id.MakeUpper();
		
		if (id =="SIN"){	 // Functia sinus CString
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=150;
			SkipSpaces();
			return nod;
		}else if (id =="COS")	  // Functia sinus CString
		{
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=151;
			SkipSpaces();
			return nod;
		}else if (id =="EXP") 	// Functia sinus CString
		{
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=152;
			SkipSpaces();
			return nod;
		}else if (id =="SQRT")		// Functia sinus CString
		{
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=153;
			SkipSpaces();
			return	nod;
		}else if (id =="LOG") 	// Functia sinus CString
		{
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=154;
			SkipSpaces();
			return nod;
		}else if (id =="TG")		// Functia sinus CString
		{
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=155;
			SkipSpaces();
			return nod;
		}else if (id =="CTG"){
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=156;
			SkipSpaces();
			return nod;
		}else if (id =="ASIN"){
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=157;
			SkipSpaces();
			return nod;
		}else if (id =="ACOS"){
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=158;
			SkipSpaces();
			return nod;
		}else if (id =="ATG"){
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=159;
			SkipSpaces();
			return nod;
		}else if (id =="$YEAR"){
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=160;
			SkipSpaces();
			return nod;
		}else if (id =="$MONTH"){
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=161;
			SkipSpaces();
			return nod;
		}else if (id =="$DAY"){
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=162;
			SkipSpaces();
			return nod;
		}else if (id =="$HOUR"){
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=163;
			SkipSpaces();
			return nod;
		}else if (id =="$MINUTE"){
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=164;
			SkipSpaces();
			return nod;
		}else if (id =="$SECOND"){
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=165;
			SkipSpaces();
			return nod;
		}else if (id =="MAX"){
			SkipSpaces();
			if(m_definitie[pozitie] != '('){
				return NULL;
			}
			pozitie++;
			nod2 = factor();
			// skip ';'
			SkipSpaces();
			if(m_definitie[pozitie] != ';'){
				return NULL;
			}
			pozitie++;
			arbore nod3 = factor();
			if(!nod3){
				return NULL;
			}
			SkipSpaces();
			if(m_definitie[pozitie] != ')'){
				return NULL;
			};
			pozitie++;
			nod = new NOD;
			nod->left = nod2;
			nod->right = nod3;
			nod->operatie=166;
			SkipSpaces();
			return nod;
		}else if (id =="MIN"){
			SkipSpaces();
			if(m_definitie[pozitie] != '('){
				return NULL;
			}
			pozitie++;
			nod2 = factor();
			// skip ';'
			SkipSpaces();
			if(m_definitie[pozitie] != ';'){
				return NULL;
			}
			pozitie++;
			arbore nod3 = factor();
			if(!nod3){
				return NULL;
			}
			SkipSpaces();
			if(m_definitie[pozitie] != ')'){
				return NULL;
			}
			pozitie++;
			nod = new NOD;
			nod->left = nod2;
			nod->right = nod3;
			nod->operatie=167;
			SkipSpaces();
			return nod;
		}else if (id =="RANDOM"){
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=168;
			SkipSpaces();
			return nod;
		}else if (id == "$NOW"){
			nod2 = factor();
			nod = new NOD;
			nod->left = NULL;
			nod->right=NULL;
			nod->operatie=169;
			SkipSpaces();
			return nod;
		}else if (id == "LASTUPDATE"){
			/* added by jackie, 2004/2/26 */
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=170;
			SkipSpaces();
			return nod;
		}else if(id == "SPAN"){
			/* added by jackie, 2004/2/26 */
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=172;
			SkipSpaces();
			return nod;
		}else if(id == "ABS"){
			// added by chenj, 2008-5-22
			nod2 = factor();
			nod = new NOD;
			nod->left = nod2;
			nod->right=NULL;
			nod->operatie=173;
			SkipSpaces();
			return nod;
		}

		// is not a keyward, check if it's a database tag
		if(tagname_by_text(&tn, (char*)(LPCTSTR)id) ){
			nod = new NOD;
			nod -> left = NULL;
			nod -> right = NULL;
			nod -> operatie = 171;
			nod -> pvObj = query_tag(HNODE_LOCAL_MACHINE, &tn.sname);
			SkipSpaces();
			result = nod;
		}else{
			result = NULL;
		}
	}
	SkipSpaces();
	return result;		  
}

int CExpression::ChangeExpression(CString & expresie)
{
	m_definitie = expresie + '\0' + '\0';
	return UpdateArbore();
}

int CExpression::Value(double& valoare)
{
	error_code=0;
	if (m_Arbore == NULL) return -1;
	valoare=vexp(m_Arbore);
	return (error_code);
}

/**/
double CExpression::vexp ( arbore a )
{
	double v;
	SYSTEMTIME tm;
	
	GetSystemTime(&tm);

	if (a->operatie==NULL) {error_code=10;return 0;}
	switch(a->operatie){
	case '+' : return( vexp(a->left)+vexp(a->right) );
	case '-' : return( vexp(a->left)-vexp(a->right) );
	case '*' : return( vexp(a->left)*vexp(a->right) );
	case '%':
		{
			v = vexp(a->right);
			if(v == 0){
				error_code = DIVISION_BY_0;
				return 0;
			}
			return (int)vexp(a->left) % (int)v;
		}
	case '/' : v=vexp(a->right) ;
		if (v==0){
			error_code=DIVISION_BY_0;
			return -vexp(a->left)/0.001;
		}else{
			return(vexp(a->left)/v);
		}
	case 150 : return(sin(vexp(a->left)));
	case 151 : return(cos(vexp(a->left)));
	case 152 : return(exp(vexp(a->left)));
	case 153 : v=vexp(a->left) ;
		if (v<0) {error_code=INVALID_DOMAIN;return 0;}
		else return(sqrt(v));
	case 154 : v=vexp(a->left) ;
		if (v<=0) {error_code=INVALID_DOMAIN;return 0;}
		else return(log(v));
	case 155 : return (tan (vexp(a->left)));
	case 156 : return (1 / tan (vexp(a->left)));
	case 157 : return (asin (vexp(a->left)));
	case 158 : return (acos (vexp(a->left)));
	case 159 : return (atan (vexp(a->left)));
	case 173 : return (fabs (vexp(a->left)));
	case 160 : return tm.wYear;
	case 161 : return tm.wMonth;
	case 162 : return tm.wDay;
	case 163 : return tm.wHour;
	case 164 : return tm.wMinute;
	case 165 : return tm.wSecond;
	case 166 : return max(vexp(a->left),vexp(a->right));
	case 167 : return min(vexp(a->left),vexp(a->right));
	case 168 : return rng_rand(0,RAND_MAX)*vexp(a->left)/RAND_MAX;
	//case '|' : return(fabs(vexp(a->left)));
	case '^' : return(pow(vexp(a->left),vexp(a->right)));
	case '@' : return (a->valoare);
		//logical operations evaluation
	case '<' : return( vexp(a->left) < vexp(a->right) );
	case '>' : return( vexp(a->left) > vexp(a->right) );
	case '!' : return(!vexp(a->right)) ;
	// added by chenj, @2008-5-22
	case '=' : return( vexp(a->left) == vexp(a->right) );
	case '&' : return (int)(vexp(a->left)) & (int)(vexp(a->right));
	case '|' : return (int)(vexp(a->left)) | (int)(vexp(a->right));
		
	case 169:
		{
			RTK_TIME t;
			rtk_time_mark(&t);
			return (double)(__int64)t.Data / 1e7;
		}
		
	case 170:
		{
			/* last update time */
			PRTK_TAG tte;
			__r8 retval = 0;
			
			tte  = (PRTK_TAG)a->left->pvObj;
			if(!tte){
				error_code=UNDEFINED_VARIABLE;
				retval = 0;
			}else{ 
				if(!(tte->d.Value.Flags & TF_Valid)){
					error_code=UNDEFINED_VARIABLE;
					retval = 0;
				}else{
					PRTK_TIME pTime = (PRTK_TIME)&tte->d.BinaryAddress[8];
					retval = (double)(__int64)pTime->Data / 1e7;
					rtk_time_mark(pTime);
				}
			}

			return retval;
		}

	case 171  : 
		{
			// a database tag
			PRTK_TAG tte;
			__r8 retval = 0;
			tte = (PRTK_TAG)a->pvObj;
			if(!tte){
				error_code=UNDEFINED_VARIABLE;
				retval = 0;
			}else{ 
				if(!(tte->d.Value.Flags & TF_Valid)){
					error_code=UNDEFINED_VARIABLE;
					retval = 0;
				}else{
					pmc_value_t dblVal;
					set_value_type(dblVal.Flags, dt_real8);
					pmc_type_cast(&tte->d.Value, &dblVal);
					retval = dblVal.Value.dbl;
				}
			}
			return retval;
		}
	
	case 172:
		{
			/* span time */
			PRTK_TAG tte;
			RTK_TIME now;
			__r8 retval = 0;
			
			tte  = (PRTK_TAG)a->left->pvObj;
			if(!tte){
				error_code=UNDEFINED_VARIABLE;
				retval = 0;
			}else{ 
				if(!(tte->d.Value.Flags & TF_Valid)){
					error_code=UNDEFINED_VARIABLE;
					retval = 0;
				}else{
					PRTK_TIME pTime = (PRTK_TIME)&(tte->d.BinaryAddress[8]);
					rtk_time_mark(&now);
					if(pTime->Data != 0){
						/* yes, the field is previouly stored with a resonable value,
						thus valid for a sub-operation to get a duration time */
						retval = rtk_time_diff(&now, pTime);
					}else{
						/* this might be the first time that a time-span was requested
						for this tag
						*/
						retval = 0;
					}
					
					*pTime = now;
				}
			}

			return retval;
		}

	}

	return 0;
}


CExpression::arbore CExpression::GetArbore()
{
	return m_Arbore;
}


CExpression::CExpression(CExpression & expresie)
{
	*this = expresie;
}

CExpression::arbore CExpression::CloneTree()
{
	return clone(m_Arbore); 
}

void CExpression::AtachVariables(CMapVariabile * vars)
{
	m_pvariabile = vars;
}

CExpression::arbore CExpression::clone(arbore arb)
{
	if (arb == NULL){
		return NULL;
	}
	arbore clonArb = new NOD;
	*clonArb = *arb;
	clonArb->left = clone(arb->left);
	clonArb->right = clone(arb->right);
	return clonArb;
}

CExpression& CExpression::operator=(CExpression &expr)
{
	m_definitie = expr.m_definitie;
	m_pvariabile = expr.m_pvariabile;
	pozitie = 0;
	m_Arbore = expr.CloneTree();
	return *this;
}

void CExpression::SkipSpaces()
{
	char c=m_definitie[pozitie];
	while ((c==' ') && c!='\0'){
		pozitie ++;
		c=m_definitie[pozitie];
	};
}


///////////////////////////////////////////////////////////////////////
// the new inserted operations
// logical operations on the first level of priority
// warning: for operations with more than one character you nedd to modify sligthly the error_code
// See also the new inserted operations in the vexp function

CExpression::arbore CExpression::logicalOp()
{
	arbore nod;
	arbore arb1 = sgOp();
	arbore arb2;
	if	(arb1 == NULL) return NULL;  // In caz de eroare terminate
	
	while ((m_definitie[pozitie]=='<') || (m_definitie[pozitie]=='>') 
		|| (m_definitie[pozitie]=='=')
		|| (m_definitie[pozitie]=='&')
		|| (m_definitie[pozitie]=='|')
		){ /* || another same priority operations*/
		nod=new NOD;
		nod->left=arb1;
		nod->operatie=m_definitie[pozitie];
		pozitie++;
		arb2 = sgOp();
		nod->right=arb2;
		if	(arb2 == NULL){
			elibmem(nod);
			return NULL;  // In caz de eroare terminate
		}
		
		arb1 = nod;
	}
	return arb1;
}


CExpression::arbore CExpression::sgOp()
{
	arbore nod = NULL;
	arbore arb2;
	if ((m_definitie[pozitie]=='!')){ /* || another same priority operations*/
		nod=new NOD;
		nod->left=NULL;
		nod->operatie=m_definitie[pozitie];
		pozitie++;
		arb2 = sgOp();
		nod->right=arb2;
		if	(arb2 == NULL){
			elibmem(nod);
			return NULL;  // In caz de eroare terminate
		}
	}else{
		nod = factor();
	};
	return nod;
};

LIBMATH_API __bool __stdcall evaluate(const char * text, double * val)
{
	CExpression expresie;
	CString s;
	s = text;
	s.Remove(' ');
	if( expresie.ChangeExpression(s) >= 0 ){
		return __false;
	}
	if(expresie.Value(*val)){
		return __false;
	}
	return __true;
}
