#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

// execute the programs in argv[1], ..., argv[argc - 1] as new processes
// pipe should connect argv[1]'s standard output to argc[2]'s standard input (if there are at least two processes)
int main(int argc, char *argv[])
{
	// argc: number of arguments passed
	// argv: array of pointers to the arguments passed

	// if no programs are passed
	if (argc == 1) {
		// program should exit with EINVAL
		exit(EINVAL);
	}

	int i;

	for (i = 1; i < argc - 1; i++) {
		// file descriptors, creates a pipe with two ends: 
		// fd[0]: input (read)
		// fd[1]: output (write)
		int fd[2];
		pipe(fd); 

		// in the parent process, fork() returns the pid of the newly created child process
		// in the child process, fork() returns 0
		pid_t child = fork();
		if (child == -1) {
			exit(errno);
		}

		// execute the child process 
		else if (child == 0) {
			// dup2(int oldfd, int newfd)
			// redirect child process's stdout (1) to the pipe's write (fd[1])
			// after dup, any write operations to stdout will write to pipe
			dup2(fd[1], 1);
			execlp(argv[i], argv[i], NULL);
			// perror("execlp");
			exit(errno);
		}

		int status;
		// parent process waits for child process to complete
		// pid_t waitpid(pid_t pid, int *status, int options); 
		waitpid(child, &status, 0);

		// redirect parent process's stdin (0) to the pipe's read (fd[0])
		// after dup, any read operations from stdin will read from the pipe
		dup2(fd[0], 0);

		// close the write end of the pipe in the parent process
		close(fd[1]);  
	}

	// last argument, don't create pipe
	execlp(argv[i], argv[i], NULL);
	// perror("execlp");
	exit(errno);

	return 0;
}
