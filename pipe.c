#include "header_prompt.c"

void parse_for_pipe(char * command)
{
	int i=0,j=0,k=0,lpidx=0,p=0;
	int length=strlen(command);
	char *pipe_cmds[100];
	pipe_cmds[0]=(char *)malloc(1000*sizeof(char));
	for(i=0;i<1000;i++)
		pipe_cmds[0][i]=0;
	for(i=0;i<length;i++)
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
		if(command[i]=='|' && quote==0)
		{
			piping=1;
			for(k=lpidx;k<i;k++)
			{
				pipe_cmds[j][k-lpidx]=command[k];
			}
			lpidx=i+1;
			j++;
			pipe_cmds[j]=(char *)malloc(1000*sizeof(char));
			for(p=0;p<1000;p++)
				pipe_cmds[j][p]=0;
		}
	}
	for(k=lpidx;k<i;k++)
	{
		pipe_cmds[j][k-lpidx]=command[k];
	}
	j++;
	num_pipe=j;
	for(i=0;i<num_pipe-1;i++)
	{
		pipe(my_pipe[i]);
	}
	if(piping==1)
	{
		for(i=0;i<num_pipe;i++)
		{
			run_pipe(pipe_cmds[i],i);
		}
	}
	else
	{
		parse_input(command);
		if(fileinput==1)
		{
			ifd2=dup(0);
			ifd=open(ifname,O_RDONLY);
			if(ifd<0)
			{
				perror("Error in Inputfile");
				exit(1);
			}
			dup2(ifd,0);
			close(ifd);
		}
		if(fileoutput==1)
		{
			ofd2=dup(1);
			printf("outfile %s\n",ofname );
			if(foutappend==1)
			{
				ofd=open(ofname,O_WRONLY | O_APPEND | O_CREAT , 0644);
			}
			else
			{
				ofd=open(ofname,O_WRONLY | O_TRUNC | O_CREAT , 0644);
			}
			if(ofd<0)
			{
				perror("Error in Output file");
				exit(1);
			}
			printf("ofd %d ofname %s\n",ofd,ofname );
			dup2(ofd,1);
			close(ofd);
		}
		execIN(input2);
		if(fileinput==1)
		{
			dup2(ifd2,0);
			close(ifd2);
		}
		if(fileoutput==1)
		{
			dup2(ofd2,1);
			close(ofd2);
		}
	}
}

void parse_input(char * input)
{
	int i,j;
	fileinput=fileoutput=0;
	int length=strlen(input);
	for(i=0;i<length;i++)
	{
		if(input[i]=='&' && quote==0)
		{
			background=1;
			input[i]=' ';
			continue;
		}
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

		if(input[i]=='<' && quote==0)
		{
			fileinput=1;
			input[i]=' ';
			int j=i+1;
			while(input[j]==' ' || input[j]=='\t')
			{
				j++;
			}
			int k=0;
			while(input[j]!=' ' && input[j]!='\t' && input[j]!=0 && input[j]!='\n' && input[i]<256 && input[i]>0)
			{
				ifname[k]=input[j];
				input[j]=' ';
				j++;
				k++;
			}
			continue;
		}

		if(input[i]=='>' && quote==0)
		{
			fileoutput=1;
			input[i]=' ';
			if(input[i+1]=='>')
			{
				foutappend=1;
				i++;
			}
			input[i]=' ';
			int j=i+1;
			while(input[j]==' ' || input[j]=='\t')
			{
				j++;
			}
			int k=0;
			while(input[j]!=' ' && input[j]!='\t' && input[j]!=0 && input[j]!='\n' && input[i]<256 && input[i]>0)
			{
				ofname[k]=input[j];
				input[j]=' ';
				j++;
				k++;
			}
			input[j]=0;
			continue;
		}
	}
	j=0;int space=0;

	for ( i = 0; input[i]!=0 && input[i]!='\n' && i<length ; ++i)
	{
		if(input[i]==' ' && space==1)
		{
			continue;
		}
		else if (input[i]==' ' && input[i]<256 && input[i]>0 && space==0)
		{
			input2[j]=' ';
			j++;
		}
		else if (input[i]<256 && input[i]>0)
		{
			input2[j]=input[i];
			j++;
			space=0;
		}
	}
}

void run_pipe(char * input,int pos)
{
	int i=0;
	pid=fork();
	if(pid<0)
	{
		perror("Error in forking");
	}
	else if(pid==0)
	{
		parse_input(input);
		ifd2=dup(0);
		ofd2=dup(1);
		
		if(fileoutput==1)
		{
			if(foutappend==1)
			{
				ofd=open(ofname,O_WRONLY | O_APPEND | O_CREAT , 0644);
			}
			else
			{
				ofd=open(ofname,O_WRONLY | O_TRUNC | O_CREAT , 0644);
			}
			if(ofd<0)
			{
				perror("Error in opening OutFile");
			}
			dup2(ofd,1);
			close(ofd);
		}
		else if(pos<num_pipe-1)
		{
			if(dup2(my_pipe[pos][1],1)<0)
				perror("Error in dup2");
		}

		if(fileinput==1)
		{
			ifd=open(ifname,O_RDONLY);
			if(ifd<0)
			{
				perror("Error in opening InFile");
				exit(1);
			}
			dup2(ifd,0);
			close(ifd);
		}
		else if(pos>0)
		{
			if(dup2(my_pipe[pos-1][0],0) < 0)
				perror("Error in dup2");
		}
		
		for(i=0;i<num_pipe-1;i++)
		{
			close(my_pipe[i][0]);
			close(my_pipe[i][1]);
		}
		execIN(input2);
		exit(0);
	}
	else
	{
		wait(NULL);
		if(pos==0)
			close(my_pipe[pos][1]);
		else if (pos<num_pipe-1)
		{
			close(my_pipe[pos-1][0]);
			close(my_pipe[pos][1]);
		}
		else
		{
			close(my_pipe[pos-1][0]);
		}
	}
}