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

char	*expand_heredoc(char *delimiter, t_redir *r)
{
	int		i;
	int		j;
	char	*exp_delimiter;
	char	quote_type;

	i = 0;
	j = 0;
	quote_type = 0;
	exp_delimiter = malloc(ft_strlen(delimiter) + 1);
	while (delimiter[i])
	{
		if ((delimiter[i] == '\'' && !quote_type)
			|| (delimiter[i] == '"' && !quote_type))
			quote_type = delimiter[i];
		if (delimiter[i] != quote_type || !quote_type)
			exp_delimiter[j++] = delimiter[i];
		i++;
	}
	if (!quote_type)
		r->heredoc_exp = 1;
	exp_delimiter[j] = 0;
	return (exp_delimiter);
}

int	expand_cmd_redirs(t_shell *shell, t_cmd *cmd, int *tok_i)
{
	t_redir	*r;
	int		ret;

	r = cmd->redirs;
	while (r)
	{
		if (r->filename && r->type != HEREDOC)
			ret = expand_filename(r, shell);
		else if (r->delimiter && r->type == HEREDOC)
			ret = expand_delimiter(r);
		else
			ret = ERR_NONE;
		if (ret != ERR_NONE)
			return (ret);
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
