#include "header_prompt.c"

void command_ls(char * path)
{
	int count,i=0,j=1;
	if(path==NULL)
	{
		count=scandir(".",&list,0,alphasort);
	}
	else
	{
		count=scandir(path,&list,0,alphasort);
	}
	for(i=0;i<count;i++)
	{
		if(list[i]->d_name[0]=='.')
		{
			continue;
		}
		else
		{
			if(piping==1)
			{
				printf("%-20s\n",list[i]->d_name);
			}
			else
			{
				printf("%-20s\t\t",list[i]->d_name);
			}
			j++;
		}
		if(piping==0)
			if(j%6==0)
				printf("\n");
	}
	printf("\n");
}

void command_lsb(char *path)
{
	int count,i=0;
	struct stat filestat;
	char timer[1000];
	if(path==NULL)
	{
		count=scandir(".",&list,0,alphasort);
	}
	else
	{
		count=scandir(path,&list,0,alphasort);
	}
	printf("total files/directories %d\n",count);
	for(i=0;i<count;i++)
	{
		stat(list[i]->d_name,&filestat);
		printf( (S_ISDIR(filestat.st_mode)) ? "d" : "-");
		printf( (filestat.st_mode & S_IRUSR) ? "r" : "-");
		printf( (filestat.st_mode & S_IWUSR) ? "w" : "-");
		printf( (filestat.st_mode & S_IXUSR) ? "x" : "-");
		printf( (filestat.st_mode & S_IRGRP) ? "r" : "-");
		printf( (filestat.st_mode & S_IWGRP) ? "w" : "-");
		printf( (filestat.st_mode & S_IXGRP) ? "x" : "-");
		printf( (filestat.st_mode & S_IROTH) ? "r" : "-");
		printf( (filestat.st_mode & S_IWOTH) ? "w" : "-");
		printf( (filestat.st_mode & S_IXOTH) ? "x" : "-");
		printf(" ");
		printf("%5ld ",(unsigned long)(filestat.st_nlink));
		printf("%15s ",(getpwuid(filestat.st_uid))->pw_name);
		printf("%15s ",(getgrgid(filestat.st_gid))->gr_name);
		printf("%7lld ",(unsigned long long)filestat.st_size);
		strftime (timer,14,"%h %d %H:%M",localtime(&filestat.st_mtime));
		printf("%s ",timer);
		printf("%s\n",list[i]->d_name);
	}
}

void command_lsa(char * path)
{
	int count,i=0,j=1;
	if (path==NULL)
	{
		count=scandir(".",&list,0,alphasort);
	}
	else
	{
		count=scandir(path,&list,0,alphasort);
	}
	for(i=0;i<count;i++)
	{
		if(piping==1)
		{
			printf("%-20s\n",list[i]->d_name);
		}
		else
			printf("%-20s\t\t",list[i]->d_name);
		j++;
		if(piping==0)
			if(j%6==0)
				printf("\n");
	}
	printf("\n");
}

void command_lsl(char * path)
{
	int count,i=0;
	struct stat filestat;
	char timer[1000];
	if (path==NULL)
	{
		count=scandir(".",&list,0,alphasort);
	}
	else
	{
		count=scandir(path,&list,0,alphasort);
	}
	printf("total files/directories %d\n",count-2);
	for(i=0;i<count;i++)
	{
		if(list[i]->d_name[0]=='.')
		{
			continue;
		}
		else
		{
			stat(list[i]->d_name,&filestat);
			printf( (S_ISDIR(filestat.st_mode)) ? "d" : "-");
			printf( (filestat.st_mode & S_IRUSR) ? "r" : "-");
			printf( (filestat.st_mode & S_IWUSR) ? "w" : "-");
			printf( (filestat.st_mode & S_IXUSR) ? "x" : "-");
			printf( (filestat.st_mode & S_IRGRP) ? "r" : "-");
			printf( (filestat.st_mode & S_IWGRP) ? "w" : "-");
			printf( (filestat.st_mode & S_IXGRP) ? "x" : "-");
			printf( (filestat.st_mode & S_IROTH) ? "r" : "-");
			printf( (filestat.st_mode & S_IWOTH) ? "w" : "-");
			printf( (filestat.st_mode & S_IXOTH) ? "x" : "-");
			printf(" ");
			printf("%5ld ",(unsigned long)(filestat.st_nlink));
			printf("%15s ",(getpwuid(filestat.st_uid))->pw_name);
			printf("%15s ",(getgrgid(filestat.st_gid))->gr_name);
			printf("%7lld ",(unsigned long long)filestat.st_size);
			strftime (timer,14,"%h %d %H:%M",localtime(&filestat.st_mtime));
			printf("%s ",timer);
			printf("%s\n",list[i]->d_name);
		}
	}
}
