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

int	exec_builtin_parent_pipes(t_cmd *cmd, t_shell *shell)
{
	char	*name;

	name = cmd->argv[0];
	if (!ft_strcmp(name, "cd"))
		return (builtin_cd_child(cmd->argv, shell));
	if (!ft_strcmp(name, "exit"))
		return (builtin_exit_child(cmd->argv, shell), 0);
	if (!ft_strcmp(name, "export"))
		return (builtin_export_child(cmd->argv, shell));
	if (!ft_strcmp(name, "unset"))
		return (builtin_unset_child(cmd->argv, shell));
	return (1);
}

static void	setup_child_fds(int fd_in, int fd[2], t_cmd *cmd, t_shell *shell)
{
	if (fd_in != STDIN_FILENO)
	{
		if (dup2(fd_in, STDIN_FILENO) == -1)
			exit_child_error(shell, "dup2", 1);
		close(fd_in);
	}
	if (cmd->next)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			exit_child_error(shell, "dup2", 1);
		close(fd[1]);
	}
}

void	run_pipeline_child(t_cmd *cmd, int fd_in, int fd[2], t_shell *shell)
{
	int	ret;

	ret = 0;
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	setup_child_fds(fd_in, fd, cmd, shell);
	if (apply_redirs_child(cmd->redirs, shell) < 0)
		exit(1);
	if (is_builtin_child(cmd->argv[0]))
		ret = exec_builtin_child(cmd, shell);
	else if (is_builtin_parent(cmd->argv[0]))
		ret = exec_builtin_parent_pipes(cmd, shell);
	else
		exec_external_child(cmd, shell);
	free_program(shell);
	exit(ret);
}

static int	exec_one_pipeline(t_cmd **cmd, pid_t *pid, int *fd_in,
		t_shell *shell)
{
	int	fd[2];

	if ((*cmd)->next && pipe(fd) == -1)
		return (perror("pipe"), signal(SIGINT, handle_sig), ERR_PIPE);
	*pid = fork();
	if (*pid == -1)
		return (handle_fork_error());
	if (*pid == 0)
		run_pipeline_child(*cmd, *fd_in, fd, shell);
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	if ((*cmd)->next)
	{
		close(fd[1]);
		*fd_in = fd[0];
	}
	*cmd = (*cmd)->next;
	return (0);
}

int	exec_pipeline(t_cmd *cmd, t_shell *shell)
{
	int		fd_in;
	int		status;
	pid_t	pid;
	int		signal_happened;

	fd_in = STDIN_FILENO;
	status = 0;
	signal_happened = 0;
	signal(SIGINT, SIG_IGN);
	while (cmd)
	{
		if (exec_one_pipeline(&cmd, &pid, &fd_in, shell) != 0)
			return (status = ERR_PIPE);
	}
	status = wait_children(pid, &signal_happened);
	signal(SIGINT, handle_sig);
	handle_pipeline_signal(status, signal_happened);
	if (status != ERR_NONE)
		clean_extra_fds();
	return (status);
}
