#ifndef JOB_LST
#define JOB_LST

#include "wrappers.h"

typedef enum {RUNNING, STOPPED} status_t;

typedef size_t jid_t;

typedef struct job_t
{
  pid_t pid;
  jid_t jid;
  status_t stat;
  char* cmd;
  struct job_t* next;
} job_t;

typedef struct
{
  job_t* head;
  job_t* tail;
} joblst_t;

joblst_t* init_jobs(void);
void add_job(joblst_t* job_lst, pid_t pid, char* cmd);
void remove_job(joblst_t* job_lst, jid_t jid);
void remove_job_p(joblst_t* job_lst, pid_t pid);
void show_jobs(joblst_t* job_lst);
void del_jobs(joblst_t* job_lst);
const char* stat_str(status_t stat);


#endif
