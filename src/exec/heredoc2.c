/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_heredocs(t_cmd *cmd)
{
	t_redir	*r;

	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->type == HEREDOC)
				return (1);
			r = r->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

char	*check_expand_heredoc(t_redir *r, char *line, t_shell *shell)
{
	char	*result;
	char	*orig_line;

	if (r->heredoc_exp == 0)
		return (line);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	orig_line = line;
	while (*line)
	{
		if (*line == '$')
		{
			result = handle_dollar(&line, result, shell, 0);
			if (!result)
				return (NULL);
			continue ;
		}
		else
			result = append_and_check(*line, result);
		if (!result)
			return (NULL);
		line++;
	}
	return (free(orig_line), result);
}
