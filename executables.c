#include "header_prompt.c"

void execute()
{
	pid_t pid,wait_pid;
	char *command=args[0];
	int i=0,j=0;
	//printf("bg %d argc %d\n",background,argcount );
	pid=fork();
	if(pid<0)
	{
		perror("ERROR");
	}
	else if(pid==0)
	{
		setpgid(pid, pid);
		//printf("CHILD\n");
		if(background == 0) tcsetpgrp(shell, getpid());
		signal (SIGINT, SIG_DFL);
		signal (SIGQUIT, SIG_DFL);
		signal (SIGTSTP, SIG_DFL);
		signal (SIGTTIN, SIG_DFL);
		signal (SIGTTOU, SIG_DFL);
		signal (SIGCHLD, SIG_DFL);
		int ret = execvp(command, args);
		if ( ret == -1)
			perror("ERROR");
		exit(EXIT_FAILURE);
	}
	else
	{
		//printf("HELLO\n");
		tcsetpgrp(shell, pid);
		add_job(pid,args[0]);
		int state;
		if(background==0) 
		{
			wait_pid = waitpid (pid, &state, WUNTRACED);

			if (!WIFSTOPPED(state)) 
			{
				rem_job(pid);
			}
			else
			{
				//printf("%s\n",args[0]);
				fprintf(stderr, "\nProcess with pid %d has stopped\n", pid);
			}
		}
		else
		{
			//printf("%d %s\n",pid,args[0]);
			printf("Process with Pid: %lld has started.\n",(long long int)pid);
		}
		tcsetpgrp(shell, my_pgid);

		command=NULL;
		free(command);
	}
}

void execIN(char * input2)
{
	int i=0;
	args[0]=strtok(input2," \t\n");
	while(args[i]!=NULL && i<MAXARG-1)
	{
		i++;
		args[i]=strtok(NULL ," \t\n");
	}
	argcount=i-1;
	if (args[0]==NULL)
	{
		return;
	}
	else if (strcmp(args[0],"pwd") == 0 && background==0)
	{
		builtin_pwd(1);
	}
	else if(strcmp(args[0],"cd")==0 && background==0)
	{
		if(args[1]!=NULL)
			builtin_cd(args[1]);
		else
			builtin_cd(HOME);
	}
	else if(strcmp(args[0],"echo")==0 && background==0)
	{
		if(args[1]!=NULL)
			builtin_echo(1);
		else
			printf("\n");
	}
	else if(strcmp(args[0],"ls")==0 && background==0)
	{
		if(args[1]==NULL)
		{
			command_ls(NULL);
		}
		if(args[1]!=NULL)
		{
			if(args[1][0]!='-')
			{
				command_ls(args[1]);
			}
			else
			{
				if(args[2]!=NULL)
				{
					if(args[2][0]!='-')
					{
						if(strcmp(args[1],"-l")==0)
						{
							command_lsl(args[2]);
						}
						if(strcmp(args[1],"-a")==0)
						{
							command_lsa(args[2]);
						}
						if(strcmp(args[1],"-la")==0)
						{
							command_lsb(args[2]);
						}
						if(strcmp(args[1],"-al")==0)
						{
							command_lsb(args[2]);
						}
					}
					else
					{
						if(args[3]!=NULL)
						{
							if(strcmp(args[1],"-l")==0 || strcmp(args[2],"-a")==0)
							{
								command_lsb(args[3]);
							}
							if(strcmp(args[1],"-a")==0 || strcmp(args[2],"-l")==0)
							{
								command_lsb(args[3]);
							}
						}
						else
						{
							if(strcmp(args[1],"-l")==0 || strcmp(args[2],"-a")==0)
							{
								command_lsb(NULL);
							}
							if(strcmp(args[1],"-a")==0 || strcmp(args[2],"-l")==0)
							{
								command_lsb(NULL);
							}
						}
					}
				}
				else
				{
					if(strcmp(args[1],"-l")==0)
					{
						command_lsl(NULL);
					}
					if(strcmp(args[1],"-a")==0)
					{
						command_lsa(NULL);
					}
					if(strcmp(args[1],"-la")==0)
					{
						command_lsb(NULL);
					}
					if(strcmp(args[1],"-al")==0)
					{
						command_lsb(NULL);
					}
				}
			}
		}
	}
	else if(strcmp(args[0],"pinfo")==0 && background==0)
	{
		if(args[1]==NULL)
		{
			pinfo("self");
		}
		else
		{
			pinfo(args[1]);
		}
	}
	else if(strcmp(args[0],"jobs")==0 && background==0)
	{
		jobs();
	}
	else if(strcmp(args[0],"kjob")==0 && background==0)
	{
		kjob();
	}
	else if(strcmp(args[0],"fg")==0 && background==0)
	{
		fg();
	}
	else if(strcmp(args[0],"bg")==0 && background==0)
	{
		bg();
	}
	else if(strcmp(args[0],"overkill")==0 && background==0)
	{
		overkill();
	}
	else if(strcmp(args[0],"setenv")==0 && background==0)
	{
		seten();
	}
	else if(strcmp(args[0],"unsetenv")==0 && background==0)
	{
		unseten();
	}
	else if(strcmp(args[0],"exit")==0 || strcmp(args[0],"quit")==0 )
	{
		exit(0);
	}
	else
	{
		execute();
	}
}
