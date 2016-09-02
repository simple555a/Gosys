%{
#define yytext bblk_text
#define yylex bblk_lex
#define yyerror bblk_error
%}
%token TOK_NUMBER
%token TOK_INTEGER
%token TOK_FLOAT
%token TOK_IN
%token TOK_OUT
%token TOK_BOOL
%token TOK_BEGIN 
%token TOK_END 
%token TOK_VAR
%token TOK_INTERFACE
%token TOK_IMPLEMENTATION 
%token TOK_FBLOCK 
%token TOK_INSTRUCTION
%token TOK_ID
%token TOK_TIMER 
%token TOK_INVOKE
%token TOK_CONST
%token TOK_TRUE
%token TOK_FALSE
%token TOK_EVENT
%token TOK_ERROR
%token TOK_ARRAY
%token TOK_OF
%token TOK_DATE
%token TOK_BYTE
%token TOK_WORD
%token TOK_DOUBLE
%token TOK_CHAR
%token TOK_SHORT
%token TOK_DWORD
%token TOK_HEXNUMBER
%token TOK_UUID
%token TOK_STRING
%token TOK_COMMENTS
%token TOK_ATTRIBUTE
%token TOK_HIDDEN
%% 

program : TOK_FBLOCK id ';' blk_attributes uuid comments interface implementation
	{
		$$ = $2; 
		$$->type = sym_program;
		$$->AddChild($4);
		$$->AddChild($5);
		$$->AddChild($6);
		$$->AddChild($7);
		$$->AddChild($8);
		g_program = $$;
		// printf("g_program = 0x%08x\n", g_program);
	}
	;
 
blk_attributes : 
	{
		$$ = new CSymbol(TOK_ATTRIBUTE);
	}
	| TOK_ATTRIBUTE '(' blk_attribute_list ')' ';'
	{
		$$ = new CSymbol(TOK_ATTRIBUTE);
		if($3){
			$$->AddChild($3);
		}
	}
	;

blk_attribute_list : blk_attribute
	{
		$$ = $1;
	}
	| blk_attribute_list ',' blk_attribute
	{
		if($1){
			$$ = $1;
			$$->AddSibling($3);
		}else{
			$$ = $3;
		}
	}
	;

blk_attribute : id
	{
		$$ = new CProperty($1->val.text, 1);
	}
	;

uuid :
	{
		/*
			generate a 8-byte uuid by folding a 16-byte uuid
		*/
		$$ = new CSymbol(TOK_UUID);
		create_f8_uuid(&$$->val.uuid);
	}
	| uuid_const ';'
	{
		$$ = $1;
	}
	;

uuid_const : TOK_UUID
	{
		$$ = new CSymbol(TOK_UUID);
		f8_uuid_from_string(&$$->val.uuid, yytext);
	}
	;

comments : 
	{
		$$ = new CSymbol(TOK_COMMENTS);
	}
	| TOK_COMMENTS comment_list
	{
		$$ = new CSymbol(TOK_COMMENTS);
		if($2){
			$$->AddChild($2);
		}
	}
	;

comment_list :
	{
		$$ = NULL;
	}
	| comment_list comment
	{
		if($1){
			$$ = $1;
			$$->AddSibling($2);
		}else{
			$$ = $2;
		}
	}
	;

comment : id '=' string ';'
	{
		$$ = $1;
		$$->AddChild($3);
	}
	;

string : TOK_STRING
	{
		$$ = new CSymbol(TOK_STRING);
		$$->val.text = strdup(yytext);
	}
	| string TOK_STRING
	{
		string s;
		s = string($$->val.text) + yytext;
		free($$->val.text);
		$$->val.text = strdup(s.c_str());
	}
	;

interface : 
	{
		$$ = new CSymbol(TOK_INTERFACE);
	} | TOK_VAR var_list
	{
		$$ = new CSymbol(TOK_INTERFACE);
		if( $2 ){
			$$->AddChild($2);
		}
	}
	;

var_list :
	{
		$$ = NULL;
	}
	| var_list var_def
	{
		$$ = $1;
		if( !$$ ){
 			$$ = $2;
		}else{
			$$->AddSibling($2);
		}
	}
	;

