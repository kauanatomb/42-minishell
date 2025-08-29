/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/29 12:10:18 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_char_to_result(char c, char *result)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	result = ft_strjoin_and_free(result, tmp);
	if (!result)
		return (NULL);
	return (result);
}

static int	expand_cmd_argv(t_shell *shell, t_cmd *cmd, int *tok_i)
{
	int		i;
	char	*expanded;

	i = 0;
	while (cmd->argv[i])
	{
		expanded = expand_var(cmd->argv[i], shell);
		if (!expanded)
			return (ERR_MEMORY);
		free(cmd->argv[i]);
		cmd->argv[i] = expanded;
		i++;
		(*tok_i)++;
	}
	return (ERR_NONE);
}

static int	expand_cmd_redirs(t_shell *shell, t_cmd *cmd, int *tok_i)
{
	t_redir	*r;
	char	*expanded;

	r = cmd->redirs;
	while (r)
	{
		if (r->type != HEREDOC && r->filename)
		{
			expanded = expand_var(r->filename, shell);
			if (!expanded)
				return (ERR_MEMORY);
			free(r->filename);
			r->filename = expanded;
		}
		r = r->next;
		(*tok_i)++;
	}
	return (ERR_NONE);
}

int	main_expand(t_shell *shell)
{
	t_cmd	*cmd;
	int		tok_i;

	cmd = shell->cmds;
	tok_i = 0;
	while (cmd)
	{
		if (expand_cmd_argv(shell, cmd, &tok_i) != ERR_NONE)
			return (ERR_MEMORY);
		if (expand_cmd_redirs(shell, cmd, &tok_i) != ERR_NONE)
			return (ERR_MEMORY);
		cmd = cmd->next;
	}
	return (ERR_NONE);
}
