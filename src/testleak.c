#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
int	main(int argc, char *argv[], char **envp)
{
	char	*path;
	char	*test;
	pid_t	pid;

	path = strdup("/home/irychkov/Desktop/minishell/src/a.out");
	test = strdup("test");
	pid = fork();
	if (pid == 0)
	{
		char *example;
		example = strdup("g");
		execve(path, argv, envp);
	}

	printf("%s\n", path);
	free(path);
	free(test);
	return (0);
}