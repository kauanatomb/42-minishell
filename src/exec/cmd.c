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

int	is_builtin_parent(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset"));
}

int	is_builtin_child(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "env"));
}

static int	dispatch_cmd(t_cmd *cmd, t_shell *shell)
{
	if (cmd->next)
		return (exec_pipeline(cmd, shell));
	else if (is_builtin_parent(cmd->argv[0]))
		return (exec_builtin_parent(cmd, shell));
	else if (is_builtin_child(cmd->argv[0]))
		return (fork_and_exec_builtin(cmd, shell));
	else
		return (exec_external(cmd, shell));
}

static int	prepare_cmd(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return (1);
	if (has_heredocs(cmd))
	{
		shell->error = prepare_heredocs(cmd, shell);
		if (shell->error)
			return (1);
	}
	if ((!cmd->argv || !cmd->argv[0]) && cmd->redirs)
	{
		if (apply_redirs(cmd->redirs) != 0)
		{
			shell->error = 1;
			return (1);
		}
	}
	if (!cmd->argv || !cmd->argv[0])
		return (1);
	return (0);
}

void	exec_cmd(t_cmd *cmd, t_shell *shell)
{
	if (prepare_cmd(cmd, shell))
		return ;
	shell->error = dispatch_cmd(cmd, shell);
	g_signal = 0;
}
