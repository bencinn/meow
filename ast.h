#ifndef AST_H
#define AST_H

typedef enum { CONST_N, EXPR_N, PRINT_N } NODETYPE;

typedef enum { E_PLUS, E_MINUS, E_DIV, E_MULT, E_PRINT, E_NEXT } EXPRTYPE;

struct I_NODE;

typedef struct {
	EXPRTYPE e;
	struct I_NODE *left;
	struct I_NODE *right;
} EXPR;

typedef struct I_NODE {
	NODETYPE t;
	union {
		int CONST_INT;
		EXPR *EXPRN_EXPR;
	};
} NODE;
#endif
