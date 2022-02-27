#ifndef __MINISHELL__
#define __MINISHELL__


extern int touch(char *filename);
extern int cat(char *filename);
extern int cp(char *src, char *dst);
extern int cd(char *path);
extern void pwd(void);
extern int do_mkdir(char *path);
extern int do_rmdir(char *pathname);
extern int rm(char *pathname);
extern int ls(char *command[10]);
extern int do_ls_dir(const char *pathname, int mode);
extern int do_ls_file(const char *filename, int mode);
extern int ln(char *command[10]);
extern int mv(char *command[10]);
#endif
