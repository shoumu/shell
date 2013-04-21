/*
 * a header file that include some of the functions used in this program
 */

#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

#include "datastruct.h"
#include <signal.h>

/*
 * command.c
 * 
 * functions used to parse the command
 */
void make_command(int,char *);
void push_command();
void print_test();
void before_parse();
void after_parse();
void parsed_clean();

/*
 * history.c
 *
 * functions about the history command
 */
void add_history();
void dump_history();
void read_history();
void put_into_history(char *);
void print_history();
char* get_original_cmd(command*);	// 获取完整的命令

/*
 * jobs.c
 *
 * about the jobs
 */
job *add_job(pid_t);
void rm_job(int sig, siginfo_t *sip, void *noused);
void ctrl_z();
void exec_fg(pid_t pid);
void exec_bg(pid_t pid);
void print_jobs();
/*
 * assist.c
 *
 * 辅助函数
 */
int exists(char *);
int str_to_int(char *str, int start, int end);
void release();
void get_env_path();
void set_goon();

/*
 * execute.c
 * some functions is used when execute
 */
void init();
void exec_inner_cmd();
void exec_single_outer_cmd();
void execute();

#endif
