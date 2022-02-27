#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "minishell.h"


int  touch(char *filename)					//touch命令：创建指定文件
{
	FILE *fp = fopen(filename, "w");
	if(NULL == fp)
	{
		printf("fail to fopen\n");
		return -1;
	}

	fclose(fp);
	return 0;
}

int cat(char *filename)						//cat命令
{
	FILE *fp = fopen(filename,"r");
	if(NULL == fp)
	{
		printf("fail to fopen\n");
		return -1;
	}

	int ch = 0;								//单个字符读取输出

	while((ch = fgetc(fp)) != EOF)
	{
		printf("%c",ch);
	}

	fclose(fp);
	return 0;
}

int cp(char *src, char *dst)				//copy命令
{
	FILE *fpsrc = fopen(src, "r");
	FILE *fpdst = fopen(dst, "w");
	if(NULL == fpsrc || NULL == fpdst)
	{
		printf("fail to fopen\n");
		return -1;
	}
	while(1)
	{
		int ch = fgetc(fpsrc);				//单个字符读取拷贝到目标文件
		if(EOF == ch)
		{
			break;
		}
		fputc(ch,fpdst);
	}

	fclose(fpsrc);
	fclose(fpdst);
	return 0;
}

int cd(char *path)							//cd命令：跳到指定目录
{
	int ret = chdir(path);
	if(ret < 0)
	{
		printf("erro prth\n");
	}
	return 0;
}

void pwd(void)								//pwd命令：输出当前工作路径
{
	char buff[1024] = {0};
	getcwd(buff, sizeof(buff));
	printf("%s\n",buff);
}

int do_mkdir(char *path)					//mkdir命令：指定路径下创建文件夹
{
	int ret = mkdir(path, 0777);
	if(ret < 0 )
	{
		printf("fail to mkdir\n");
	}
}

int do_rmdir(char *pathname)				//rmdir命令：删除指定路径下空文件夹
{
	int ret = rmdir(pathname);
	if(ret < 0 )
	{
		printf("fail to rmdir\n");
	}
}

int rm(char *pathname)						//rm命令：删除指定路径下普通文件或空文件夹
{
	int ret = remove(pathname);
	if(ret < 0 )
	{
		printf("fail to rmdir\n");
	}
}

int ls(char *command[10])					//ls命令：按照对应模式输出路径下文件
{
	char *path = NULL;
	int mode = 0;							//默认输入ls为模式 0   输入ls -a 为模式 1  输入ls -l为模式 2
	if(NULL == command[1])
	{
		path = ".";
	}
	else if(strcmp(command[1], "-a") == 0)	
	{
		mode = 1;
		if(NULL == command[2])
		{
			path = ".";						//在判断输入ls -a 条件下后续无输入 给后面do_ls_dir 函数传入当前地址 .
		}
		else								//在判断输入ls -a 条件下后续有输入 给后面do_ls_dir 函数传入输入地址
		{
			path = command[2];
		}
	}
	else if(strcmp(command[1], "-l") == 0)
	{
		mode = 2;
		if(NULL == command[2])
		{
			path = ".";						//在判断输入ls -l 条件下后续无输入 给后面do_ls_dir 函数传入当前地址 .
		}
		else								//在判断输入ls -l 条件下后续有输入 给后面do_ls_dir 函数传入输入地址
		{
			path = command[2];
		}
	}
	else
	{
		path = command[1];
	}

	if(NULL != command[2] && strcmp(command[2],"-a") == 0)
	{
		mode = 1;
		path = command[1];
	}
	else if(NULL != command[2] && strcmp(command[2], "-l") == 0)
	{
		mode = 2;
		path = command[1];
	}
	do_ls_dir(path, mode);
}

int do_ls_dir(const char *pathname, int mode)	//指定目录按对应模式输出目录下文件
{
	DIR *dir = opendir(pathname);
	if(NULL == dir)
	{
		printf("fail to opendir\n");
		return -1;
	}
	while(1)
	{
		struct dirent *p = readdir(dir);
		if(NULL == p)
		{
			break;
		}
		char path[1024] = {0};
		if(strcmp(pathname, "/") == 0)
		{
			sprintf(path, "%s%s",  pathname, p->d_name);	//根目录拼接
		}
		else
		{
			sprintf(path, "%s/%s", pathname, p->d_name);	//其他目录拼接
		}
		if(p->d_name[0] == '.' && mode != 1)				//在ls (模式 0)和ls -l(模式 2)下跳过隐藏文件
		{
			continue;
		}
		do_ls_file(path, mode);
	}
	if(mode != 2)											//在ls 和 ls -a 输出完成后换行
	{
		printf("\n");
	}
	closedir(dir);
	return 0;
}

