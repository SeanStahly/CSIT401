#include <stdio.h>

int main(int argc, char ** argv, char ** envp)
{
	printf("Argv Address %p\n", &argv);
	for(int a =0; a <argc; a++)
	{
		printf("Argv[%i] Address %p Pointer Value %p String=%s\n", a, &argv[a], argv[a], argv[a]);
	}
	return 0;
}
