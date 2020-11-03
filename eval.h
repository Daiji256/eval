#if !defined(EVAL_H)
#define EVAL_H

#define STR_LENGTH 1024

void init(const char in[], char out[]);
void strrep(char str[], const char *bef, const char *aft);
double eval(char *str);
double eval2(char *str);
double calc(char *str, int i);
double calcfunc(char *str);
int isnumber(char c);
int isoperator(char c);
char *getoperator(char *str);

#endif /* EVAL_H */
