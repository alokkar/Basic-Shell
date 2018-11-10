shell : main.c header_prompt.o pinfo.o builtin_commands.o ls.o executables.o pipe.o env.o jobs.o
	gcc -o shell main.c

pinfo.o : pinfo.c
	gcc -c pinfo.c

builtin_commands.o : builtin_commands.c
	gcc -c builtin_commands.c

ls.o : ls.c
	gcc -c ls.c

executables.o : executables.c
	gcc -c executables.c

header_prompt.o : header_prompt.c
	gcc -c header_prompt.c

pipe.o : pipe.c
	gcc -c pipe.c

jobs.o : jobs.c
	gcc -c jobs.c

env.o : env.c
	gcc -c env.c

clean :
	rm shell header_prompt.o pinfo.o builtin_commands.o ls.o executables.o pipe.o
