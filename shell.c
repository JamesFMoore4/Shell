#include "wrappers.h"
#include "jobs.h"

#define MAXLINE 8192
#define MAXARGS 128

void eval(char* cmd);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

joblst_t* job_lst;

int main(void)
{
  char cmd[MAXLINE];

  Signal(SIGCHLD, sigchld_handler);
  Signal(SIGTSTP, sigtstp_handler);
  Signal(SIGINT, sigint_handler);
  init_jobs(job_lst);
  
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
  printf("%s", cmd);
}

void sigchld_handler(int sig)
{
  
}

void sigtstp_handler(int sig)
{
  
}

void sigint_handler(int sig)
{
  
}
