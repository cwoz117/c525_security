#include <stdio.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[]){
	struct user_regs_struct reg;
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
	reg.rip += 281;
	ptrace(PTRACE_SETREGS, child, NULL, &reg);
	
	// Let the program continue
	ptrace(PTRACE_DETACH, child, NULL, NULL);
}
