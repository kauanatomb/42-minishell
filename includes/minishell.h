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
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <unistd.h>
# include <signal.h>
# include <stdio.h>

typedef struct	s_token
{
	char	**l_tokens;
	char	**l_types;
}			t_token;

typedef struct s_shell
{
	int		error;
	int		path_flag;
	char	**env;
	t_token	*tokens;
}	t_shell;

// Shell struct
int	init_shell_env(t_shell *shell, char **envp);

// Signal
void	signal_receiver(void);

// Global
extern volatile sig_atomic_t	g_signal;

#endif
