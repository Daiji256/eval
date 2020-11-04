#if !defined(EVAL_H)
#define EVAL_H

#define STR_LENGTH 1024

void init(char str[], const char org[]);
void strrep(char str[], const char *bef, const char *aft);
double eval(const char *org);
double eval2(char *str);
double calc(const char *str, char *str2, int i);
double calcfunc(char *str);
int isnumber(char c);
int isoperator(char c);
char *getoperator(char *str);

#endif /* EVAL_H */
