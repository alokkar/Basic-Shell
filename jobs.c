#include "header_prompt.c"

void add_job(pid_t pid,char * name)
{
	//printf("%s\n",name);
	table[num_jobs].name=(char *)malloc(1000*sizeof(char));
	table[num_jobs].pid=pid;
	strcpy(table[num_jobs].name,name);
	table[num_jobs].active=1;
	num_jobs++;
}
void rem_job(pid_t pid)
{
	int i;
	for (i=0; i <10000 ; i++)
	{
		if(table[i].pid==pid)
		{
			table[i].active=0;
			num_jobs--;
			break;
		}
	}
}
void jobs()
{
	int i,j=1;
	int id;
	char *pname ,pstatus;
	char procdir[1000];
	char * pid;
	pid=(char *)malloc(1000*sizeof(char));
	pname=(char *)malloc(1000*sizeof(char));
	for (i = 0; i <10000; i++)
	{
		if(table[i].active==1)
		{
			strcpy(procdir,"/proc/");
			tostr(table[i].pid,pid);
			strcat(procdir,pid);
			strcat(procdir,"/stat");
			FILE * fd1 =fopen(procdir,"r");
			if(fd1<0)
			{
				perror("Error");
			}
			fscanf(fd1,"%d %s %c",&id,pname,&pstatus);
			fclose(fd1);
			pname++;
			pname[strlen(pname)-1]='\0';
			if(pstatus=='T')
			{
				//printf("[%d]\tSTOPPED\t%s [%d]\n",j,pname,table[i].pid);
				printf("[%d]\tSTOPPED\t%s [%d]\n",j,table[i].name,table[i].pid);
			}
			else if (pstatus=='S' || pstatus=='R')
			{
				//printf("[%d]\tRUNNING\t%s [%d]\n",j,pname,table[i].pid);
				printf("[%d]\tRUNNING\t%s [%d]\n",j,table[i].name,table[i].pid);
			}
			else if (pstatus=='Z')
			{
				//printf("[%d]\tZOMBIE\t%s [%d]\n",j,pname,table[i].pid);
				printf("[%d]\tZOMBIE\t%s [%d]\n",j,table[i].name,table[i].pid);
			}
			j++;
		}
	}
}

void kjob()
{
	if(args[1]==NULL || args[2]==NULL)
	{
		fprintf(stderr, "Invalid use of Kjobs\n");
		return;
	}
	if(args[1]!=NULL && args[2]!=NULL && args[3]!=NULL)
	{
		fprintf(stderr, "Invalid use of Kjobs\n");
		return;
	}
	int i,j=1;
	for (i = 0; i <10000; i++)
	{
		if(table[i].active==1)
		{
			if(j==atoi(args[1]))
				break;
			j++;
		}
	}
	if(i==10000)
	{
		fprintf(stderr, "Job not Found\n");
		return;
	}
	else
	{
		kill(table[i].pid,atoi(args[2]));
	//	rem_job(table[i].pid);
	}
}

void fg()
{
	if(args[2]!=NULL)
	{
		fprintf(stderr, "Invalid use of fg\n");
		return;
	}
	if (args[1]==NULL)
	{
		fprintf(stderr, "Invalid use of fg\n");
		return;
	}
	int i,j=1;
	for (i = 0; i < 10000; ++i)
	{
		if(table[i].active==1)
		{
			if(j==atoi(args[1]))
				break;
			j++;
		}
	}
	if(i==10000)
	{
		fprintf(stderr, "Job not found\n");
		return;
	}
	int state;
	pid_t wait_pid, pid=table[i].pid;
	kill(pid,SIGCONT);
	tcsetpgrp(shell, pid);
	wait_pid = waitpid (pid, &state, WUNTRACED);
	if (!WIFSTOPPED(state)) 
	{
		rem_job(pid);
	}
	else
	{
		fprintf(stderr, "\nProcess with pid %d has stopped\n", pid);
	}
	tcsetpgrp(shell, my_pgid);
}

void bg()
{
	if(args[1]==NULL || args[2]!=NULL)
	{
		fprintf(stderr, "Invalid use of bg\n");
		return;
	}
	int i,j=1;
	for (i = 0; i < 10000; ++i)
	{
		if(table[i].active==1)
		{
			if(j==atoi(args[1]))
				break;
			j++;
		}
	}
	if(i==10000)
	{
		fprintf(stderr, "Job not found\n");
		return;
	}
	kill(table[i].pid,SIGCONT);
}

void overkill()
{
	if(args[1]!=NULL)
	{
		fprintf(stderr, "Invalid use of overkill\n");
		return;
	}
	int i;
	for (i=0;i<10000;i++)
	{
		if (table[i].active==1)
		{
			kill(table[i].pid,SIGKILL);
			rem_job(table[i].pid);
		}
	}
}