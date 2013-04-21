#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "datastruct.h"
#include "functions.h"

extern command* parsed_command;
history history_cmds;  // 用来保存历史命令

// 添加一条命令到histroy中
void add_history()
{
	char * temp = get_original_cmd(parsed_command);
	// test
	//printf("original cmd: %s\n",temp);

	put_into_history(temp);

	free(temp);
}

// 将所有的命令写入到文件中
void dump_history()
{
	FILE* file;
	if(history_cmds.start < 0)	// 没有命令的时候
		return ;
	
	if((file = fopen("history.dat","w")) == NULL)
	{
		printf("write history commands error\n");
		exit(0);
	}

	// test
	//printf("start: %d\n",history_cmds.start);

	while(history_cmds.start != history_cmds.end)
	{
		fprintf(file,"%s\n",history_cmds.cmds[history_cmds.start]);
		history_cmds.start = (history_cmds.start + 1) % HISTORY_LEN;
	}
	
	// finish
	fclose(file);
}

// 读入保存到文件中的histroy命令
void read_history()
{
	char buff[100] = {0};
	FILE* file;
	int i;
	history_cmds.start = -1;
	history_cmds.end = 0;
	// initial cmds
	for(i = 0;i < HISTORY_LEN;i ++)
	{
		history_cmds.cmds[i][0] = '\0';
	}

	//printf("there\n");
	
	// 读取
	if((file = fopen("history.dat","r")) == NULL)
	{
		printf("write history commands error\n");
		exit(0);
	}
	while(1)
	{
		fgets(buff, 100, file);
		if(strlen(buff) == 0)		// this is important
			return;
		else
			put_into_history(buff);
		buff[0] = '\0';
	}
}

void put_into_history(char *temp)
{
	// 放入到history中
	// 在这个地方，在history中start指向开始的第一个命令
	// end指向最后一个命令的下一个命令
	if(history_cmds.end == history_cmds.start)
	{
		history_cmds.start = (history_cmds.start + 1) % HISTORY_LEN;
	}
	// 第一次使用
	if(history_cmds.start == -1)	
	{
		history_cmds.start = 0;
	}

	strcpy(history_cmds.cmds[history_cmds.end ++],temp);

	if(history_cmds.end >= HISTORY_LEN)
	{
		history_cmds.end %= HISTORY_LEN;
	}
}


// print the history command
void print_history()
{
	// just print 
	// cann't change any data
	int s = history_cmds.start;
	int e = history_cmds.end;
	if(s == -1)	// no history command
	{
		printf("Haven't input any command\n");
		return;
	}

	// print the history commands
	while(s != e)
	{
		printf("%s\n",history_cmds.cmds[s]);
		s = (s + 1) % HISTORY_LEN;
	}
	
}

// get original commands
char* get_original_cmd(command *cmd)
{
	int i;
	
	char *temp = (char *)malloc(sizeof(char) * 100);
	simple_command * tcmd = cmd->sim_cmd;
	
	// 写入命令
	while(tcmd != NULL)
	{
		for(i = 0;tcmd->args[i];i ++)
		{
			strcat(temp, tcmd->args[i]);
			strcat(temp, " ");
		}
		if(tcmd->input != NULL)
		{
			strcat(temp, "< ");
			strcat(temp, tcmd->input);
			strcat(temp, " ");
		}
		if(tcmd->output != NULL)
		{
			strcat(temp, "> ");
			strcat(temp, tcmd->output);
			strcat(temp, " ");
		}
		if(tcmd->next != NULL)
		{
			strcat(temp, "| ");
		}

		tcmd = tcmd->next;
	}
	if(cmd->is_back)
	{
		strcat(temp, "&");
	}
	return temp;
}