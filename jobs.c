#include "jobs.h"

static size_t job_cntr;

joblst_t* init_jobs(void)
{
  joblst_t* job_lst;
  
  job_cntr = 0;
  job_lst = (joblst_t*)malloc(sizeof(joblst_t));
  job_lst->head = NULL;
  job_lst->tail = NULL;
}

void add_job(joblst_t* job_lst, pid_t pid, char* cmd)
{
  job_t* new_job;

  new_job = (job_t*)malloc(sizeof(job_t));
  new_job->pid = pid;
  new_job->jid = ++job_cntr;
  new_job->stat = RUNNING;
  new_job->next = NULL;
  new_job->cmd = (char*)malloc(strlen(cmd) + 1);
  strcpy(new_job->cmd, cmd);

  if (!job_lst->head)
    job_lst->head = job_lst->tail = new_job;
  else
  {
    job_lst->tail->next = new_job;
    job_lst->tail = new_job;
  }

}

void remove_job(joblst_t* job_lst, jid_t jid)
{
  job_t* ptr, *prev;

  prev = NULL;
  ptr = job_lst->head;
  while (ptr)
  {
    if (ptr->jid == jid)
    {
      if (job_lst->head == job_lst->tail)
	job_lst->head = job_lst->tail = NULL;
      else if (ptr == job_lst->head)
	job_lst->head = job_lst->head->next;
      else if (ptr == job_lst->tail)
      {
	prev->next = NULL;
	job_lst->tail = prev;
      }
      else
	prev->next = ptr->next;

      free(ptr->cmd);
      free(ptr);
      return;
    }
    prev = ptr;
    ptr = ptr->next;
  }  
}

void remove_job_p(joblst_t* job_lst, pid_t pid)
{
  job_t* ptr, *prev;

  prev = NULL;
  ptr = job_lst->head;
  while (ptr)
  {
    if (ptr->pid == pid)
    {
      if (job_lst->head == job_lst->tail)
	job_lst->head = job_lst->tail = NULL;
      else if (ptr == job_lst->head)
	job_lst->head = job_lst->head->next;
      else if (ptr == job_lst->tail)
      {
	prev->next = NULL;
	job_lst->tail = prev;
      }
      else
	prev->next = ptr->next;

      free(ptr->cmd);
      free(ptr);
      return;
    }
    prev = ptr;
    ptr = ptr->next;
  }  
}

void show_jobs(joblst_t* job_lst)
{
  job_t* ptr;
  const char* status;

  ptr = job_lst->head;
  while (ptr)
  {
    status = stat_str(ptr->stat);
    printf("[%zu] %s %s\n", ptr->jid, status, ptr->cmd);
    ptr = ptr->next;
  }
}

void del_jobs(joblst_t* job_lst)
{
  job_t* ptr;

  ptr = job_lst->head;
  while (ptr)
  {
    free(ptr->cmd);
    free(ptr);
    ptr = ptr->next;
  }
  free(job_lst);
}

const char* stat_str(status_t stat)
{
  static const char* running = "RUNNING";
  static const char* stopped = "STOPPED";

  switch (stat)
  {
  case RUNNING:
    return running;
  case STOPPED:
    return stopped;
  default:
    return NULL;
  }
}
