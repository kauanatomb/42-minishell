/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_tokens(const char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]))
			i++;
		else if (ft_is_operator(line[i]))
		{
			skip_operator(line, &i);
			count++;
		}
		else
		{
			skip_word(line, &i);
			count++;
		}
	}
	return (count);
}

static char	*strip_quotes(const char *src, int *quote_type)
{
	size_t	len;

	len = ft_strlen(src);
	if (quote_type)
		*quote_type = 0;
	if (len >= 2
		&& ((src[0] == '\'' && src[len - 1] == '\'')
			|| (src[0] == '"' && src[len - 1] == '"')))
	{
		if (quote_type && src[0] == '\'')
			*quote_type = 1;
		else
			*quote_type = 2;
		return (ft_substr(src, 1, len - 2));
	}
	return (ft_strdup(src));
}

static int	normalize_tokens(t_shell *shell)
{
	int		i;
	char	*clean;
	int		quote_type;

	i = 0;
	while (shell->tokens[i].value)
	{
		clean = strip_quotes(shell->tokens[i].value, &quote_type);
		if (!clean)
			return (ERR_MEMORY);
		free(shell->tokens[i].value);
		shell->tokens[i].value = clean;
		shell->tokens[i].quote_type = quote_type;
		i++;
	}
	return (ERR_NONE);
}

int	lex_line(t_shell *shell, const char *line)
{
	int	token_count;

	token_count = count_tokens(line);
	shell->num_tokens = token_count;
	shell->tokens = malloc(sizeof(t_token) * (token_count + 1));
	if (!shell->tokens)
		return (ERR_MEMORY);
	if (tokenize_all(shell, line) || normalize_tokens(shell))
	{
		free_tokens(shell->tokens);
		return (ERR_MEMORY);
	}
	return (ERR_NONE);
}
