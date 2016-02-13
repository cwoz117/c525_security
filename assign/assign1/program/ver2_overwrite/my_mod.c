#include <stdio.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[]){
	struct user_regs_struct reg;
	
	/*
	int payload[] = {0xbf, 0xe0, 0x05, 0x40, 0x00, 	// "printf("Hello World!\n");
			 0xe8, 0xcc, 0xfe, 0xff, 0xff}; */

	int payload[] = {0xbf, 0x7c, 0x0c, 0x40, 0x00,	// From helloworld-x86_64
			 0xe8, 0x6d, 0xfb, 0xff, 0xff};	// main + 281

	char *arg[] = {"../helloworld-x86_64", "file1", "file2", "file3", "file4"};
	char *env[] = {NULL};

	pid_t child = fork();
	ptrace(PTRACE_SETOPTIONS, child, NULL, PTRACE_O_TRACEEXEC);
	if (child == 0){
		ptrace(PTRACE_TRACEME, NULL, NULL, NULL);
		execve("helloworld-x86_64", arg, env);
	}

	// Wait for tracee to SIGSTOP
	waitpid(child, NULL, WSTOPPED);

	// Get registers for the program counter
	ptrace(PTRACE_GETREGS, child, NULL, &reg);
	
	// Overwrite text data with payload
	for (int i = 0; i < 10; i++){
		ptrace(PTRACE_POKEDATA, child, payload[i], &reg.rip); 
		reg.rip += 1;
	}	
	// Let the program continue
	ptrace(PTRACE_DETACH, child, NULL, NULL);
}