var_def : id ':' type_def ';' attribute_part
	{
		$$ = new CSymbol(sym_vardecl);
		$$->val.text = $1->val.text;
		$1->val.text = NULL;
		delete $1;
		$$->AddChild($3);
		if($5){
			$$->AddChild($5);
		}
	}
	;
	
type_def : basic_type
	{
		$$ = $1;
	}
	| array_type
	{
		$$ = $1;
	}
	;

basic_type : TOK_EVENT 
	{
		$$ = new CProperty("type", PIN_T_EVENT);
	}
	| TOK_INTEGER 
	{
		$$ = new CProperty("type", PIN_T_INTEGER);
	}
	| TOK_FLOAT 
	{
		$$ = new CProperty("type", PIN_T_FLOAT);
	}
	| TOK_BOOL
	{
		$$ = new CProperty("type", PIN_T_BOOL);
	}
	| TOK_DATE
	{
		$$ = new CProperty("type", PIN_T_DATE);
	}
	| TOK_BYTE
	{
		$$ = new CProperty("type", PIN_T_BYTE);
	}
	| TOK_WORD
	{
		$$ = new CProperty("type", PIN_T_WORD);
	}
	| TOK_DOUBLE
	{
		$$ = new CProperty("type", PIN_T_DOUBLE);
	}
	| TOK_CHAR
	{
		$$ = new CProperty("type", PIN_T_CHAR);
	}
	| TOK_SHORT
	{
		$$ = new CProperty("type", PIN_T_SHORT);
	}
	| TOK_DWORD
	{
		$$ = new CProperty("type", PIN_T_DWORD);
	}
	;

array_type : TOK_ARRAY '[' integer_number ']' TOK_OF basic_type
	{
		$$ = new CProperty("type", PIN_T_ARRAY);
		$$->AddChild($3);
		$$->AddChild($6);
	}
	;

attribute_part :
	{
		$$ = NULL;
	}
	| attribute_list ';'
	{
		$$ = $1;
	}
	;
	
attribute_list : attribute
	{
		$$ = $1;
	}
	| attribute_list ',' attribute
	{
		$$ = $1;
		$$->AddSibling($3);
	}
	;

attribute : TOK_IN 
	{
		$$ = new CProperty("direction", TOK_IN);
	}
	| TOK_OUT
	{
		$$ = new CProperty("direction", TOK_OUT);
	}
	| TOK_TIMER '(' integer_number ')'
	{
		$$ = new CProperty("timer", $3->val.ival);
		delete $3;
	}
	| TOK_INVOKE '(' id ')'
	{
		$$ = new CProperty("invoke", $3->val.text);
		delete $3;
	}
	| TOK_HIDDEN
	{
		$$ = new CProperty("hidden", 0);
	}
	;
	
integer_number : TOK_NUMBER
	{
		$$ = new CSymbol(sym_integer_number);
		$$->val.ival = atoi(yytext);
	}
	| TOK_HEXNUMBER
	{
		$$ = new CSymbol(sym_integer_number);
		sscanf(yytext, "%x", &$$->val.ival);
	}
	;
	
id : TOK_ID 
	{
		$$ = new CSymbol(sym_id);
		$$->val.text = strdup(yytext);
	}
	;

implementation : TOK_IMPLEMENTATION
	{
		$$ = new CSymbol(TOK_IMPLEMENTATION);
		$$->val.text = strdup(yytext);
	};

%%

void dumpBblkGrammar()
{
	int i;	
	int lastN=-1;	
	for(i=1; i<sizeof(yyrline)/sizeof(yyrline[0]); i++){	  
		int yyi;
		unsigned int yylno = yyrline[i];
	  	// YYFPRINTF (stdout, "Rule %03d (line %03u), ", i - 1, yylno);	  
	  	if(lastN != yyr1[i]){
	  		YYFPRINTF (stdout, "\t;\n\n%s -> ", yytname [yyr1[i]]);
	  		lastN=yyr1[i];
	  	}else{
	  		YYFPRINTF (stdout, "\t| ");
	  	}
		/* Print the symbols being reduced, and their result.  */
		for (yyi = yyprhs[i]; 0 <= yyrhs[yyi]; yyi++){
			YYFPRINTF (stdout, "%s ", yytname [yyrhs[yyi]]);
		}
		YYFPRINTF(stdout, "\n");
	}
}
