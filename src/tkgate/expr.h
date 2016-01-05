#ifndef __expr_h
#define __expr_h

#define EE_OK		0
#define EE_RETURN	1
#define EE_BREAK	2
#define EE_NOCASE	3
#define EE_NOTDEF	4
#define EE_NOTLIT	5
#define EE_BADFUNC	6
#define EE_DIV0		7
#define EE_OVERFLOW	8
#define EE_INTERNAL	-1

typedef int EValueLookup(const char *func,const char *name,void *data,int *rval);

typedef struct expr {
  int op;			/* Op code */
  char *lit;			/* Literal or function name  */
  int value;			/* Value of expression  */
  struct expr *l,*r,*x;		/* left, right and extended operands */  
} Expr;

Expr *Expr_lit(const char*);
Expr *Expr_num(int);
Expr *Expr_op(int,Expr*,Expr*);
Expr *Expr_case(int,int);
Expr *Expr_op3(int,Expr*,Expr*,Expr*);
Expr *Expr_func(const char*,Expr*,Expr*);
int Expr_print(Expr *e);
int Expr_sprint(char *buf,int n,Expr *e);

int Expr_eval(Expr *e,int *rval,EValueLookup *f,void *d);

void delete_Expr(Expr *e);

const extern char *expr_errsym;

#endif
