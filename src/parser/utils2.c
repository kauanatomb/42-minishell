/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*get_last_cmd(t_shell *shell)
{
	t_cmd	*cmd;

	cmd = shell->cmds;
	if (!cmd)
		return (NULL);
	while (cmd->next)
		cmd = cmd->next;
	return (cmd);
}

void	attach_redir(t_redir *redir, t_cmd *curr)
{
	t_redir	*last;

	if (!curr->redirs)
		curr->redirs = redir;
	else
	{
		last = curr->redirs;
		while (last->next)
			last = last->next;
		last->next = redir;
	}
}

int	set_redir_target(t_redir *redir, t_token *file_tok)
{
	if (redir->type != HEREDOC)
	{
		redir->filename = ft_strdup(file_tok->value);
		if (!redir->filename)
			return (ERR_MEMORY);
	}
	else
	{
		redir->delimiter = ft_strdup(file_tok->value);
		if (!redir->delimiter)
			return (ERR_MEMORY);
	}
	return (ERR_NONE);
}
