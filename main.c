#include "header_prompt.c"
#include "pinfo.c"
#include "builtin_commands.c"
#include "ls.c"
#include "executables.c"
#include "pipe.c"
#include "env.c"
#include "jobs.c"

int main()
{
	int i=0;
	size_t buf =0;
	my_pid=my_pgid=getpid();
	signal (SIGINT, SIG_IGN);
	signal (SIGTSTP, SIG_IGN);
	signal (SIGQUIT, SIG_IGN);
	signal (SIGTTIN, SIG_IGN);
	signal (SIGTTOU, SIG_IGN);
	getcwd(cwd,sizeof(cwd));
	strcpy(HOME,cwd);
	printf("\e[1;1H\e[2J");
	while(exitflag==0)
	{
		int exit_stat;
		if(signal(SIGCHLD,signal_handler)==SIG_ERR)
			perror("can't catch SIGCHLD");
		if(signal(SIGINT,signal_handler)==SIG_ERR)
			perror("can't catch SIGINT!");
		{
			print_prompt();
			fflush(stdout);
			fflush(stdin);
			input=NULL;
			fileinput=0;
			fileoutput=0;
			foutappend=0;
			buf=0;
			piping=0;
			quote=0;
			background=0;
			getline(&input,&buf ,stdin);
			input2=(char *)malloc(1000*sizeof(char));
			ifname=(char *)malloc(1000*sizeof(char));
			ofname=(char *)malloc(1000*sizeof(char));
			for (i = 0; i < 1000; ++i)
			{
				ifname[i]=0;
				ofname[i]=0;
				input2[i]=0;
			}
			i=0;int j=0;int k=0,p=0;
			int length = strlen(input);
			commands[0]=(char *)malloc(1000*sizeof(char));
			for(i=0;i<1000;i++)
			{
				commands[0][i]=0;
			}
			for (i = 0; i < length; i++)
			{
				if((input[i]=='"' || input[i]=='\'') && quote==0 )
				{
					quote=1;
					continue;
				}
				if((input[i]=='"' || input[i]=='\'') && quote==1 )
				{
					quote=0;
					continue;
				}
				if(input[i]==';' && quote==0)
				{
					commands[j][k]=0;
					j++;
					k=0;
					commands[j]=(char *)malloc(1000*sizeof(char));
					for(p=0;p<1000;p++)
					{
						commands[j][p]=0;
					}
				}
				else
				{
					if(input[i]==0 || input[i]==10)
					{
						continue;
					}
					else
					{
						commands[j][k]=input[i];
						k++;
					}
				}
			}
			if(input[length-1]!=';')
			{
				j++;
			}
			int numcomm=j;
			for(i=0;i<numcomm;i++)
			{
				parse_for_pipe(commands[i]);
			}
			free(input2);
			free(ifname);
			free(ofname);
		}
	}
	return 0;
}
