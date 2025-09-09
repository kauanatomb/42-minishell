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

void	ft_warning_heredoc(char *args)
{
	write(2, "bash: warning: here-document at line 1 delimited ", 49);
	write(2, "by end-of-file (wanted `", 24);
	write(2, args, ft_strlen(args));
	write(2, "')\n", 3);
}

void	sigint_heredoc(int sig)
{
	(void)sig;
	g_signal = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	close(STDIN_FILENO);
}

int	has_heredocs(t_cmd *cmd)
{
	t_redir	*r;

	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->type == HEREDOC)
				return (1);
			r = r->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

int	run_heredoc_child(t_redir *r, int pipe_fd, t_shell *shell)
{
	char *line;

	(void)shell;
	signal(SIGINT, sigint_heredoc);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, r->filename) == 0)
		{
			if (g_signal == 130)
			{
				close(pipe_fd);
				free(line);
				exit(130);
			}
			if (!line)
				ft_warning_heredoc(r->filename);
			free(line);
			break ;
		}
		// line = check_expand_heredoc(shell, line);
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
	int		saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), ERR_PIPE);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), ERR_FORK);
	if (pid == 0)
		run_heredoc_child(r, pipe_fd[1], shell);
	waitpid(pid, &status, 0);
	close(pipe_fd[1]);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		dup2(saved_stdin, STDIN_FILENO);
		printf("test\n");
		close(saved_stdin);
		return (clean_extra_fds(), 130);
	}
	printf("test 1\n");
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
