#ifndef _EVAL_H_
#define _EVAL_H_

#define STR_LENGTH 1024

double eval(const char *org);
double calc(const char *org, char *str, int op);
double calcfunc(const char *org);
void strinit(char *str, const char *org);
void strrep(char *str, const char *bef, const char *aft);
void num2str(char *str, double num);
int isnumber(char c);
int isoperator(char c);
int isfunc(char c);

#endif /* _EVAL_H_ */
