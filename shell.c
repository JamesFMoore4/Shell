#include "wrappers.h"
#include "jobs.h"

#define MAXLINE 8192
#define MAXARGS 128

void eval(char* cmd);
int parseline(char* cmd, char** argv);
int builtin(char** argv);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

joblst_t* job_lst;
extern char** environ;
volatile sig_atomic_t pid;
volatile sig_atomic_t fgpid;
char fgcmd[MAXLINE];

int main(void)
{
  char cmd[MAXLINE];

  Signal(SIGCHLD, sigchld_handler);
  Signal(SIGTSTP, sigtstp_handler);
  Signal(SIGINT, sigint_handler);
  
  job_lst = init_jobs();
  fgpid = 0;
  pid = 0;
  
  while (1)
  {
    printf("shell> ");
    fgets(cmd, MAXLINE, stdin);
    if (feof(stdin))
      exit(0);
    eval(cmd);
  }

  return 0;
}

void eval(char* cmd)
{
  char* argv[MAXARGS];
  char cmd_args[MAXLINE];
  int bg, status;
  sigset_t mask_all, mask_one, prev;
  
  strcpy(cmd_args, cmd);
  bg = parseline(cmd_args, argv);
  if (!argv[0])
    return;

  Sigfillset(&mask_all);
  Sigemptyset(&mask_one);
  Sigaddset(&mask_one, SIGCHLD);

  if (!builtin(argv))
  {
    Sigprocmask(SIG_BLOCK, &mask_one, &prev);
    if (!(pid = Fork()))
    {
      Sigprocmask(SIG_SETMASK, &prev, NULL);
      Execve(argv[0], argv, environ);
    }

    Setpgid(pid, 0);
    
    if (!bg)
    {
      strcpy(fgcmd, cmd);
      fgpid = pid;
      pid = 0;
      Sigprocmask(SIG_SETMASK, &prev, NULL);
      while(!pid);
      fgpid = pid = 0;
    }
    else
    {
      printf("%d %s", pid, cmd);
      Sigprocmask(SIG_BLOCK, &mask_all, NULL);
      add_job(job_lst, pid, cmd);
      Sigprocmask(SIG_SETMASK, &prev, NULL);
    }
      
  }
}

//TODO: Add more built-in commands
int builtin(char** argv)
{
  if (!strcmp(argv[0], "quit"))
    exit(0);
  if (!strcmp(argv[0], "&"))
    return 1;
  return 0;
}

int parseline(char* cmd, char** argv)
{
  char* delim;
  int argc;
  int bg;

  cmd[strlen(cmd)-1] = ' ';
  while (*cmd && (*cmd == ' '))
    cmd++;

  argc = 0;
  while (delim = strchr(cmd, ' '))
  {
    argv[argc++] = cmd;
    *delim = '\0';
    cmd = delim + 1;
    while (*cmd && (*cmd == ' '))
      cmd++;
  }
  argv[argc] = NULL;

  if (!argc)
    return 1;

  if (bg = (*argv[argc-1] == '&'))
    argv[--argc] = NULL;

  return bg;
}

void sigchld_handler(int sig)
{
  sigset_t mask, prev;
  int status, olderrno;
  pid_t ret_pid;

  olderrno = errno;
  Sigfillset(&mask);
  
  while ((ret_pid = waitpid(-1, &status, WUNTRACED | WNOHANG)) > 0)
  {
    if (ret_pid == fgpid)
      pid = ret_pid;
    
    if (WIFSIGNALED(status))
      printf("\nProcess %d terminated by signal %d\n", ret_pid, WTERMSIG(status));
    
    Sigprocmask(SIG_BLOCK, &mask, &prev);
    remove_job_p(job_lst, ret_pid);
    Sigprocmask(SIG_SETMASK, &prev, NULL);
  }
  
  errno = olderrno;
}

void sigtstp_handler(int sig)
{
  sigset_t mask, prev;

  Sigfillset(&mask);
  
  if (fgpid)
  {
    Kill(-fgpid, SIGTSTP);
    Sigprocmask(SIG_BLOCK, &mask, &prev);
    add_job(job_lst, fgpid, fgcmd);
    Sigprocmask(SIG_SETMASK, &prev, NULL);
  }
  printf("\n");
  pid = fgpid;
}

void sigint_handler(int sig)
{
  if (fgpid)
    Kill(-fgpid, SIGINT);
  printf("\n");
  pid = fgpid;
}
