digit [0-9]
letter [a-z]

%{

#include "log.h"
#include "y.tab.h"

void yyerror(char* c);
%}

%%

"for" return FOR;
"from" return FROM;
"print" return PRINT;
".." return DOTDOT;

[1-9]{digit}* {
		yylval.num = atoi(yytext);
		return INTEGER;
}

"[^"\n]*["\n] {
		yylval.text = strdup(yytext+1);
		if (yylval.text[yyleng-2] != '"')
				yyerror("improperly terminated string");
		else
				yylval.text[yyleng-2] = 0;
		return STRING;
}

[ \t\n]+ ;

"(" return LPAREN;
")" return RPAREN;

{letter}({letter}|{digit})* {
		yylval.text = yytext;
		return IDENT;
}

";" return SEMICOLON;
%%

int yywrap(void) {
		return 1;
}
