#include "header_prompt.c"

void builtin_pwd(int print)
{
	getcwd(cwd,sizeof(cwd));
	if (print==1)
		printf("%s\n",cwd );
}

void builtin_cd(char * dir)
{
	chdir(dir);
	builtin_pwd(0);
}

void builtin_echo(int argnum)
{
	while(args[argnum]!=NULL && argnum<MAXARG)
	{
		int i;
		char *str;
		str=(char*)malloc(1000*sizeof(char));
		for(i=0;i<strlen(args[argnum]);i++)
		{
			str[i]=args[argnum][i];
		}
		if(str[0]=='"')
		{
			str++;
			if( str[strlen(str)-1]=='"' )
			{
				str[strlen(str)-1]=0;
				if(str[0]=='$')
				{	
					str++;
					if(strcmp(str,"HOME")==0)
					{
						printf("%s ",HOME);
					}
					else if (strcmp(str,"PWD")==0)
					{
						printf("%s ",cwd);
					}
					else
					{
						if(getenv(str))
							printf("%s ",getenv(str));
					}
				}
				else
					printf("%s ",str);
			}
			else
			{
				if(str[0]=='$')
				{
					str++;
					if(strcmp(str,"HOME")==0)
					{
						printf("%s ",HOME);
					}
					else if (strcmp(str,"PWD")==0)
					{
						printf("%s ",cwd);
					}
					else
					{
						if(getenv(str))
							printf("%s ",getenv(str));
					}
				}
				else
				{
					printf("%s ",str);
				}
			}
		}
		else if (str[0]=='$')
		{
			str++;
			if( str[strlen(str)-1]=='"')
				str[strlen(str)-1]=0;
			if(strcmp(str,"HOME")==0)
			{
				printf("%s ",HOME);
			}
			else if (strcmp(str,"PWD")==0)
			{
				printf("%s ",cwd);
			}
			else
			{
				if(getenv(str))
					printf("%s ",getenv(str));
			}
		}
		else
		{
			if( str[strlen(str)-1]=='"' )
			{
				str[strlen(str)-1]=0;
				printf("%s ",str);
			}
			else
			{
				printf("%s ",str);
			}
		}
		argnum++;
	}
	printf("\n");
}
