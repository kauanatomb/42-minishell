/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd2.c                                             :+:      :+:    :+:   */
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
		return (builtin_exit(cmd->argv, shell));
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

int	fork_and_exec_builtin(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		ret;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), ERR_FORK);
	if (pid == 0)
	{
		if (apply_redirs_child(cmd->redirs, shell) < 0)
			exit (1);
		ret = exec_builtin_child(cmd, shell);
		free_program(shell);
		exit(ret);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}
