/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
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

static t_token_type classify_operator(const char *s, int len)
{
    if (len == 1 && s[0] == '|')
        return PIPE;
    if (len == 1 && s[0] == '>')
        return OUTPUT;
    if (len == 1 && s[0] == '<')
        return INPUT;
    if (len == 2 && s[0] == '>' && s[1] == '>')
        return APPEND;
    if (len == 2 && s[0] == '<' && s[1] == '<')
        return HEREDOC;
    return WORD;
}

int tokenize_all(t_shell *shell, const char *line)
{
    int i;
    int j;
    int start;
    int len;

    i = 0;
    j = 0;
    while (line[i])
    {
        if (ft_isspace(line[i]))
            i++;
        else
        {
            start = i;
            if (ft_is_operator(line[i]))
                skip_operator(line, &i);
            else
                skip_word(line, &i);
            len = i - start;
            shell->tokens[j].value = ft_strndup(&line[start], len);
            if (!shell->tokens[j].value)
                return (ERR_LEX);
            if (ft_is_operator(shell->tokens[j].value[0]))
                shell->tokens[j].type = classify_operator(shell->tokens[j].value, len);
            else
                shell->tokens[j].type = WORD;
            j++;
        }
    }
    shell->tokens[j].value = NULL;
    shell->tokens[j].type = -1;
    return (ERR_NONE);
}
