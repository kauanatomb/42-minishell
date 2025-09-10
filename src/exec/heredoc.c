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

void	ft_warning_heredoc(char *args)
{
	write(2, "bash: warning: here-document at line 1 delimited ", 49);
	write(2, "by end-of-file (wanted `", 24);
	write(2, args, ft_strlen(args));
	write(2, "')\n", 3);
}

int	handle_heredoc_line_end(char *line, char *filename)
{
	if (!line || ft_strcmp(line, filename) == 0)
	{
		if (!line)
			ft_warning_heredoc(filename);
		if (line)
			free(line);
		return (1);
	}
	else
		return (0);
}

void	run_heredoc_child(t_redir *r, int pipe_fd, t_shell *shell)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if (handle_heredoc_line_end(line, r->filename))
			break ;
		line = check_expand_heredoc(r, line, shell);
		if (!line)
		{
			close(pipe_fd);
			exit(ERR_MEMORY);
		}
		write(pipe_fd, line, ft_strlen(line));
		write(pipe_fd, "\n", 1);
		free(line);
	}
	close(pipe_fd);
	exit(0);
}

int	prepare_heredoc_one(t_redir *r, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), ERR_PIPE);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (signal(SIGINT, handle_sig), perror("fork"), ERR_FORK);
	if (pid == 0)
		run_heredoc_child(r, pipe_fd[1], shell);
	close(pipe_fd[1]);
	status = wait_children(pid);
	if (status)
		write(STDOUT_FILENO, "\n", 1);
	signal(SIGINT, handle_sig);
	r->fd = pipe_fd[0];
	r->type = INPUT;
	return (status);
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
