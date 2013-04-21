#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastruct.h"
#include "functions.h"


command* parsed_command = NULL;
char cmd_buff[10][20] = {0};		// command name with his argument
int arg_index = 1;				// how many args in a coomand,it start at 1
char input_file[20] = {0};		// input name
char output_file[20] = {0};		// output file name
int is_back = 0;



void make_command(int type,char *str)
{
	if(type == 0) // a back ground command
	{
		is_back = 1;
		printf("is_back: %d\n",is_back);
	}
	else if(type == 1)		// command args
	{
		strcpy(cmd_buff[arg_index ++],str);
	}
	else if(type == 2)		// input file name
	{
		strcpy(input_file,str);
	}
	else if(type == 3)		// output file name
	{
		strcpy(output_file,str);
	}
	else if(type == 4)		// command name
	{
		strcpy(cmd_buff[0],str);
	}
	//printf("type: %d  is_back: %d\n",type,is_back);
}

void push_command()
{
	//printf("-------------------------\n");
	int i,j;

	simple_command *cmd = (simple_command*)malloc(sizeof(simple_command));
	cmd->next = NULL;

	cmd->args = (char **)malloc(sizeof(char *) * arg_index);
	for(i = 0;i < arg_index;i ++)
	{
		//printf("arg_%d: %s\n",i,cmd_buff[i]);
		cmd-> args[i] = (char *)malloc(sizeof(char) * (strlen(cmd_buff[i]) + 1));
		strcpy(cmd->args[i],cmd_buff[i]);
	}

	if(strlen(input_file) != 0)
	{
		//printf("input: %s\n", input_file);
		cmd->input = (char *)malloc(sizeof(char) * (strlen(input_file) + 1));
		strcpy(cmd->input,input_file);
	}

	if(strlen(output_file) != 0)
	{
		//printf("output: %s\n", output_file);
		cmd->output = (char *)malloc(sizeof(char) * (strlen(output_file) + 1));
		strcpy(cmd->output,output_file);
	}

	//printf("---------------------------\n");
	// 将命令放入到里面去
	if(parsed_command->sim_cmd == NULL)
	{
		parsed_command->sim_cmd = cmd;
	}
	else
	{
		simple_command *temp = parsed_command->sim_cmd;
		while(temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = cmd;
	}

	//printf("arg_index: %d\n",arg_index);

	arg_index = 1;	// 将所有的值清空，提供给下面的命令使用，这里从1开始，0用来放命令名字
	input_file[0] = '\0';
	output_file[0] = '\0';
	
}

void print_test()
{
	int i,j;

	//printf("is_back_final: %d\n",is_back);
	simple_command * temp = parsed_command->sim_cmd;

	printf("+++++++++++++++++++++\n");
	printf("is_back: %d\n",parsed_command->is_back);
	while(temp != NULL)
	{
		printf("cmd_name: %s\n",temp->args[0]);
		for(i = 1;temp->args[i];i ++)
		{
			printf("cmd_args: %s\n",temp->args[i]);
		}
		printf("input_file: %s\n",temp->input);
		printf("output_file: %s\n",temp->output);
		temp = temp->next;

		printf("===========================\n");
	}
	printf("+++++++++++++++++++++\n\n\n\n");
}


void before_parse()
{
	parsed_command = (command *)malloc(sizeof(command));
	parsed_command->sim_cmd = NULL;
}

// 一次命令解析以后，将命令的数恢复为0
void after_parse()
{
	parsed_command->is_back = is_back;
	is_back = 0;
}

// 解析之后释放分配的空间
void parsed_clean()
{
	int i;
	simple_command *temp = parsed_command->sim_cmd;
	free(parsed_command);
	while(temp != NULL)
	{
		for(i = 0; temp->args[i]; i ++)
		{
			free(temp->args[i]);
		}
		free(temp->input);
		free(temp->output);
		temp = temp->next;
	}
}
