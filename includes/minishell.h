/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:47:47 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <unistd.h>
# include <signal.h>

typedef enum e_error
{
	ERR_NONE,
	ERR_LEX,
	ERR_PARSE,
	ERR_EXEC,
}	t_error;

typedef enum e_quote_type
{
	NONE,
	SINGLE,
	DOUBLE,
}	t_quote_type;

typedef enum e_token_type
{
	WORD,
	PIPE,
	INPUT,
	HEREDOC,
	OUTPUT,
	APPEND,
}	t_token_type;

typedef struct	s_token
{
	char			*value;
	t_token_type	type;
	t_quote_type	quote_type;
}			t_token;
// at the moment im not dealing with heredoc
typedef struct s_cmd
{
	char    **argv; // WORDs
	char    *infile; // if <
	char    *outfile; // if > or >>
	int     append;      // 1 if ">>", 0 if ">"
	struct s_cmd *next;  // next if pipes
}   t_cmd;

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
int	ft_isspace(char c);

// Shell struct
int	init_shell_env(t_shell *shell, char **envp);

// Signal
void	signal_receiver(void);

// Lexer
int lex_line(t_shell *shell, const char *line);
void skip_operator(const char *line, int *i);
int ft_is_operator(char c);
void skip_word(const char *line, int *i);

int tokenize_all(t_shell *shell, const char *line);
void ft_free_tokens(t_token *tokens);
char *ft_strndup(const char *s, size_t n);

// PARSE
int parse(t_shell *shell);

// Global
extern volatile sig_atomic_t	g_signal;

#endif
