/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_heredoc(int sig)
{
	(void)sig;
	g_signal = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	prepare_heredocs(t_cmd *cmd)
{
	t_redir	*r;
	int		pipe_fd[2];
	char	*line;

	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->type == HEREDOC)
			{
				if (pipe(pipe_fd) == -1)
					return (perror("pipe"), ERR_PIPE);
				signal(SIGINT, sigint_heredoc);
				while (1)
				{
					if (g_signal == 130)

					line = readline("> ");
					if (!line || ft_strcmp(line, r->filename) == 0)
					{
						free(line);
						break ;
					}
					write(pipe_fd[1], line, ft_strlen(line));
					write(pipe_fd[1], "\n", 1);
					free(line);
				}

				close(pipe_fd[1]);
				r->fd = pipe_fd[0];
				r->type = INPUT;
			}
			r = r->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
