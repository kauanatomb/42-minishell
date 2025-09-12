/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_list(t_cmd *cmds)
{
	t_cmd	*next;
	t_redir	*next_r;
	t_redir	*last;

	while (cmds)
	{
		next = cmds->next;
		ft_free_array(cmds->argv);
		if (cmds->redirs)
		{
			last = cmds->redirs;
			while (last)
			{
				next_r = last->next;
				free(last->filename);
				free(last);
				last = next_r;
			}
		}
		free(cmds);
		cmds = next;
	}
}

void	print_parse_error(t_error code, t_token *token)
{
	if (code == ERR_UNEXPECTED_TOKEN && token)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		ft_putstr_fd(token->value, 2);
		ft_putstr_fd("'\n", 2);
	}
	else if (code == ERR_MISS_FILENAME)
	{
		ft_putstr_fd("bash: syntax error near unexpected ", 2);
		ft_putstr_fd("token `newline'\n", 2);
	}
	else if (code == ERR_MEMORY)
		ft_putstr_fd("bash: memory allocation error\n", 2);
	else if (code == ERR_PARSE)
		ft_putstr_fd("bash: parse error\n", 2);
}

int	countword(t_shell *shell, int start)
{
	int	count;

	count = 0;
	while (start < shell->num_tokens && shell->tokens[start].type != PIPE)
	{
		if (shell->tokens[start].type == WORD)
			count++;
		start++;
	}
	return (count);
}

t_cmd	*new_cmd(int counted_words)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = ft_calloc(counted_words + 1, sizeof(char *));
	if (!cmd->argv)
		return (free(cmd), NULL);
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}
