#ifndef AST_H
#define AST_H
typedef enum { PRINT_N } NODETYPE;

typedef enum { E_PLUS, E_MINUS, E_DIV, E_MULT, E_INT } EXPRTYPE;

struct I_NODE;

typedef struct I_EXPR {
	EXPRTYPE e;
	union {
		struct {
			struct I_EXPR *l;
			struct I_EXPR *r;
		} bin;
		char *ident; // TODO: how tf do we resolve var.
		int integer;
	} u;
} EXPR;

typedef struct I_STMT {
	NODETYPE t;
	struct I_STMT *next;
	union {
		EXPR *PRINT_E; // print use this
	} u;
} STMT;
#endif
