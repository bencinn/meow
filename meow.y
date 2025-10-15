%{
#include "log.h"
#include "ast.h"
#include <stdlib.h>

int yylex(void);
void yyerror(char *);

EXPR* n_expr_int(int n);
EXPR* n_bexpr(EXPR* left, EXPR* right, EXPRTYPE et);
STMT* n_print_stmt(EXPR* param);

extern STMT* final;

%}

%union {
    int num;
    char* text;
	EXPR* expr;
	STMT* stmt;
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

%type<expr> expr
%type<stmt> stmt_list stmt

%%
program:
	   stmt_list { final = $1; }

stmt_list:
		 stmt { $$ = $1; }
		 | stmt stmt_list {
				($1)->next = $2;
				$$ = $1;
		 }
		 ;

stmt:
		PRINT expr SEMICOLON { $$ = n_print_stmt($2); }
		;

expr:
		INTEGER { $$ = n_expr_int($1); }
		| IDENT { yyerror("ident not implemented"); }
		| STRING { yyerror("string not implemented"); }
		| expr PLUS expr { $$ = n_bexpr($1, $3, E_PLUS); }
		| expr MINUS expr { $$ = n_bexpr($1, $3, E_MINUS); }
		| expr STAR expr { $$ = n_bexpr($1, $3, E_MULT); }
		| expr SLASH expr { $$ = n_bexpr($1, $3, E_DIV); }
		| LPAREN expr RPAREN { $$ = $2; }
		;

%%

EXPR* n_expr_int(int n) {
		EXPR* e = malloc(sizeof(EXPR));
		e->e = E_INT;
		e->u.integer = n;
		return e;
}

EXPR* n_bexpr(EXPR* left, EXPR* right, EXPRTYPE et) {
		EXPR* e = malloc(sizeof(EXPR));
		e->e = et;
		e->u.bin.l = left;
		e->u.bin.r = right;
		return e;
}

STMT* n_print_stmt(EXPR* param) {
		STMT* s = malloc(sizeof(STMT));
		s->t = PRINT_N;
		s->u.PRINT_E = param;
		s->next = NULL;
		return s;
}

void yyerror(char* s) {
	log_error("fuck you: %s", s);
	exit(1);
}
