#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_LENGTH 1024
enum token {NUMBER = 1, OPERATOR = 2, LEFTPAREN = 3, RIGHTPAREN = 4};

struct count
{
	int leftparen;
	int number;
	int operator;
	int sign;
	int rightparen;
};

int isoperator(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/') return 1;
	return 0;
}

char *getoperator(char *exp)
{
	int i, count = 0;

	for (i = 0; (count != 0 || exp[i] != ')') && exp[i] != '\0'; i++)
	{
		if (count == 0 && isoperator(exp[i])) return &exp[i];
		else if (exp[i] == '(') count++;
		else if (exp[i] == ')') count--;
	}
	return NULL;
}

int calc(char *str, int i)
{
	int j = i, k,  tmp = 0, a, b;
	//式中を配列で操作するための添え字j
	//文字列置換字に使うループ用変数k
	//式の値を保存する変数tmp
	//式の左辺、右辺の値を保存するa,b

	char exp[STR_LENGTH], val[10];
	//演算している式を保存する文字列exp
	//値を文字列として使用するval

	char *pl, *pr, *next;
	//左辺の値の先頭pl、右辺の値の終端pr
	//右辺の先の演算子をチェックする時に使うnext

	while(1)
	{   //左辺の値を検索する
		j--;
		if (str[j] == ')')
		{ //左辺終端がカッコとじである場合
			while (str[j] != '(')
			{
				//カッコまで進めて、evalで左辺値を求める
				j--;
				if (j == -1) exit(EXIT_FAILURE);
			}
			pl = str + j;   //左辺値の先頭を保存
			a = eval(pl);
			//左辺の値は得たので、ループを抜ける
			break;

		}
		else if (isdigit(str[j]))
		{	//数だった場合
			while (isdigit(str[j - 1]))
			{	//数の先頭までずらしてatoi
				j--;
				if (j == 0) exit(EXIT_FAILURE);
			}
			pl = str + j;   //左辺の先頭を保存
			a = atoi(pl);   //値を取得して、ループを抜ける
			break;
		}
		else if (str[j] == '(')
		{  //演算子が式先頭のマイナスだった場合
			a = 0;  //左辺値は0として演算すればよい
			pl = str + j + 1;
			break;
		}
	}

	j = i;  //今度は右辺の値を格納する
	while (str[j] != ')')
	{  //最大で右辺終端まで
		j++;
		if (str[j] == '(')
		{ //右辺がカッコである場合
			pr = str + j;   //右辺値の値先頭アドレスを保存
			if (str[i] == '+' || str[i] == '-')
			{	//現在の式の演算子が加減である場合、先の乗除をチェックする
				if ((next = getoperator(pr)) != NULL)
				{  //右辺の先に乗除がないか
					switch(*next)
					{
						case '*':
						case '/': b = calc(pr, next - pr); break;
						case '+':
						case '-': b = eval(pr); break;
						default: break;
					}
				}
				else b = eval(pr);
			}
			else b = eval(pr);

			//右辺値終端を式の終わりと見るため
			//右辺値の終端を探す
			if (str[j] == '(')
			{ //右辺先頭がカッコである場合
				while (str[j] != ')')
				{
					j++;
					if (str[j] == '\0') exit(EXIT_FAILURE);
				}
			}
			else if (isdigit(str[j])) while (isdigit(str[j + 1])) j++;
			else exit(EXIT_FAILURE);
			//このときstr + jは右辺値の終端を指す
			break;
		}
		else if (isdigit(str[j]))
		{	//右辺値が数の場合
			pr = str + j;   //右辺値先頭を保存
			if (str[i] == '+' || str[i] == '-')
			{
				if ((next = getoperator(pr)) != NULL)
				{  //同じく右辺の先の乗除をチェック
					switch(*next)
					{
						case '*':
						case '/': b = calc(pr, next - pr); break;
						case '+':
						case '-': b = atoi(pr); break;
						default: break;
					}
				}
				else b = atoi(pr);   //atoiで数を求める
			}
			else b = atoi(pr);   //自身の演算が乗除であった場合、先に計算

			if (str[j] == '(')
			{ //右辺先頭がカッコである場合
				while (str[j] != ')')
				{
					j++;
					if (str[j] == '\0') exit(EXIT_FAILURE);
				}
			}
			else if (isdigit(str[j])) while (isdigit(str[j + 1])) j++;
			else exit(EXIT_FAILURE);
			break;
		}
	}

	//右辺、左辺の値を受け取ったので演算する
	switch(str[i])
	{
		case '+': tmp = a + b; break;
		case '-': tmp = a - b; break;
		case '*': tmp = a * b; break;
		case '/':
		{
			if (b == 0) exit(EXIT_FAILURE);
			else tmp = a / b;
			break;
		}
		default: break;
	}

	//計算の終わった式を消去する
	//左辺値の先頭が式の始まり、右辺値の計算より、str + jが終端である
	k = 0;
	while (pl + k <= str + j)
	{   //まずは演算した式をexpにコピー
		exp[k] = pl[k];
		k++;
	}
	exp[k] = '\0';

	if (tmp >= 0) sprintf(val, "%d", tmp);
	else sprintf(val, "(%d)", tmp);

	mystrrep(str, exp, val);	//置換
	return tmp;
}

int eval(char *str)
{
	int i, tmp, count;
	//ループ用変数i、演算結果を保存するtmp、カッコ のカウント用count
	char *p;
	//ポインタ操作用p

	count = 0;
	p = str + 1;
	tmp = atoi(p);
	while (1)
	{
		if (*p == '(') if ((tmp = eval(p)) < 0) count++;
		else if (*p == ')') count--;
		else if (*p == '\0') exit(EXIT_FAILURE);
		p++;
		if (count < 0) exit(EXIT_FAILURE);
		if (count == 0 && *p == ')') break;
	}

	while ((p = getoperator(str + 1)) != NULL) tmp = calc(str, p - str);

	return tmp;
}

void strrep(char str[], const char *bef, const char *aft)
{
	char tmp[STR_LENGTH], *p;

	p = str;
	if (p = strstr(p, bef))
	{
		strcpy(tmp, p + strlen(bef));
		*p = '\0';
		strcat(str, aft);
		strcat(str, tmp);
	}
}

void init(const char in[], char out[])
{
	sprintf(out, "(%s)", in);
	while (strchr(out, " ")) strrep(out, " ", "");
}

int main(int argc, char **argv)
{
	char str[STR_LENGTH];

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s \"func\"\n", argv[0]);
		return 0;
	}

	init(argv[1], str);

	return 0;
}
