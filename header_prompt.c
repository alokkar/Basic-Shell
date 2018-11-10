#ifndef Header_prompt
#define Header_prompt 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <sys/wait.h>
#include<signal.h>

void builtin_pwd(int print);
void builtin_cd(char * dir);
void builtin_echo(int argnum);
void command_ls();
void command_lsl();
void command_lsa();
void command_lsb();
void print_prompt();
void execute();
void pinfo(char * pid);
void run_pipe(char * input,int pos);
void parse_input(char * input);
void parse_for_pipe(char * command);
void execIN(char * input2);
void add_job(pid_t pid,char * name);
void rem_job(pid_t pid);
void jobs();
void kjob();
void fg();
void bg();
void seten();
void unseten();
void signal_handler(int signum);
void overkill();
void tostr(int pid, char * ret);

#ifndef MAXARG
#define MAXARG 100
#endif

DIR *mydir;
struct dirent **list;

char *ifname,*ofname;
char *commands[100];
int piping,num_pipe;
pid_t pid,pgid;
int my_pipe[100][2],status;
char HOME[10000], Temp[10000] , Prompt[10000];
char cwd[10000];
char *input, *input2;
char *args[MAXARG];
int argcount=0, background=0 , exitflag=0, quote=0, fileinput=0,fileoutput=0;
pid_t ppid;
int ifd,ifd2,ofd,ofd2;
int foutappend=0;
int num_jobs=0;
int shell=STDERR_FILENO;
pid_t my_pid,my_pgid;

typedef struct Job_table
{
	pid_t pid;
	int active;
	char * name;
} job_table;

job_table table[10000];


void tostr(int pid, char * ret)
{
	int i, rem, len = 0, n;
	n = pid;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	for (i = 0; i < len; i++)
	{
		rem = pid % 10;
		pid = pid / 10;
		ret[len - (i + 1)] = rem + '0';
	}
	ret[len] = '\0';
}

void print_prompt()
{
	char retdir[1000], pwd[1000], *name , hostname[100];
	struct passwd *pass; 
	pass = getpwuid(getuid());
	gethostname(hostname,99); 
	name = pass->pw_name;
	getcwd(pwd,1000);
	int lh=strlen(HOME);
	int ld=strlen(pwd);
	//printf("%s %d\n%s %d\n",HOME,lh,pwd,ld);
	if (strcmp(pwd,HOME)==0)
	{
		//printf("HOME\n");
		strcpy(retdir,"~");
	}
	else
	{
		//printf("NOT HOME\n");
		int j=0;
		while(j<ld)
		{
			if(HOME[j]==pwd[j])
			{
				j++;
			}
			else
				break;
		}
		if(j<lh)
		{
			//printf("HAHA\n");
			strcpy(retdir,pwd);
		}
		else
		{
			//printf("NANA\n");
			strcpy(retdir,"~");
			strcat(retdir,&pwd[j]);
		}
	}
	printf("<%s@%s:%s>",name,hostname,retdir);
}

void signal_handler(int signum) 
{
	if(signum == SIGINT) 
	{
		signal(SIGINT,SIG_IGN);
		signal(SIGINT, signal_handler);
	}
	else if(signum == SIGCHLD) 
	{
		int i, status, child_pid;
		while((child_pid = waitpid(-1, &status, WNOHANG)) > 0) 
		{
			for(i = 0; i < num_jobs; i++)
			{
				if(table[i].active==0) continue;
				else if(table[i].pid == child_pid)
					break;
			}
			if(i != num_jobs) 
			{
				if(WIFEXITED(status))
					fprintf(stdout, "\n%s with pid %d exited normally\n", table[i].name, table[i].pid);
				else if(WIFSIGNALED(status))
					fprintf(stdout, "\n%s with pid %d has exited with signal\n", table[i].name, table[i].pid);
				rem_job(table[i].pid);
			}
		}
	}
}

#endif /* Header_prompt */