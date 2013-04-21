#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#include "datastruct.h"
#include "functions.h"

extern command* parsed_command;
extern char full_cmd[100];

pid_t fg_pid; // the number of the fore ground process
extern int goon;

/*
 * do all the initialize work
 */
void init()
{
	read_history();	// history
	get_env_path();	// initialize the command path

	// register signals
	struct sigaction action;
	action.sa_sigaction = rm_job;
	sigfillset(&action.sa_mask);
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGCHLD, &action, NULL);

	signal(SIGTSTP, ctrl_z);
}

/*
 * about the inner command
 *  
 * exit, cd, jobs, fg, bg
 */
void exec_inner_cmd()
{
	if(parsed_command == NULL)
	{
		return;
	}

	if(strcmp(parsed_command->sim_cmd->args[0],"exit") == 0)
	{
		// handle history
		add_history();
		dump_history();
		exit(0);
	}
	else if(strcmp(parsed_command->sim_cmd->args[0],"cd") == 0)
	{
		if(parsed_command->sim_cmd->args[1] != NULL)
		{
			if(chdir(parsed_command->sim_cmd->args[1]) == -1)
			{
				printf("No such directory\n");
			}
		}
	}

	else if(strcmp(parsed_command->sim_cmd->args[0],"history") == 0)
	{
		//printf("%s\n", "input this command");
		print_history();
	}
	else if(strcmp(parsed_command->sim_cmd->args[0],"jobs") == 0)
	{
		print_jobs();
	}
	else if(strcmp(parsed_command->sim_cmd->args[0],"bg") == 0)
	{
		char * temp = parsed_command->sim_cmd->args[1];
		exec_bg(str_to_int(temp, 0, strlen(temp)));
	}
	else if(strcmp(parsed_command->sim_cmd->args[0],"fg") == 0)
	{
		char * temp = parsed_command->sim_cmd->args[1];
		exec_fg(str_to_int(temp, 0, strlen(temp)));
	}
	else
	{
		if(!parsed_command->sim_cmd->next)
		{
			exec_single_outer_cmd();
		}
	}
}

// execute single outer command
// not thought about the pipe command
void exec_single_outer_cmd()
{
	pid_t pid;
	int redirect_in, redirect_out;
	simple_command * sim_cmd = parsed_command->sim_cmd;
	job *cur_job = NULL;

	int test;

	if(exists(sim_cmd->args[0]))
	{
		if((pid = fork()) < 0)
		{
			perror("fork failed");
		}

		if(pid == 0) // child process
		{
			// input redirect
			if(sim_cmd->input != NULL)
			{
				if(redirect_in = open(sim_cmd->input, O_RDONLY) == -1)
				{
					printf("can't open file: %s\n", sim_cmd->input);
					return ;
				}

				// redirect
				dup2(redirect_in, 0);
			}

			// output redirect
			if(sim_cmd->output != NULL)
			{
				if(redirect_out = open(sim_cmd->output, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR) == -1)
				{
					printf("can't open file: %s\n", sim_cmd->output);
					return;
				}
			}

			if(parsed_command->is_back)
			{
				signal(SIGUSR1, set_goon);

				while(goon == 0)
				{
					;
				}
				goon = 0;

				//cur_job = add_jobs(getpid());
				printf("[%d]\t%s\t\t\t%s\n", getpid(), RUNNING, get_original_cmd(parsed_command));
				kill(getppid(), SIGUSR1);

			}

			// execute
			// there must use the full path of a comd ,so use a variable full_cmd
			if(execv(full_cmd,sim_cmd->args) < 0)
			{
				perror("Execv failed");
				return;
			}

		}
		else		// parent
		{
			if(parsed_command->is_back)
			{
				fg_pid = 0;
				add_job(pid);
				kill(pid, SIGUSR1);  // user defined sig

				// 
				signal(SIGUSR1, set_goon);
				while(goon == 0)
					;
				goon = 0;
			}
			else
			{
				fg_pid = pid;
				waitpid(pid, NULL, 0);
			}	
		}
	}
	else
	{
		printf("Command not found\n");
		return;
	}
}

// execute
void execute()
{
	add_history();
	exec_inner_cmd();

	printf("%s\n", "out of exec command");

	//print_jobs();

	parsed_clean();
}