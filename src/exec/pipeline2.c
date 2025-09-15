/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_children(pid_t last_pid, int *signal_happened)
{
	int		status;
	int		exit_code;
	pid_t	wpid;

	exit_code = 1;
	wpid = waitpid(-1, &status, 0);
	while (wpid > 0)
	{
		if (WIFEXITED(status) && last_pid == wpid)
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (last_pid == wpid)
				exit_code = 128 + WTERMSIG(status);
			*signal_happened = 1;
		}
		wpid = waitpid(-1, &status, 0);
	}
	if (exit_code == 141)
		exit_code = 1;
	return (exit_code);
}

void	exit_child_error(t_shell *shell, const char *msg, int code)
{
	if (msg)
		perror(msg);
	free_program(shell);
	exit(code);
}

void	handle_pipeline_signal(int status, int signal_happened)
{
	if ((signal_happened && status == 0) || (signal_happened && status == 130))
		write(STDOUT_FILENO, "\n", 1);
	if (status == 131)
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
}
