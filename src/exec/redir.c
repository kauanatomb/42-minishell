/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_input_fd(t_redir *redirs)
{
	if (redirs->fd != 0)
		return (redirs->fd);
	else
		return (open(redirs->filename, O_RDONLY));
}

int	apply_redirs(t_redir *redirs, t_shell *shell)
{
	int	fd;

	while (redirs)
	{
		if (redirs->type == OUTPUT)
			fd = open(redirs->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redirs->type == APPEND)
			fd = open(redirs->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redirs->type == INPUT)
			fd = get_input_fd(redirs);
		else
			return (1);
		if (fd < 0)
			return (perror(redirs->filename), free_program(shell), -1);
		if (redirs->type == INPUT)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redirs = redirs->next;
	}
	return (0);
}
