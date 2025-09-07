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

int exec_builtin_parent_pipes(t_cmd *cmd, t_shell *shell)
{
	char	*name;

	name = cmd->argv[0];
	if (!ft_strcmp(name, "cd"))
		return (builtin_cd_child(cmd->argv, shell));
	// if (!ft_strcmp(name, "exit"))
	// 	return (builtin_exit_child(cmd->argv, shell), 0);
	if (!ft_strcmp(name, "export"))
		return (builtin_export_child(cmd->argv, shell));
	if (!ft_strcmp(name, "unset"))
		return (builtin_unset_child(cmd->argv, shell));
	return (1);
}


static pid_t	spawn_pipeline_child(t_cmd *cmd, int fd_in, int fd[2],
			t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		if (fd_in != STDIN_FILENO)
		{
			if (dup2(fd_in, STDIN_FILENO) == -1)
				exit(1);
			close(fd_in);
		}
		if (cmd->next)
		{
			close(fd[0]);
			if (dup2(fd[1], STDOUT_FILENO) == -1)
				exit(1);
			close(fd[1]);
		}
		if (apply_redirs(cmd->redirs) < 0)
			exit(1);
		if (is_builtin_child(cmd->argv[0]))
			exit(exec_builtin_child(cmd, shell));
		else if (is_builtin_parent(cmd->argv[0]))
			exit(exec_builtin_parent_pipes(cmd, shell));
		else
			exit(exec_external(cmd, shell));
	}
	return (pid);
}

static int	wait_pipeline_children(pid_t last_pid)
{
	int		status;
	int		exit_code;
	pid_t	wpid;

	exit_code = 1;
	wpid = waitpid(-1, &status, 0);
	while (wpid > 0)
	{
		if (wpid == last_pid)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = 128 + WTERMSIG(status);
		}
		wpid = waitpid(-1, &status, 0);
	}
	return (exit_code);
}

int	exec_pipeline(t_cmd *cmd, t_shell *shell)
{
	int		fd_in;
	int		fd[2];
	int		status;
	pid_t	pid;

	fd_in = STDIN_FILENO;
	status = 0;
	while (cmd)
	{
		if (cmd->next && pipe(fd) == -1)
			return (perror("pipe"), 1);
		pid = spawn_pipeline_child(cmd, fd_in, fd, shell);
		if (pid == -1)
			return (perror("fork"), 1);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		if (cmd->next)
		{
			close(fd[1]);
			fd_in = fd[0];
		}
		cmd = cmd->next;
	}
	status = wait_pipeline_children(pid);
	return (status);
}
