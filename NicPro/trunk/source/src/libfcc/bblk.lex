%{
	
#define isatty(n) 0
static int lastToken;
static void print(int token);

#ifndef _MAIN_
#define print_ret(n) \
	do{\
		lastToken = n; \
		return n;\
	}while(0)
#else
#define print_ret(n) \
	do{\
		lastToken = n; \
		return n;\
	}while(0)
int yyline=1;
#endif

static char string_buf[256];
static char *string_buf_ptr;

%}
digit           [0-9]
hexdigit		{digit}|[a-f]|[A-F]
letter          [A-Za-z€‚ƒ„…†‡ˆ‰Š‹Œ‘’“”•–—˜™š›œŸ ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäåæçèéêëìíîïğñòóôõö÷øùúûüışÿ]
id              ({letter}|[_])({letter}|{digit}|[_])*
string          {({letter}|{digit}|{other_char})+}
nat				{digit}+
int_num         [-|+]?{nat}

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
	print_ret(TOK_STRING);
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
	// yyline++;
}

"//".*$ {
	// c++ style comment
}

[\n] {
	yyline++; 
}

{int_num} {
	print_ret(TOK_NUMBER);
	}
"@"|","|"."|":"|";"|"("|")"|"%"|"="|"["|"]" {
	return(*yytext);
}
"float" {
	print_ret(TOK_FLOAT);
	}
"bool" {
	print_ret(TOK_BOOL);
	}
"fblock" {
	print_ret(TOK_FBLOCK);
	}
"array" {
	print_ret(TOK_ARRAY);
}
"of" {
	print_ret(TOK_OF);
}
"date" {
	print_ret(TOK_DATE);
}
"double" {
	print_ret(TOK_DOUBLE);
}
"char" {
	print_ret(TOK_CHAR);
}
"short" {
	print_ret(TOK_SHORT);
}
"integer" {
	print_ret(TOK_INTEGER);
	}
"byte" {
	print_ret(TOK_BYTE);
}
"word" {
	print_ret(TOK_WORD);
}
"dword" {
	print_ret(TOK_DWORD);
}
"var" {
	print_ret(TOK_VAR);
}
"hidden" {
	print_ret(TOK_HIDDEN);
}
"implementation" {
	static char buffer[65536];
	int next;
	int i=0;
	memset(buffer, 0, sizeof(buffer));
	next = yyinput();
	while(next != EOF){
		buffer[i++] = next;
		next = yyinput();
	}
	buffer[i] = 0;
	yytext = buffer;
	print_ret(TOK_IMPLEMENTATION);
}

"begin" {print_ret( TOK_BEGIN);}
"end" {print_ret( TOK_END);}
"invoke" {print_ret(TOK_INVOKE);}
"const" {print_ret(TOK_CONST);}
"true" {print_ret(TOK_TRUE);}
"false" {print_ret(TOK_FALSE);}
"event" {print_ret(TOK_EVENT);}
"in" {print_ret(TOK_IN);}
"out" {print_ret(TOK_OUT);}
"timer" {print_ret(TOK_TIMER);}
"comments" {print_ret(TOK_COMMENTS);}
"attribute" {print_ret(TOK_ATTRIBUTE);}

{id}    {
	print_ret(TOK_ID);
	}

[-+]?"0x"{hexdigit}+ {
	print_ret(TOK_HEXNUMBER);
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
	print_ret(TOK_UUID);
}

. {
	fprintf(yyout, "Line %d, %c(%s):Token error.\n", yyline, *yytext, yytext);
	return 0;
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
#define ee(n) case n:fprintf(stderr, "%s=%s\n", #n, yytext);break;
	ee(TOK_NUMBER);
	ee(TOK_BEGIN);
	ee(TOK_END);
	ee(TOK_VAR);
	ee(TOK_FBLOCK);
	ee(TOK_INSTRUCTION);
	ee(TOK_ID);
	ee(TOK_IN);
	ee(TOK_OUT);
	ee(TOK_INVOKE);
	ee(TOK_TIMER);
	ee(TOK_CONST);
	ee(TOK_TRUE);
	ee(TOK_FALSE);
	ee(TOK_EVENT);
	ee(TOK_INTEGER);
	ee(TOK_FLOAT);
	default:
		fprintf(stderr, "%d=%s\n", token, yytext);
	}
}
#else
static void print(int token)
{
}
#endif

#ifdef _MAIN_
YYSTYPE yylval;
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
