SRCS= main.c parsing/tokenizing.c parsing/tokenizing2.c parsing/lexing.c parsing/utils.c parsing/utils2.c parsing/utils3.c parsing/utils4.c parsing/utils5.c\
	parsing/counting_tokens.c parsing/expanding.c parsing/expanding2.c  parsing/expanding3.c parsing/ft_split.c  parsing/lexing2.c execution/infile_outfile_parent.c\
	garbage_collector.c parsing/parsing.c parsing/parsing2.c parsing/parsing3.c execution/builtins.c execution/command.c parsing/parsing_export.c parsing/heredoc.c\
	execution/path.c execution/utils.c execution/cd.c execution/pipe_utils.c execution/builtins2.c execution/export_utils.c execution/infile_outfile.c execution/unset_pwd.c \
	execution/export2.c execution/export3.c execution/env.c execution/exit.c execution/pipe.c execution/export.c execution/utils2.c shlvl_init.c env_init.c execution/exit2.c
OBJS = $(SRCS:%.c=%.o)
HEADER = minishell.h 

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -l readline -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) 

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean
