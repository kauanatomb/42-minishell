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
		fprintf(stderr,
			"minishell: syntax error near unexpected token `%s'\n",
			token->value);
	}
	else if (code == ERR_MISSING_FILENAME)
	{
		fprintf(stderr,
			"minishell: syntax error near unexpected token `newline'\n");
	}
	else if (code == ERR_MEMORY)
	{
		fprintf(stderr, "minishell: memory allocation error\n");
	}
	else if (code == ERR_PARSE)
	{
		fprintf(stderr, "minishell: parse error\n");
	}
}
