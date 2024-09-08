#ifndef SYS_WRAPPER
#define SYS_WRAPPER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

typedef void handler_t(int);

// System call wrapper functions for error handling
pid_t Fork(void);
pid_t Waitpid(pid_t pid, int* statusp, int options);
pid_t Wait(int* statusp);
int Execve(const char* filename, char* const argv[], char*
	   const envp[]);
pid_t Getpgrp(void);
int Setpgid(pid_t pid, pid_t pgid);

int Kill(pid_t pid, int sig);
unsigned int Alarm(unsigned int secs);
handler_t* Signal(int signum, handler_t* handler);
int Sigprocmask(int how, const sigset_t* set, sigset_t* oldset);
int Sigemptyset(sigset_t* set);
int Sigfillset(sigset_t* set);
int Sigaddset(sigset_t* set, int signum);
int Sigdelset(sigset_t* set, int signum);
int Sigismember(const sigset_t* set, int signum);

// Error messaging functions
void unix_error(char* msg);

#endif
