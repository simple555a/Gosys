%{
#define isatty(n) 0

static void print(int n);

#if !defined(_MAIN_)
#define print_ret(n) do{\
	if(cblk_debug) \
		print(n); \
	return n;\
	}while(0)
#else
#define print_ret(n) return n
#endif

static char string_buf[256];
static char *string_buf_ptr;
%}
digit           [0-9]
letter          [A-Za-z€‚ƒ„…†‡ˆ‰Š‹Œ‘’“”•–—˜™š›œŸ ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäåæçèéêëìíîïğñòóôõö÷øùúûüışÿ]
id              ({letter}|[_])({letter}|{digit}|[_])*
nat				{digit}+

%x str

%%

\"      {
	string_buf_ptr = string_buf; 
	BEGIN(str);
}

<str>\"        { /* saw closing quote - all done */
	BEGIN(INITIAL);
	*string_buf_ptr = '\0';
	/* return string constant token type and
	* value to parser
	*/
	yytext = string_buf;
	print_ret(cblk_TOK_STRING);
}

<str>\n        {
	/* error - unterminated string constant */
	/* generate error message */
}

<str>\\[0-7]{1,3} {
	/* octal escape sequence */
	int result;

	(void) sscanf( yytext + 1, "%o", &result );

	if ( result > 0xff )
		 /* error, constant is out-of-bounds */

	*string_buf_ptr++ = result;
}
 
<str>\\[0-9]+ {
	/* generate error - bad escape sequence; something
	 * like '\48' or '\0777777'
	 */
}

<str>\\n  *string_buf_ptr++ = '\n';
<str>\\t  *string_buf_ptr++ = '\t';
<str>\\r  *string_buf_ptr++ = '\r';
<str>\\b  *string_buf_ptr++ = '\b';
<str>\\f  *string_buf_ptr++ = '\f';

<str>\\(.|\n)  *string_buf_ptr++ = yytext[1];

<str>[^\\\n\"]+        {
	char *yptr = yytext;

	while ( *yptr ){
		*string_buf_ptr++ = *yptr++;
	}
}

[ |\t]+ {
}

^"#".*$ {
	// lines starting with '#' are comments
}

[\n] {
	yyline++; 
}

"#".*[\n] {
	/* comment line */
	yyline++;
}
"@"|","|"."|":"|";"|"("|")"|"%"|"="|"["|"]" {
	print_ret(*yytext);
}
"->" {
	print_ret(cblk_TOK_POINTER);
	}
{id} {
	int token;
	if(type_from_name(yytext, &token) ){
		print_ret(token);
	}
	print_ret(cblk_TOK_ID);
}

"uuid{".*"}" {
	if(strlen(yytext) != 6+16+3){
		fprintf(
			yyout, 
			"Line %d, %c(%s):Token error.\n", 
			yyline, 
			*yytext, 
			yytext
			);
		return 0;
	}
	yytext += 5;
	yytext[19] = 0;
	print_ret(cblk_TOK_UUID);
}

{nat} {
	print_ret(cblk_TOK_NAT);
	}
%%
int yywrap()
{
	return 1;
}

#ifdef YYDEBUG
static void print(int token)
{
	switch(token){
#define ee(n) case n:printf("%s=%s\n", #n, yytext);break;
	ee(cblk_TOK_NAT);
	ee(cblk_TOK_USES);
	ee(cblk_TOK_EXPORTS);
	ee(cblk_TOK_CBLOCK);
	ee(cblk_TOK_LINKS);
	ee(cblk_TOK_POINTER);
	ee(cblk_TOK_ID);
	ee(cblk_TOK_END);
	ee(cblk_TOK_UUID);
	ee(cblk_TOK_STRING);
	ee(cblk_TOK_COMMENTS);
	default:
		printf("%d=%s\n", token, yytext);
		break;
	} 
}
#else
static void print(int token)
{
}
#endif

s_i_map g_fblocknames;

__bool type_from_name(const char * text, int * token)
{
#define dd(t,k) if( !strcmp(text, t) ){*token = k; return __true;}
	dd("cblock", cblk_TOK_CBLOCK);
	dd("uses", cblk_TOK_USES);
	dd("links", cblk_TOK_LINKS);
	dd("end", cblk_TOK_END);
	dd("exports", cblk_TOK_EXPORTS);
	dd("comments", cblk_TOK_COMMENTS);
#undef dd

/*
	s_i_map::iterator p;
	p = g_fblocknames.find(text);
	if(p != g_fblocknames.end()){
		*token = cblk_TOK_FBLOCK_NAME;
		return __true;
	}
*/
	return __false;
}

#ifdef _MAIN_
YYSTYPE yylval;
int yyline = 1;
main()
{
	int token=1;
	do{
		token = yylex();
		print(token);
	}while(token);
	return 0;
}
#endif