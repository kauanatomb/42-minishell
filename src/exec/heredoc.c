/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_fork_error(void)
{
	signal(SIGINT, handle_sig);
	perror("fork");
	clean_extra_fds();
	return (ERR_FORK);
}

void	handle_signal_heredoc(int sig)
{
	g_signal = 128 + sig;
	write(STDIN_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	close(STDIN_FILENO);
}

static int	read_heredoc_to_pipe(int write_fd, t_redir *r, t_shell *shell)
{
	char	*line;

	g_signal = 0;
	signal(SIGINT, handle_signal_heredoc);
	while (1)
	{
		line = readline("> ");
		if (g_signal == 130)
			return (free(line), close(write_fd), 130);
		if (handle_heredoc_line_end(line, r->filename))
			break ;
		line = check_expand_heredoc(r, line, shell);
		if (!line)
		{
			close(write_fd);
			return (ERR_MEMORY);
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
	signal(SIGINT, handle_sig);
	return (0);
}

int	prepare_heredoc_one(t_redir *r, t_shell *shell)
{
	int	pipe_fd[2];
	int	ret;
	int	saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), ERR_PIPE);
	ret = read_heredoc_to_pipe(pipe_fd[1], r, shell);
	close(pipe_fd[1]);
	if (ret != 0)
	{
		if (g_signal == 130)
			dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
		clean_extra_fds();
		return (ret);
	}
	r->fd = pipe_fd[0];
	r->type = INPUT;
	return (0);
}

int	prepare_heredocs(t_cmd *cmd, t_shell *shell)
{
	t_redir	*r;
	int		ret;

	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->type == HEREDOC)
			{
				ret = prepare_heredoc_one(r, shell);
				if (ret != 0)
					return (ret);
			}
			r = r->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
