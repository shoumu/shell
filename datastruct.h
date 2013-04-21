#ifndef __DATASTRUCT_H
#define __DATASTRUCT_H

#include <sys/types.h>

#define HISTORY_LEN 500
#define CMD_LEN	50

// status about a job
#define RUNNING "running"
#define STOPPED "stopped"
#define DONE	"done"

/*
 * a struct of a command
 * @author shoumu
 */
typedef struct simple_command
{
	char ** args;
	char *input;
	char *output;

	struct simple_command *next;		// use in the pipe command
}simple_command;

typedef struct command
{
	int is_back;
	simple_command *sim_cmd;
}command;

/*
 * a struct of history
 * use a loop structure to store the history command
 * if end > HISTORY_LEN then end = end % HISTORY_LEN
 */
typedef struct history
{
	int start;
	int end;
	char cmds[HISTORY_LEN][CMD_LEN];
}history;

/*
 * struct that use to save a process's status
 */
typedef struct job
{
	pid_t pid;					// process number
	char cmd[CMD_LEN];
	char status[10];
	struct job *next;
}job;

#endif