int do_ls_file(const char *filename, int mode)
{
	if(mode == 2)						//在ls -l条件下输出以下信息
	{
		char *strmon[12] = {"Jan", "Fed", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep","Oct", "Nov", "Dec"};

		struct stat buff;				//定义stat结构体缓冲区buff
		lstat(filename, &buff);			//获得文件属性放入buff

		if(S_ISREG(buff.st_mode))		//获取文件类型输出
		{
			putchar('-');
		}
		else if(S_ISDIR(buff.st_mode))
		{
			putchar('d');
		}
		else if(S_ISCHR(buff.st_mode))
		{
			putchar('c');
		}
		else if(S_ISBLK(buff.st_mode))
		{
			putchar('b');
		}
		else if(S_ISFIFO(buff.st_mode))
		{
			putchar('p');
		}
		else if(S_ISLNK(buff.st_mode))
		{
			putchar('l');
		}
		else if(S_ISSOCK(buff.st_mode))
		{
			putchar('s');
		}
		else
		{
			putchar('#');
		}

		buff.st_mode & S_IRUSR ? putchar('r') : putchar('-');	//获取用户权限输出
		buff.st_mode & S_IWUSR ? putchar('w') : putchar('-');
		buff.st_mode & S_IXUSR ? putchar('x') : putchar('-');

		buff.st_mode & S_IRGRP ? putchar('r') : putchar('-');	//获取组中权限输出
		buff.st_mode & S_IWGRP ? putchar('w') : putchar('-');
		buff.st_mode & S_IXGRP ? putchar('x') : putchar('-');

		buff.st_mode & S_IROTH ? putchar('r') : putchar('-');	//获取其他用户权限输出
		buff.st_mode & S_IWOTH ? putchar('w') : putchar('-');
		buff.st_mode & S_IXOTH ? putchar('x') : putchar('-');

		struct passwd *pw = getpwuid(buff.st_uid);				//通过文件uid得到用户名字符串
		struct group  *pg = getgrgid(buff.st_gid);				//通过文件gid得到组名称字符串
		struct tm *ptm = localtime(&(buff.st_mtime));			//通过文件最后修改时间得到结构体 tm 的时间

		printf(" %2lu %s %s %5ld",buff.st_nlink, pw->pw_name,pg->gr_name, buff.st_size);			//输出链接数、用户名、组名、文件大小
		printf(" %s %2d %02d:%02d ", strmon[ptm->tm_mon], ptm->tm_mday, ptm->tm_hour, ptm->tm_min);	//输出最后修改时间
	}

	const char *p = filename + strlen(filename) - 1;		//从文件名最后找/输出后面的名字
	while(*p != '/')
	{
		--p;
	}
	++p;

	printf("%s", p);						//输出文件名
	if(mode != 2)											
	{
		printf("  ");						//在ls 和 ls -a下以另个空格隔开
	}
	else
	{
		printf("\n");						//在ls -l下以换行隔开
	}
	return 0;
}

int ln(char *command[10])					//ln 命令：创建一个硬链接或者软连接
{
	int ret;
	if(strcmp(command[1], "-s") == 0 && command[2] != NULL && command[3] != NULL)	//输入ln -s 且后面还有两个语句下创建软连接
	{
		ret = symlink(command[2], command[3]);
		if(ret < 0)
		{
			printf("fail to symlink\n");
			return -1;
		}
	}
	else if(command[1] != NULL && command[2] != NULL)	//输入ln 且后面还有两个语句下创建硬连接
	{
		ret = link(command[1], command[2]);
		if(ret < 0)
		{
			printf("fail to link\n");
			return -1;
		}
	}
	else
	{
		printf("Usage ln failname\n");
	}
}

int mv(char *command[10])
{
	if(NULL == command[1] || NULL == command[2])
	{
		printf("Usage mv failname\n");
		return -1;
	}
	struct stat buffsrc;
	struct stat buffdst;

	int ret_lstat = lstat(command[1], &buffsrc);	//获得第一个输入的文件属性放入buffsrc
	lstat(command[2], &buffdst);					//获得第二个输入的文件属性放入buffdst
	if(ret_lstat < 0)
	{
		printf("fail to lstat\n");
		return -1;
	}

	if(S_ISREG(buffsrc.st_mode) && S_ISDIR(buffdst.st_mode))	//第一个输入的文件类型为普通文件第二个输入的文件类型为目录文件下移动文件
	{
		char buff[1024] = {0};

		if(command[1][0] == '.' || command[1][0] == '/')		//字符串拼接获得移动后路径名称
		{
			char *p = command[1] + strlen(command[1]) - 1;
			while(*p != '/')
			{
				--p;
			}
			++p;
			sprintf(buff, "%s/%s",command[2], p);
		}
		else
		{
			sprintf(buff, "%s/%s",command[2], command[1]);
		}
		cp(command[1], buff);									//拷贝普通文件到对应目录下
		rm(command[1]);											//删除之前普通文件

	}
	else if(S_ISREG(buffsrc.st_mode) || S_ISDIR(buffsrc.st_mode))	//第一个输入的文件类型为普通文件或者目录文件重命名为输入的第二个字符名称
	{
		int ret_rename = rename(command[1], command[2]);
		if(ret_rename < 0)
		{
			printf("fail to rename\n");
		}
	}
	else
	{
		printf("type mismach\n");
		return -1;
	}

	return 0;
}




