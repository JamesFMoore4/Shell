#include "wrappers.h"

pid_t Fork(void)
{
  pid_t pid;
  if ((pid = fork()) == -1)
    unix_error("fork error");
  return pid;
}

pid_t Waitpid(pid_t pid, int* statusp, int options)
{
  pid_t ret;
  if ((ret = waitpid(pid, statusp, options)) == -1)
    unix_error("waitpid error");
  return ret;
}

pid_t Wait(int* statusp)
{
  pid_t pid;
  if ((pid = wait(statusp)) == -1)
    unix_error("wait error");
  return pid;
}

int Execve(const char* filename, char* const argv[], char*
	   const envp[])
{
  int ret;
  if ((ret = execve(filename, argv, envp)) == -1)
    unix_error("execve error");
  return ret;
}

pid_t Getpgrp(void)
{
  return getpgrp();
}

int Setpgid(pid_t pid, pid_t pgid)
{
  int ret;
  if ((ret = setpgid(pid, pgid)) == -1)
    unix_error("setpgid error");
  return ret;
}

int Kill(pid_t pid, int sig)
{
  int ret;
  if ((ret = kill(pid, sig)) == -1)
    unix_error("kill error");
  return ret;
}

unsigned int Alarm(unsigned int secs)
{
  return alarm(secs);
}

handler_t* Signal(int signum, handler_t* handler)
{
  struct sigaction action, old_action;

  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_RESTART;

  if (sigaction(signum, &action, &old_action) == -1)
    unix_error("signal error");
  
  return old_action.sa_handler;
}

int Sigprocmask(int how, const sigset_t* set, sigset_t* oldset)
{
  int ret;
  if ((ret = sigprocmask(how, set, oldset)) == -1)
    unix_error("sigprocmask error");
  return ret;
}

int Sigemptyset(sigset_t* set)
{
  int ret;
  if ((ret = sigemptyset(set)) == -1)
    unix_error("sigemptyset error");
  return ret;
}

int Sigfillset(sigset_t* set)
{
  int ret;
  if ((ret = sigfillset(set)) == -1)
    unix_error("sigfillset error");
  return ret;
}

int Sigaddset(sigset_t* set, int signum)
{
  int ret;
  if ((ret = sigaddset(set, signum)) == -1)
    unix_error("sigaddset error");
  return ret;
}

int Sigdelset(sigset_t* set, int signum)
{
  int ret;
  if ((ret = sigdelset(set, signum)) == -1)
    unix_error("sigdelset error");
  return ret;
}

int Sigismember(const sigset_t* set, int signum)
{
  int ret;
  if ((ret = sigismember(set, signum)) == -1)
    unix_error("sigismember error");
  return ret;
}

void unix_error(char* msg)
{
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(0);
}
