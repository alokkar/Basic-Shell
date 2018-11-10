#include "header_prompt.c"

void pinfo(char * pid)
{
	char procdir[1000];
	int i;
	strcpy(procdir,"/proc/");
	strcat(procdir,pid);
	strcat(procdir,"/stat");
	FILE * fd1 =fopen(procdir,"r");
	if(fd1<0)
	{
		perror("Error");
	}

	int id,vm;
	char pname[1000] ,pstatus;
	fscanf(fd1,"%d %s %c",&id,pname,&pstatus);

	fclose(fd1);


	strcpy(procdir,"/proc/");
	strcat(procdir,pid);
	strcat(procdir,"/status");
	fd1 =fopen(procdir,"r");
	if(fd1<0)
	{
		perror("Error");
	}
	i=0;
	char data[1000], name[1000] , exepath[1000],temp[1000] , finalpath[1000];
	while(fgets(data,25,fd1)!=NULL)
	{
		if(i==0)
		{
			sscanf(data,"%s%s",temp,name);
		}
		if(i==19)
		{
			sscanf(data,"%s%d",temp,&vm);
		}
		i++;
	}
	fclose(fd1);

	strcpy(procdir,"/proc/");
	strcat(procdir,pid);
	strcat(procdir,"/exe");
	if(readlink(procdir,exepath,1000*sizeof(char)) < 0)
	{
		perror("Error");
	}


	int lh=strlen(HOME);
	int ld=strlen(exepath);
	//printf("%s %d\n%s %d\n",HOME,lh,exepath,ld);
	if (strcmp(exepath,HOME)==0)
	{
		//printf("HOME\n");
		strcpy(finalpath,"~");
	}
	else
	{
		//printf("NOT HOME\n");
		int j=0;
		while(j<ld)
		{
			if(HOME[j]==exepath[j])
			{
				j++;
			}
			else
				break;
		}
		if(j<lh)
		{
			//printf("HAHA\n");
			strcpy(finalpath,exepath);
		}
		else
		{
			//printf("NANA\n");
			strcpy(finalpath,"~");
			strcat(finalpath,&exepath[j]);
		}
	}
	

	printf("Name ----- %s\n",name);
	printf("PID -----  %d\n",id);
	printf("Process Status -----  %c\n",pstatus);
	printf("Memory Allocated ----- %d kB\n",vm);
	printf("Executable Path ----- %s\n",finalpath);
}