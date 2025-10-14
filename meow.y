%{
#include "log.h"
#include "ast.h"
#include <stdlib.h>

int yylex(void);
void yyerror(char *);

NODE* n_const_int(int n);
NODE* n_expr(NODE* left, NODE* right, EXPRTYPE e);

extern NODE* final;

%}

%union {
    int num;
    char* text;
	struct I_NODE* n;
}
%token <num> INTEGER
%token<text> IDENT
%token<text> STRING

%token DOTDOT
%token SEMICOLON

%token PLUS
%token MINUS
%token STAR
%token SLASH

%token LPAREN
%token RPAREN

%token LBRACE
%token RBRACE

%token PRINT
%token FOR
%token FROM

%left PLUS MINUS
%left STAR SLASH

%type<n> expr stmt stmt_list 

%%
program:
	   stmt_list { final = $1; }

stmt_list:
		 stmt { $$ = $1; }
		 | stmt_list stmt { $$ = n_expr($1, $2, E_NEXT); }
		 ;

stmt:
		PRINT expr SEMICOLON { $$ = n_expr($2, NULL, E_PRINT); }
		;

expr:
		INTEGER { $$ = n_const_int($1); }
		| IDENT { $$ = NULL; }
		| STRING { yyerror("string not implemented"); }
		| expr PLUS expr { $$ = n_expr($1, $3, E_PLUS); }
		| expr MINUS expr { $$ = n_expr($1, $3, E_MINUS); }
		| expr STAR expr { $$ = n_expr($1, $3, E_MULT); }
		| expr SLASH expr { $$ = n_expr($1, $3, E_DIV); }
		| LPAREN expr RPAREN { $$ = $2; }
		;

%%

NODE* n_const_int(int n) {
		log_trace("creating CONST_INT %d", n);
		NODE* no = malloc(sizeof(NODE));
		no->t = CONST_N;
		no->CONST_INT = n;
		return no;
}

NODE* n_expr(NODE* left, NODE* right, EXPRTYPE e) {
		log_trace("creating EXPR_N %d", e);
		NODE* no = malloc(sizeof(NODE));
		no->t = EXPR_N;
		EXPR* ee = malloc(sizeof(EXPR));
		ee->left = left;
		ee->right = right;
		ee->e = e;

		no->EXPRN_EXPR = ee;
		return no;
}

void yyerror(char* s) {
	log_error("fuck you: %s", s);
	exit(1);
}
