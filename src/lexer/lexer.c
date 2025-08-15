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

void skip_operator(const char *line, int *i)
{
    if ((line[*i] == '<' && line[*i + 1] == '<') ||
        (line[*i] == '>' && line[*i + 1] == '>'))
        *i += 2;
    else
        (*i)++;
}

int ft_is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>' || c == '&');
}

void skip_word(const char *line, int *i)
{
    char    quote;

    while (line[*i] && !ft_isspace(line[*i]) && !ft_is_operator(line[*i]))
    {
        if (line[*i] == '\'' || line[*i] == '"')
        {
            quote = line[(*i)++];
            while (line[*i] && line[*i] != quote)
                (*i)++;
            if (line[*i] == quote)
                (*i)++;
        }
        else
            (*i)++;
    }
}

static int count_tokens(const char *line)
{
    int i = 0, count = 0;
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
    return count;
}

int lex_line(t_shell *shell, const char *line)
{
    int token_count;

    token_count = count_tokens(line);
    shell->num_tokens = token_count;
    shell->tokens = malloc(sizeof(t_token) * (token_count + 1));
    if (!shell->tokens)
        return (1);
    if (tokenize_all(shell, line))
        return (1);
    return (0);
}
