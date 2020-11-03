#include "eval.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s \"func\"\n", argv[0]);
		return 0;
	}

	printf("str: %s\n", argv[1]);
	printf("ans: %f\n", eval(argv[1]));

	return 0;
}
