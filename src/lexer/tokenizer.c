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

void ft_free_tokens(t_token *tokens)
{
    int i;

    if (!tokens)
        return ;
    i = 0;
    while (tokens[i].value)
    {
        free(tokens[i].value);
        i++;
    }
    free(tokens);
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

char *ft_strndup(const char *s, size_t n)
{
    char *dup;

    if (!s)
        return NULL;
    dup = malloc(n + 1);
    if (!dup)
        return NULL;
    ft_strlcpy(dup, s, n + 1);
    return dup;
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
            {
                ft_free_tokens(shell->tokens);
                return (1);
            }
            if (ft_is_operator(shell->tokens[j].value[0]))
                shell->tokens[j].type = classify_operator(shell->tokens[j].value, len);
            else
                shell->tokens[j].type = WORD;
            j++;
        }
    }
    shell->tokens[j].value = NULL;
    shell->tokens[j].type = -1;
    return (0);
}
