#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <valgrind/memcheck.h>

int	execve(const char *path, char *const argv[], char *const envp[])
{
	int	(*_execve)(const char *path, char *const argv[], char *const envp[]);

	VALGRIND_DO_LEAK_CHECK;
	_execve = dlsym(RTLD_NEXT, "execve");
	if (!_execve)
		abort();
	return (_execve(path, argv, envp));
}
