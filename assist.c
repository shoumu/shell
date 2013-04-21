#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "functions.h"

char *env_path[10] = {0};	// 用来保存命令所存在的路径
char full_cmd[100] = {0};	// use in the exec
int goon = 0;

// 判断一个命令是否存在
int exists(char *name)
{
	int i = 0;
	//char cmd_buff[100] = {0};
	full_cmd[0] = '\0';


	if((name[0] == '.' || name[0] == '/') && access(name, F_OK) == 0)
	{
		return 1;	// 表示能够访问
	}
	else
	{
		while(env_path[i] != NULL)
		{
			strcpy(full_cmd, env_path[i]);
			strcat(full_cmd, name);
			if(access(full_cmd, F_OK) == 0)
			{
				return 1;
			}

			i ++;
	}
		return 0;
	}
}

// 将字符串转换为整形量，end表示最后一位的下一位
int str_to_int(char *str, int start, int end)
{
	int i,j;
	char chs[20];
	for(i = start,j = 0; i < end;i ++,j ++)
	{
		if(str[i] >= '0' && str[i] <= '9')
		{
			chs[j] = str[i];
		}
		else
			return -1;
	}
	chs[j] = '\0';

	return atoi(chs);
}

// 释放环境变量
void release()
{
	int i;
	for(i = 0; strlen(env_path[i]) > 0;i ++ )
	{
		free(env_path[i]);
	}
}

// 获取文件环境路径
void get_env_path()
{
	FILE * file;
	char buff[100];
	char temp[30];
	int i,j,env_index = 0,len,temp_len;
	if((file = fopen("user-sh.conf","r")) == NULL)
	{
		perror("initila environment failed\n");
		exit(1);
	}

	fgets(buff, 100, file);	// read the user configuration file
	len = strlen(buff);
	for(i = 0,j = 0;i < len; i ++)
	{
		if(buff[i] == ':') 
		{
			if(temp[j - 1] != '/')
			{
				temp[j ++] = '/';
			}
			temp[j] = '\0';

			j = 0;
			temp_len = strlen(temp);

			env_path[env_index] = (char *)malloc(sizeof(char) * (temp_len + 1));
			strcpy(env_path[env_index], temp);

			env_index ++;
		}
		else 
		{
			temp[j ++] = buff[i];
		}
	}
}

// set the value of gone 
void set_goon()
{
	goon = 1;
}