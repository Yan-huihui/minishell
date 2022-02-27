
OBJ=a.out

SRC+=main.c
SRC+=cmd.c
SRC+=minishell.c

$(OBJ):$(SRC)
	gcc $^ -o $@

.PHONY:
clear:
	rm $(OBJ)

