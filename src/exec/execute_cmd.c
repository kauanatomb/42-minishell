/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *name)
{
	if (!name)
		return (0);
	return (!ft_strcmp(name, "cd") || !ft_strcmp(name, "exit")
		|| !ft_strcmp(name, "export") || !ft_strcmp(name, "unset")
		|| !ft_strcmp(name, "echo") || !ft_strcmp(name, "pwd")
		|| !ft_strcmp(name, "env"));
}

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

void	execute_cmd(t_cmd *cmd, t_shell *shell)
{
	int		status;
	pid_t	pid;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	if (!ft_strcmp(cmd->argv[0], "cd") || !ft_strcmp(cmd->argv[0], "exit")
		|| !ft_strcmp(cmd->argv[0], "export")
		|| !ft_strcmp(cmd->argv[0], "unset"))
	{
		shell->error = exec_builtin_parent(cmd, shell);
	}
	else if (!ft_strcmp(cmd->argv[0], "echo") || !ft_strcmp(cmd->argv[0], "pwd")
		|| !ft_strcmp(cmd->argv[0], "env"))
	{
		pid = fork();
		if (pid == -1)
			return (perror("fork"));
		if (pid == 0)
		{
			if (apply_redirs(cmd->redirs) < 0)
				exit(1);
			exit(exec_builtin_child(cmd, shell));
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			(shell->error = WEXITSTATUS(status));
	}
	else
		shell->error = exec_external(cmd, shell);
}
