/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
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
	int		i;
	t_cmd	*curr;
	t_cmd	*next;

	if (!cmds)
		return ;
	curr = cmds;
	while (curr)
	{
		next = curr->next;
		if (curr->argv)
		{
			i = 0;
			while (curr->argv[i])
				free(curr->argv[i++]);
			free(curr->argv);
		}
		free(curr->infile);
		free(curr->outfile);
		free(curr);
		curr = next;
	}
}

void	print_parse_error(t_error code, t_token *token)
{
	if (code == ERR_UNEXPECTED_TOKEN && token)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(token->value, 2);
		ft_putstr_fd("'\n", 2);
	}
	else if (code == ERR_MISSING_FILENAME)
	{
		ft_putstr_fd("minishell: syntax error near unexpected ", 2);
		ft_putstr_fd("token `newline'\n", 2);
	}
	else if (code == ERR_MEMORY)
		ft_putstr_fd("minishell: memory allocation error\n", 2);
	else if (code == ERR_PARSE)
		ft_putstr_fd("minishell: parse error\n", 2);
}
