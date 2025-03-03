/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbenzaid <tbenzaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:09:41 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/03 01:46:36 by tbenzaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
# include <fcntl.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/stat.h>


extern int g_in_readline;

typedef struct s_env {
    char *env_var;
    struct s_env *next;
} t_env;

typedef enum e_tokens_types
{
	PIPE ,
	INPUT_DIRECTION ,
	OUTPUT_DIRECTION ,
	OUT_APPEND ,
	HERE_DOC ,
	WORD ,
}	t_tokens_types;

typedef struct s_token
{
	int		type;
	int		quoted;
	int		expandable;
	int		part_of_previous;
	char	*content;
	int		split_later;
}	t_token;

typedef enum e_quote
{
	NO_QUOTE,
	S_QUOTE,
	D_QUOTE,
}	t_quote;

typedef	struct s_alloc
{
	void	*addr;
	struct s_alloc	*next;
}	t_alloc;

typedef struct s_redir
{
	char	*name;
	int		type;
	int		here_doc_expandable;
	char	*here_doc_filename;
	int		quote;
	struct s_redir *next;
}	t_redir;


typedef	struct s_command
{
	char	**args;
	int 	arg_n;
	t_redir	*files;
}	t_command;

typedef	struct s_data
{
	char	*line;
	char	*pipe_line;
	t_env   *env;
	int	exit_status;
	t_command	**commands;
	t_command	**readline_commands;
	int		command_count;
	int		readline_command_count;
	t_token		**tokens;
	int	token_size;
	t_token		**readline_tokens;
	t_alloc	*alloc;
	t_alloc	*child_alloc;
	char	*default_path;
}	t_data;

int		count_tokens(char *str);
int		lexing(t_token **tokens, t_data *data);
int		check_quotes(char *str, t_data *data);
int		is_operation(int i);
int		check_special_char(t_token **tokens);
int		w_ch(char c);
int		is_special(char c);
int		to_handle(char c);
int		handle_quotes(char *str, int *i, char quote_char);
int		count_no_quotes(char *str);
int	check_consecutive_expressions(t_token **tokens, t_data *data);
int		check_end_and_pipe(t_token **tokens, t_data *data);

void	tokenize(t_data *data, char *cmd);
void	loop_token_arr(char *str, t_token **token_arr, t_data *data);
void	insert_other_ops(t_token *token, char *str, t_data *data);
t_token	*insert_token_arr_word(int *i, char *str, int quote, t_data *data);
t_token	*insert_token_arr_op(int *i, char *str, t_data *data);

void	expanding(t_data *data);
char	*handle_multiple_dollars(char	*token, int count, int i, t_data *data);
char	*build_exp_str(char *token, char *previous, int i, t_data *data);
char	*handle_quoted_token(char *token, t_data *data);
char	*expand_token(char *token, t_data *data);
char	*create_spaces(int count, t_data *data);
char	*join_tokens(char **strs, char *token, int j, t_data *data);
char	*get_expanded_value(char *token, int i, int *j, t_data *data);
int	skip_ops_except_pipe(t_token **tokens, int *current, t_data *data);
void	handle_heredocs2(t_token **tokens, int stop, t_data *data);
void	handle_heredocs(t_token **tokens, t_data *data);
void	remove_token(t_token **tokens, int index);

void	parse(t_data *data);
int	count_commands(t_token **tokens);

void	tokenize_new_stuff(char *cmd, t_data *data);
void	tokenize_pipe(t_data *data, char *cmd);
void	expanding_new_stuff(t_data *data);
void	parsing_new_stuff(t_data *data);
void	join_both_arrays(t_data *data);
int	lexing_new_stuff(t_token **tokens, t_data *data);
int	check_consecutive_expressions2(t_token **tokens, t_data *data);

void	add_data_line(char *str, t_data *data);

size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1, t_data *data);
int		ft_isprint(int c);
char	*ft_strjoin(char const *s1, char const *s2, t_data *data);
char	**ft_split(char const *s, char sep, t_data *data);
int		is_quote(char c);
char	*ft_itoa(int n, t_data *data);
char	*ft_strtrim(char const *s1, t_data *data);
char	*get_full_name_in(t_token **tok, t_redir *inf, int *i, t_data *data);
int	count_commands(t_token **tokens);
int	arg_n(t_token **tokens, int tokens_count);

void	ft_lstclear_garbage(t_alloc **lst);
void	ft_lstadd_front(t_alloc **lst, t_alloc *new);
void	*ft_malloc(size_t size, t_data *data);
void	print_error_status(int status, char *str, const char *msg);
void	exit_status(int status, t_data *data);

char	*get_full_name(t_token **tokens, int *i, t_data *data);
char	*get_path(char **env, char *cmp, t_data *data, t_alloc **head);
void	free_ultimate(char **args);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
void echo(char **str);
void cd(char **str, t_data *data);
void pwd();
void env(char **str, t_data *data);
void export(char **str, t_data *data);
void exit_program(char **str, t_data *data);
void addenv(char **env, t_env **head);
int execu_cmd(char **str, char **env_list, t_data *data);
char **convert_env_list_to_array(t_env *head, t_data *data);
int    execute(t_data *data);
t_redir	*insert_file(t_token **tokens, int *i, t_data *data);
void	here_doc(t_redir *infile, t_data *data);
void pipe_cas(t_command **cmd,t_env *env_list,t_data *data);
void check_if_building(char **args, t_env *env_list,t_data *data, t_alloc **head);
void	lstadd(t_redir **lst, t_redir *new);
int	ft_strcmp(char *s1, char *s2);
void	free_exit(t_data *data);
size_t	word_count(char const *s, char sep);
void	files(t_command *command);
t_env	*ft_lstlast2(t_env *lst);
int	ft_isalnum(int c);
void	ft_lstadd_back2(t_env **lst, t_env *new);
void unset(char **str, t_data *data);
char	*ft_strncpy(char *dest, char *src, int n);
void add_export(char *str, t_data *data);
int is_valid(const char *str);
char **sort_export(t_env *head, t_data *data, t_alloc **head_ch);
void	sigint_handler(int sig);
void exit_child(char **str);
void check_add(char *str, t_data *data);
char	*ftt_strdup( char *s1);
char	*ftt_strdup2( char *s1, t_alloc **head);
void	*ft_malloc2(size_t size, t_alloc **child);
void cd_child(char **str, t_data *data);
void export_child(char **str, t_data *data, t_alloc **head);
char	*ft_strjoin2(char const *s1, char const *s2, t_alloc **data);
char	*ft_getenv2(char *str, t_data *data);
void sigquit_handler(int sig);
#endif