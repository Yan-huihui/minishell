#include "cmd.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "minishell.h"

void show_cmd_line(void)			//输出命令提示行
{
	char buff[1024] = {0};
	char cmd[1024] = {0};
	getcwd(buff, sizeof(buff));
	sprintf(cmd,"linux@ubuntu:%s$ ",buff);
	printf("%s",cmd);
}

int get_cmd(char *cmd)				//获取输入字符放入传入对应字符数组
{
	fgets(cmd, 1024, stdin);
	if(cmd[0] == '\n')
	{
		return -1;
	}
	cmd[strlen(cmd) - 1] = '\0';	//把最后的\n更改为\0
	return 0;
}

int do_cmd(char *cmd)
{
	int i;
	char *command[10] = {NULL};		
	char *p = strtok(cmd, " ");		//以空格截取字符串 用指针数组指向每个截取到的字符串首地址
	for(i = 0 ; i < 10 ; ++i)
	{
		command[i] = p;
		p = strtok(NULL, " ");

		if(NULL == p)
		{
			break;
		}
	}

	if(strcmp(command[0], "exit") == 0)		//输入第一个字符串为exit退出
	{
		return 1;
	}
	else if(strcmp(command[0], "touch") == 0)	//输入第一个字符串为touch 判断并执行 touch命令
	{
		if(NULL == command[1])
		{
			printf("Usage touch filename\n");
			return 0;
		}
		touch(command[1]);
	}
	else if(strcmp(command[0], "cat") == 0)		//输入第一个字符串为cat 判断并执行 cat命令
	{
		if(NULL == command[1])
		{
			printf("Usage cat filename\n");
			return 0;
		}
		cat(command[1]);
	}
	else if(strcmp(command[0], "cp") == 0)		//输入第一个字符串为cp 判断并执行 cp命令
	{
		if(NULL == command[1] || NULL == command[2])
		{
			printf("Usage cp filename\n");
			return 0;
		}
		cp(command[1], command[2]);
	}
	else if(strcmp(command[0], "cd") == 0)		//输入第一个字符串为cd 判断并执行 cd命令
	{
		if(NULL == command[1])
		{
			printf("Usage cd filename\n");
			return 0;
		}
		cd(command[1]);
	}
	else if(strcmp(command[0], "pwd") == 0)		//输入第一个字符串为pwd 执行pwd命令
	{
		pwd();
	}
	else if(strcmp(command[0], "mkdir") == 0)	//输入第一个字符串为mkdir 判断并执行mkdir命令
	{
		if(NULL == command[1])
		{
			printf("Usage mkdir filename\n");
			return 0;
		}
		do_mkdir(command[1]);
	}
	else if(strcmp(command[0], "rmdir") == 0)	//输入第一个字符串为rmdir 判断并执行 rmdir命令
	{
		if(NULL == command[1])
		{
			printf("Usage mkdir filename\n");
			return 0;
		}
		do_rmdir(command[1]);
	}
	else if(strcmp(command[0], "rm") == 0)		//输入第一个字符串为rm 判断并执行 rm命令
	{
		if(NULL == command[1])
		{
			printf("Usage mkdir filename\n");
			return 0;
		}
		rm(command[1]);
	}
	else if(strcmp(command[0], "ls") == 0)		//输入第一个字符串为ls 执行 ls命令
	{
		ls(command);							//可以成功调用情况下字符串个数不一 是否成功使用在函数内判断
	}
	else if(strcmp(command[0], "ln") == 0)		//输入第一个字符串为ln 执行命令 ln
	{
		ln(command);
	}
	else if(strcmp(command[0], "mv") == 0)		//输入第一个字符串为mv 执行命令 mv
	{
		mv(command);
	}
	else
	{
		printf("not found command\n");
		return 0;
	}
}
