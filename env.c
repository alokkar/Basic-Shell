#include "header_prompt.c"

void seten()
{
	if(args[1]==NULL || args[3]!=NULL)
	{
		fprintf(stderr, "Invalid use of setenv\n");
		return;
	}
	if(getenv(args[1]))
	{
		if(setenv(args[1],args[2],2)<0)
		{
			perror("setenv error");
		}
	}
	else
	{
		if(setenv(args[1],args[2],0)<0)
		{
			perror("setenv error");
		}
	}
}

void unseten()
{
	if(args[1]==NULL || args[2]!=NULL)
	{
		fprintf(stderr, "Invalid use of setenv\n");
		return;
	}
	if(unsetenv(args[1])<0)
	{
		perror("setenv error");
	}
}