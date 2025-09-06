/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/21 18:29:37 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_operator(const char *line, int *i)
{
	if ((line[*i] == '<' && line[*i + 1] == '<')
		|| (line[*i] == '>' && line[*i + 1] == '>'))
		*i += 2;
	else
		(*i)++;
}

t_token_type	classify_operator(const char *s, int len)
{
	if (len == 1 && s[0] == '|')
		return (PIPE);
	if (len == 1 && s[0] == '>')
		return (OUTPUT);
	if (len == 1 && s[0] == '<')
		return (INPUT);
	if (len == 2 && s[0] == '>' && s[1] == '>')
		return (APPEND);
	if (len == 2 && s[0] == '<' && s[1] == '<')
		return (HEREDOC);
	return (WORD);
}

static int	add_token(t_token *tokens, int *j, const char *start, int len)
{
	tokens[*j].value = ft_strndup(start, len);
	if (!tokens[*j].value)
		return (ERR_MEMORY);
	if (ft_is_operator(tokens[*j].value[0]))
		tokens[*j].type = classify_operator(tokens[*j].value, len);
	else
		tokens[*j].type = WORD;
	(*j)++;
	return (ERR_NONE);
}

static int	process_token(t_token *tokens, int *j, const char *line, int *i)
{
	int	start;

	start = *i;
	if (ft_is_operator(line[*i]))
		skip_operator(line, i);
	else
		skip_word(line, i);
	return (add_token(tokens, j, &line[start], *i - start));
}

int	tokenize_all(t_shell *shell, const char *line)
{
	int	i;
	int	j;
	int	err;

	i = 0;
	j = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]))
			i++;
		else
		{
			err = process_token(shell->tokens, &j, line, &i);
			if (err)
				return (err);
		}
	}
	shell->tokens[j].value = NULL;
	shell->tokens[j].type = -1;
	return (ERR_NONE);
}
