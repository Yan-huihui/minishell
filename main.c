#include <stdio.h>
#include "cmd.h"
int main(int argc, const char *argv[])
{
	while(1)
	{
		char cmd[1024] = {0};
		show_cmd_line();
		int ret_get = get_cmd(cmd);
		if(ret_get == -1)
		{
			continue;
		}
		int ret = do_cmd(cmd);
		if(ret == 1)
		{
			break;
		}
	}	
	return 0;
}
