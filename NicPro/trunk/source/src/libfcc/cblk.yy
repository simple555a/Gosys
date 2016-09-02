%{
/*
	Composite function block grammer file.
	Jackie, 2003/3/18
	Revision history :
	2002/12/xx, first version of resource compiler
	2003/3/18,  resource compiler was discarded from current design,
	instead we use composite function block.
*/
#define yytext cblk_text
#define yylex cblk_lex
#define yyerror cblk_error
%}

%token cblk_TOK_NAT
%token cblk_TOK_ID
%token cblk_TOK_CBLOCK
%token cblk_TOK_USES
%token cblk_TOK_LINKS
%token cblk_TOK_EXPORTS
%token cblk_TOK_POINTER
%token cblk_TOK_END
%token cblk_TOK_UUID
%token cblk_TOK_STRING
%token cblk_TOK_COMMENTS


%token SYM_BLOCK_DECLARATION
%token SYM_LINK_DECLARATION
%token SYM_PIN
%token SYM_EXPORT_DECLARATION

%%

composite_block : cblk_TOK_CBLOCK id ';' uuid comments uses links exports cblk_TOK_END '.'
	{
		$$ = $2;
		g_program = $2;		// id
		$$->type = cblk_TOK_CBLOCK;
		$$->AddChild($4);		// uuid
		$$->AddChild($5);		// comments
		$$->AddChild($6);		// uses
		$$->AddChild($7);		// links
		$$->AddChild($8);		// exports
	}
	;

comments : 
	{
		$$ = new CSymbol(cblk_TOK_COMMENTS);
	}
	| cblk_TOK_COMMENTS comment_list
	{
		$$ = new CSymbol(cblk_TOK_COMMENTS);
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

string : cblk_TOK_STRING
	{
		$$ = new CSymbol(cblk_TOK_STRING);
		$$->val.text = strdup(yytext);
	}
	| string cblk_TOK_STRING
	{
		string s;
		s = string($$->val.text) + yytext;
		free($$->val.text);
		$$->val.text = strdup(s.c_str());
	}
	;

id : cblk_TOK_ID
	{
		$$ = new CSymbol(cblk_TOK_ID);
		$$->val.text = strdup(yytext);
	}
	;

uuid :
	{
		/*
			generate a 8-byte uuid by folding a 16-byte uuid
		*/
		$$ = new CSymbol(cblk_TOK_UUID);
		RTK_GUID guid;
		guid.create();
		int i;
		char * v=(char*)&$$->val.uuid;
		for(i=0; i<8; i++){
			v[i] = guid.data[i] + guid.data[i+8];
		}
	}
	| uuid_const ';'
	{
		$$ = $1;
	}
	;

uuid_const : cblk_TOK_UUID
	{
		char * t;		
		__uint i;
		__u16 * u;

		$$ = new CSymbol(cblk_TOK_UUID	);
		u=(__u16*)(&$$->val.uuid);
		t = yytext;
		t[4] = 0;
		t[9] = 0;
		t[14] = 0;		
				
		sscanf(t, "%x", &i);
		u[0] = i;
		sscanf(t+5, "%x", &i);
		u[1] = i;
		sscanf(t+10, "%x", &i);
		u[2] = i;
		sscanf(t+15, "%x", &i);
		u[3] = i;
	}
	;

uses : cblk_TOK_USES block_list
	{
		$$ = new CSymbol(cblk_TOK_USES);
		if($2){
			$$->AddChild($2);
		}
	}
	;

block_list : 
	{
		$$ = NULL;
	}
	| block_list block_declaration
	{
		if($1){
			$$ = $1;
			$$->AddSibling($2);
		}else{
			$$ = $2;
		}		
	}
	;

block_declaration : id_list ':' id ';'
	{
		$$ = new CSymbol(SYM_BLOCK_DECLARATION);
		$$->AddChild($1);
		$$->AddChild($3);
		// printf("Got function-block %s\n", $3->val.text);
		g_fblocknames.insert(s_i_map::value_type($1->val.text, 1));
	}
	;

id_list : id
	{
		$$ = $1;
	}
	| id_list ',' id
	{
		$$=$1;
		$$->AddSibling($3);
	}
	;

links : cblk_TOK_LINKS link_list
	{
		$$ = new CSymbol(cblk_TOK_LINKS);
		if($2){
			$$->AddChild($2);
		}
	}
	;

link_list : 
	{
		$$ = NULL;
	}
	| link_list link_declaration
	{
		if($1){
			$$ = $1;
			$$->AddSibling($2);
		}else{
			$$ = $2;
		}		
	}
	;

link_declaration : pin cblk_TOK_POINTER pin_list ';'
	{
		$$ = new CSymbol(SYM_LINK_DECLARATION);
		$$->AddChild($1);
		$$->AddChild($3);
	}
	;


pin : id '.' id
	{
		$$ = new CSymbol(SYM_PIN);
		$$->AddChild($1);
		$$->AddChild($3);
	}
	;

pin_list : pin
	{
		$$ = $1;
	}
	| pin_list ',' pin
	{
		$$ = $1;
		$$->AddSibling($3);
	}
	;

exports : cblk_TOK_EXPORTS export_list
	{
		$$ = new CSymbol(cblk_TOK_EXPORTS);
		if($2){
			$$->AddChild($2);
		}
	}
	;

export_list : 
	{
		$$ = NULL;
	}
	| export_list export_declaration
	{
		if($1){
			$$ = $1;
			$$->AddSibling($2);
		}else{
			$$ = $2;
		}
	}
	;

export_declaration : id '=' pin ';'
	{
		$$ = new CSymbol(SYM_EXPORT_DECLARATION);
		$$->AddChild($1);
		$$->AddChild($3);
	}
	;

%%

int yyerror(const char * s)
{
	fprintf(stderr, "Syntax error on line %d:%s", yyline, s);
	return 0;
}

void dumpCblkGrammar()
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
