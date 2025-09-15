/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*validate_command_path(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;

	cmd_path = find_command_path(cmd->argv[0], shell->env);
	if (!cmd_path)
	{
		if (errno == EACCES)
			handle_permission_denied(cmd->argv[0], shell);
		else
			exit_command_not_found(cmd->argv[0], shell);
	}
	if (is_directory(cmd_path))
		exit_is_directory(cmd_path, shell);
	return (cmd_path);
}

static void	setup_child_env(t_cmd *cmd, t_shell *shell)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!cmd->argv[0] || !cmd->argv[0][0])
	{
		free_program(shell);
		exit(0);
	}
	if (apply_redirs_child(cmd->redirs, shell) < 0)
		exit(1);
}

void	exec_external_child(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;

	setup_child_env(cmd, shell);
	cmd_path = validate_command_path(cmd, shell);
	execve(cmd_path, cmd->argv, shell->env);
	perror(cmd_path);
	free_program(shell);
	free(cmd_path);
	exit(126);
}

int	exec_external(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		signal_happened;

	signal(SIGINT, SIG_IGN);
	signal_happened = 0;
	pid = fork();
	status = 1;
	if (pid == -1)
		return (perror("fork"), signal(SIGINT, handle_sig), ERR_FORK);
	if (pid == 0)
		exec_external_child(cmd, shell);
	status = wait_children(pid, &signal_happened);
	signal(SIGINT, handle_sig);
	handle_pipeline_signal(status, signal_happened);
	return (status);
}
