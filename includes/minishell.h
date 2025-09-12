/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/21 18:10:50 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>

typedef enum e_error
{
	ERR_PIPE = -3,
	ERR_FORK = -2,
	ERR_MEMORY = -1,
	ERR_NONE,
	ERR_MISS_FILENAME = 2,
	ERR_UNEXPECTED_TOKEN = 2,
	ERR_PARSE,
	ERR_EXEC,
	ERR_COMM_NOT_FOUND = 127,
}	t_error;

typedef enum e_token_type
{
	WORD,
	PIPE,
	INPUT,
	HEREDOC,
	OUTPUT,
	APPEND,
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
}	t_token;

typedef struct s_redir
{
	int				fd;
	char			*filename;
	int				heredoc_exp;
	t_token_type	type;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	t_error	error;
	int		path_flag;
	char	**env;
	t_token	*tokens;
	t_cmd	*cmds;
	int		num_tokens;
}	t_shell;

// Main
int		ft_isspace(char c);
void	clean_extra_fds(void);
void	exit_ctrld(t_shell *shell);
int		init_shell_env(t_shell *shell, char **envp);
int		ft_array_len(char **arr);
void	ft_free_array(char **dest);
bool	is_line_empty(char *line);
void	free_shell_env(char **env);

// Signal
void	signal_receiver(void);
void	handle_sig(int sig);

// Lexer
int		lex_line(t_shell *shell, const char *line);
void	skip_operator(const char *line, int *i);
int		ft_is_operator(char c);
void	skip_word(const char *line, int *i);
int		tokenize_all(t_shell *shell, const char *line);
void	free_tokens(t_token *tokens);
char	*ft_strndup(const char *s, size_t n);
void	skip_word(const char *line, int *i);

// Parse
int		parse(t_shell *shell);
void	print_parse_error(t_error code, t_token *token);
void	free_cmd_list(t_cmd *cmds);
int		countword(t_shell *shell, int start);
t_cmd	*new_cmd(int counted_words);
void	attach_redir(t_redir *redir, t_cmd *curr);
t_cmd	*get_last_cmd(t_shell *shell);

// Expand var
int		main_expand(t_shell *shell);
char	*expand_dollar_question(t_shell *shell, char *result);
char	*expand_key_var(char **str, t_shell *shell, char *result);
char	*append_char_to_result(char c, char *result);
char	*expand_var(char *str, t_shell *shell);
char	*handle_dollar(char **str, char *result, t_shell *shell, int mode);
char	*append_and_check(char c, char *result);
// Execution
void	exec_cmd(t_cmd *cmd, t_shell *shell);
int		apply_redirs(t_redir *redirs);
int		exec_external(t_cmd *cmd, t_shell *shell);
int		is_builtin_child(char *cmd);
int		is_builtin_parent(char *cmd);
int		exec_builtin_child(t_cmd *cmd, t_shell *shell);
int		prepare_heredocs(t_cmd *cmd, t_shell *shell);
void	clean_argv_empty_cmds(t_cmd *cmd);
//External exec
void	exit_command_not_found(char *cmd);
void	exit_is_directory(char *path);
int		is_directory(const char *path);
void	handle_permission_denied(char *cmd);
// Builtin
int		builtin_echo(char **argv);
int		builtin_unset(char **argv, t_shell *shell);
int		builtin_export(char **argv, t_shell *shell);
int		builtin_cd(char **argv, t_shell *shell);
int		builtin_exit(char **argv, t_shell *shell);
int		builtin_pwd(t_shell *shell);
int		builtin_env(char **env);
// utils
int		create_env(char *key, char *value, char ***env);
int		change_value(char *key, char *value, char **env, int i);
bool	is_valid_key(char *key);
char	**split_key_value_env(char *s);
int		is_created(char *key, char **env);
int		create_env(char *key, char *value, char ***env);
int		check_directory_access(char *arg);
int		builtin_export_print(t_shell *shell);
int		print_error_exp(t_shell *shell, char *arg);
void	p_error_cd(t_shell *shell, char *s, int type);
char	*give_env_value(char **env, char *key);
// heredocs
char	*check_expand_heredoc(t_redir *r, char *line, t_shell *shell);
int		has_heredocs(t_cmd *cmd);
void	ft_warning_heredoc(char *args);
int		handle_heredoc_line_end(char *line, char *filename);
int		handle_fork_error(void);
// pipeline
int		exec_pipeline(t_cmd *cmd, t_shell *shell);
int		builtin_export_child(char **argv, t_shell *shell);
int		builtin_cd_child(char **args, t_shell *shell);
int		builtin_unset_child(char **argv, t_shell *shell);
void	builtin_exit_child(char **argv, t_shell *shell);
int		wait_children(pid_t last_pid, int *signal_happened);
void	handle_pipeline_signal(int status, int signal_happened);

// Global
extern volatile sig_atomic_t	g_signal;

#endif