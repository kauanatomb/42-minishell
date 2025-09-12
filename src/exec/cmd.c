/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin_parent(t_cmd *cmd, t_shell *shell)
{
	char	*name;

	name = cmd->argv[0];
	if (!ft_strcmp(name, "cd"))
		return (builtin_cd(cmd->argv, shell));
	if (!ft_strcmp(name, "exit"))
		return (builtin_exit(cmd->argv, shell), 0);
	if (!ft_strcmp(name, "export"))
		return (builtin_export(cmd->argv, shell));
	if (!ft_strcmp(name, "unset"))
		return (builtin_unset(cmd->argv, shell));
	return (1);
}

int	exec_builtin_child(t_cmd *cmd, t_shell *shell)
{
	char	*name;

	name = cmd->argv[0];
	if (!ft_strcmp(name, "echo"))
		return (builtin_echo(cmd->argv));
	if (!ft_strcmp(name, "pwd"))
		return (builtin_pwd(shell));
	if (!ft_strcmp(name, "env"))
		return (builtin_env(shell->env));
	return (1);
}

int	is_builtin_parent(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset"));
}

int	is_builtin_child(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "env"));
}

int	fork_and_exec_builtin(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), ERR_FORK);
	if (pid == 0)
	{
		if (apply_redirs(cmd->redirs) < 0)
			exit (1);
		exit(exec_builtin_child(cmd, shell));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}

void	exec_cmd(t_cmd *cmd, t_shell *shell)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	if (has_heredocs(cmd))
	{
		shell->error = prepare_heredocs(cmd, shell);
		if (shell->error)
			return ;
	}
	if (cmd->next)
	{
		shell->error = exec_pipeline(cmd, shell);
		return ;
	}
	if (is_builtin_parent(cmd->argv[0]))
	{
		shell->error = exec_builtin_parent(cmd, shell);
		return ;
	}
	if (is_builtin_child(cmd->argv[0]))
	{
		shell->error = fork_and_exec_builtin(cmd, shell);
		return ;
	}
	shell->error = exec_external(cmd, shell);
	g_signal = 0;
}
