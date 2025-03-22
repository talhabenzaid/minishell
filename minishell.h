/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:09:41 by oessoufi          #+#    #+#             */
/*   Updated: 2025/03/15 19:41:20 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define PATH "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <sys/types.h>
# include <limits.h>
# include <sys/stat.h>

extern int	g_in_readline;

typedef struct s_env
{
	char			*env_var;
	struct s_env	*next;
}	t_env;

typedef enum e_tokens_types
{
	PIPE,
	INPUT_DIRECTION,
	OUTPUT_DIRECTION,
	OUT_APPEND,
	HERE_DOC,
	WORD,
}	t_tokens_types;

typedef struct s_token
{
	int		type;
	int		quoted;
	int		expandable;
	int		part_of_previous;
	char	*content;
	int		split_later;
	int		here_doc_trim;
	int		ambigious;
}	t_token;

typedef enum e_quote
{
	NO_QUOTE,
	S_QUOTE,
	D_QUOTE,
}	t_quote;

typedef struct s_alloc
{
	void			*addr;
	struct s_alloc	*next;
}	t_alloc;

typedef struct s_redir
{
	char			*name;
	int				type;
	int				here_doc_expandable;
	char			*here_doc_filename;
	int				here_doc_trim;
	int				quote;
	int				ambigious;
	int				here_doc_fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char	**args;
	int		arg_n;
	int		is_export;
	t_redir	*files;
}	t_command;

typedef struct s_data
{
	char		*pwd;
	char		*line;
	t_env		*env;
	t_command	**commands;
	int			command_count;
	t_token		**tokens;
	int			token_size;
	t_alloc		*alloc;
	char		*default_path;
	int			*fds;
	int			fd_count;
	int			fd_write;
}	t_data;

int		count_tokens(char *str);
int		lexing(t_token **tokens, t_data *data);
int		check_quotes(char *str);
int		is_operation(int i);
int		w_ch(char c);
int		to_handle(char c);
int		handle_quotes(char *str, int *i, char quote_char);
int		count_no_quotes(char *str);
int		check_consecutive_expressions(t_token **tokens);
int		check_end_and_pipe(t_token **tokens, t_data *data);

void	tokenize(t_data *data, char *cmd);
void	loop_token_arr(char *str, t_token **token_arr, t_data *data);
void	insert_other_ops(t_token *token, char *str, t_data *data);
t_token	*insert_token_arr_word(int *i, char *str, int quote, t_data *data);
t_token	*insert_token_arr_op(int *i, char *str, t_data *data);

void	expanding(t_data *data);
char	*handle_multiple_dollars(char *token, int count, int i, t_data *data);
char	*build_exp_str(char *token, char *previous, int *i, t_data *data);
char	*expand_token(char *token, t_data *data);
char	*get_expanded_value(char *token, int i, int *j, t_data *data);

int		skip_ops_except_pipe(t_token **tokens, int *current, t_data *data);
void	handle_heredocs(t_token **tokens, t_data *data);
void	remove_token(t_token **tokens, int index);
int		here_doc(t_redir *infile, int *j, t_data *data);

void	check_export(t_data *data, t_token **tokens, t_command *command);
t_redir	*in_heredoc(t_token **tokens, int *i, t_data *data);
int		parse(t_data *data);
int		count_commands(t_token **tokens);
void	ft_putstr_fd(char *s, int fd);
int		arg_n(t_token **tokens, int tokens_count);
void	handle_redirect(t_token **tokens, int *i, t_data *data);
char	*get_full_name(t_token **tokens, int *i, t_data *data);
char	*get_full_name_in(t_token **tok, t_redir *inf, int *i, t_data *data);
t_redir	*insert_file(t_token **tokens, int *i, t_data *data);
void	splt(char *str, char **command, int *j, t_data *data);

void	add_data_line(char *str, t_data *data);
void	ft_lstclear_garbage(t_alloc **lst);
void	ft_lstadd_front(t_alloc **lst, t_alloc *new);
void	*ft_malloc(size_t size, t_data *data);
void	*ft_malloc2(size_t size, t_data *data, t_alloc **child);
void	print_error_status(char *str, const char *msg);
void	exit_status(int status);
void	free_exit(t_data *data);
void	free_exit_child(t_data *data, t_alloc **head, int i);
char	*expand_token2(t_token **tokens, char *token, t_data *data);
int		exit_stat(int status, int flag);

void	echo(char **str);
void	cd(char **str, t_data *data);
void	pwd(t_data *data);
void	env(char **str, t_data *data, t_alloc **head);
void	export(char **str, t_data *data);
void	unset(char **str, t_data *data);
void	exit_program(char **str, t_data *data);
void	cd_child(char **str, t_data *data, t_alloc **head);
void	export_child(char **str, t_data *data, t_alloc **head);
void	exit_child(char **str, t_data *data, t_alloc **head);
void	check_if_building(char **args, t_data *data, t_alloc **head);
void	cd_to_path(t_data *data, char *path);
void	cd_to_home(t_data *data);
int		prepare_cd_path(char **str, t_data *data, char **target_path);

int		execu_cmd(char **str, char **env_list, t_data *data);
char	**convert_env_list_to_array(t_env *head, t_data *data);
int		execute(t_data *data);
void	pipe_cas(t_command **cmd, t_env *env_list, t_data *data);
void	files(t_command *command, t_data *data, t_alloc **head);

void	addenv(char **env, t_data *data);
void	add_export(char *str, t_data *data);
char	**sort_export(t_env *head, t_data *data, t_alloc **head_ch);
void	check_add(char *str, t_data *data);
char	*ft_getenv2(char *str, t_data *data);
char	*get_path(char **env, char *cmp, t_data *data, t_alloc **head);
void	free_exit2(t_data *data, int i);

void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	noting(int seg);
void	destroy_heredocs(t_data *data);

size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1, t_data *data);
char	*ft_strdup2(char *s1, t_data *data, t_alloc **head);
char	*ftt_strdup(char *s1);
int		ft_isprint(int c);
int		ft_isalnum(int c);
char	*ft_strjoin(char const *s1, char const *s2, t_data *data);
char	*ft_strjoin2(char *s1, char*s2, t_data *data1, t_alloc **data);
char	**ft_split(char const *s, char sep, t_data *data);
int		is_quote(char c);
char	*ft_itoa(int n, t_data *data);
char	*ft_strtrim(char const *s1, t_data *data);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strncpy(char *dest, char *src, int n);
size_t	word_count(char const *s, char sep);
int		is_valid(const char *str);
char	*ft_strtrim_tab(char const *s1, t_data *data);
int		ft_isdigit(int c);
int		is_redir(int i);
char	*ft_getenv(char *str, t_data *data);

void	lstadd(t_redir **lst, t_redir *new);
t_env	*ft_lstlast2(t_env *lst);
void	ft_lstadd_back2(t_env **lst, t_env *new);
int		ft_isalpha(int c);
void	update_existing(t_env *current, char *str, char *rest, t_data *data);
void	handle_new_entry(char *str, char *rest, int key_len, t_data *data);
void	append_env(t_env *node, char *new_value,
			char *current_rest, t_data *data);
void	execute_command(char **argv, char **env, t_data *data,
			t_command *command);
void	ft_lstclear_env(t_env **lst);
void	ft_dup2(int input, int output, t_data *data);
void	remove_env_var(char *var_name, t_data *data);
int		files_p(t_command *command);
void	command_init(t_command *command);
int		arg_x(t_token **tokens, int tokens_count);
int		fih_space(const char *str);
int		ft_strcmp_env(const char *s1, const char *s2);
void	shlvl_init(t_data *data);
int		is_inenv(char *str, t_data *data);
void	ft_putnbr_fd(int n, int fd);
int		ft_atoi(const char *str);
void	data_init(t_data *data, int argc, char **argv);
void	env_init(char **env, t_data *data);
char	*get_heredoc_name(t_data *data);
int		open_heredocs(t_data *data);
int		update_pwd_and_oldpwd(t_data *data, char *path3, char *prev_wd);
void	print_too_many_args(int is_child);
void	print_invalid_arg_msg(char *arg, int is_child);
void	handle_exit(t_data *data, t_alloc **head, int status, int is_child);
void	initialize_heredoc_fds(t_data *data);
int		count_heredocs(t_command **commands);

#endif