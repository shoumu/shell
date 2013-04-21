#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "datastruct.h"

extern command* parsed_command;
job * head = NULL;	// 这里作为job头
int ingnore = 0;
extern pid_t fg_pid;


job* add_job(pid_t pid)
{

	//printf("job pid: %d\n",pid);

	job *njob = (job *)malloc(sizeof(job));
	job *current = NULL, *last = NULL;

	// initialize
	njob->pid = pid;

	strcpy(njob->cmd,get_original_cmd(parsed_command));
	strcpy(njob->status, RUNNING);
	njob->next = NULL;
	
	// 插入job
	if(head == NULL)
	{
		head = njob;
	}
	else
	{
		current = head;
		while(current != NULL && current->pid < pid)
		{
			last = current;
			current = current-> next;
		}
		last->next = njob;
		njob->next = current;
	}
	return njob;
}

void rm_job(int sig, siginfo_t* sip, void *noused)
{
	job *cur = NULL,*last = NULL;
	pid_t pid;

	if(ingnore)
	{
		ingnore = 0;
	}

	pid = sip->si_pid;

	// find 
	cur = head;

	while(cur != NULL && cur->pid < pid)
	{
		last = cur;
		cur = cur->next;
	}

	if(cur == NULL)
	{
		return; 
	}

	// remove
	if(cur == head)
	{
		head = cur->next;
	}
	else 
	{
		last->next = cur->next;
	}

	free(cur);
}

// ctrl + z
void ctrl_z()
{
	job *cur = NULL;

	printf("fg_pid: %d\n",fg_pid);

	//printf("%d\n",fg_pid);
	if(fg_pid == 0) // no foreground command
	{
		return ;
	}

	ingnore = 1;	// SIGCHLD is produced by ctrl_z, can't move the job
	cur = head;

	while(cur != NULL && (cur->pid != fg_pid))
	{
		cur = cur->next;
	}

	//printf("%s\n", "come there");

	if(cur == NULL)
	{
		//printf("%s\n", "come there");
		cur = add_job(fg_pid);		// there doesn't have the job
		//printf("%s\n", "job is NULL 2");
	}

	if(cur == NULL)
	{
		printf("%s\n", "cur is still NULL");
	}

	strcpy(cur->status, STOPPED);	// change into stop
	printf("[%d]\t%s\t\t\t%s\n", cur->pid,cur->status,cur->cmd);

	fg_pid = 0;		// no fore ground command
}

void exec_fg(pid_t pid)
{
	job *cur = NULL;
	int i;

	ingnore = 1;	// 
	
	cur = head;
	while(cur != NULL && cur->pid != pid)
	{
		cur = cur->next;
	}

	if(cur == NULL)
	{
		printf("fg: %d: no such job\n", pid);
		return ;
	}

	fg_pid = cur->pid;
	strcpy(cur->status, RUNNING);

	//signal(SIGTSOP, ctrl_z);
	i = strlen(cur->cmd);
	if(i > 0 && (cur->cmd[i-1] == '&'))
	{
		cur->cmd[i-1] = '\0';
	}

	printf("fg_pid: %d\n", fg_pid);

	printf("%s\n", cur->cmd);
	kill(cur->pid, SIGCONT);
	waitpid(fg_pid, NULL, 0);		// wait for the fore ground command

}

void exec_bg(pid_t pid)
{
	job *cur = NULL;
	int len;

	cur = head;
	while(cur != NULL && cur-> pid != pid)
	{
		cur = cur->next;		// find the command
	}

	if(cur == NULL)
	{
		printf("bg: %d: no such job", pid);
		return;
	}
	if(strcmp(cur->status, RUNNING) == 0)
	{
		printf("bg: job %d already in background\n", pid);
	}
	strcpy(cur->status, RUNNING);
	len = strlen(cur->cmd);
	cur->cmd[len ++] = ' ';
	cur->cmd[len ++] = '&';
	cur->cmd[len] = '\0';
	printf("[%d]\t%s\n", pid, cur->cmd);

	kill(cur-> pid, SIGCONT);
}

void print_jobs()
{
	job *cur = head;
	while(cur)
	{
		printf("[%d]\t%s\t\t\t%s\n", cur->pid, cur->status, cur->cmd);
		cur = cur->next;
	}
}