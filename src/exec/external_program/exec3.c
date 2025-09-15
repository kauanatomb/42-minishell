/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_directory(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (1);
	return (0);
}

void	exit_command_not_found(char *cmd, t_shell *shell)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	free_program(shell);
	exit(127);
}

void	exit_is_directory(char *path, t_shell *shell)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	free(path);
	free_program(shell);
	exit(126);
}

void	handle_permission_denied(char *cmd, t_shell *shell)
{
	perror(cmd);
	free_program(shell);
	exit(126);
}

void	clean_argv_empty_cmds(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!cmd || !cmd->argv)
		return ;
	while (cmd->argv[i])
	{
		if (cmd->argv[i][0] != '\0')
			cmd->argv[j++] = cmd->argv[i];
		i++;
	}
	cmd->argv[j] = NULL;
}
