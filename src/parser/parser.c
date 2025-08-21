/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//i need to treat errors

int count_words(t_shell *shell, int start)
{
    int count;

    count = 0;
    while (start < shell->num_tokens && shell->tokens[start].type != PIPE)
    {
        if (shell->tokens[start].type == WORD)
            count++;
        start++;
    }
    return (count);
}

t_cmd   *new_cmd(int counted_words)
{
    t_cmd   *cmd;
    int     i;

    cmd = malloc(sizeof(t_cmd));
    cmd->argv = malloc(sizeof(char *) * (counted_words + 1));
    if (!cmd->argv)
        return (NULL);
    i = 0;
    while (i <= counted_words)
        cmd->argv[i++] = NULL;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->append = 0;
    cmd->next = NULL;
    return (cmd);
}
// argv ["word1", "word2" ...]
int add_to_argv(t_cmd *cmd, char *word, int pos)
{
    cmd->argv[pos] = ft_strdup(word);
    if (!cmd->argv[pos])
        return (ERR_PARSE);
    return (ERR_NONE);
}

int parse(t_shell *shell)
{
    t_cmd *head;
    t_cmd *curr;
    int     counted_words;

    counted_words = count_words(shell, 0);
    head = new_cmd(counted_words);
    if (!head)
        return (ERR_PARSE);
    curr = head;
    int i = 0;
    int start = 0;
    while (i < shell->num_tokens)
    {
        if (shell->tokens[i].type == WORD)
        {
            add_to_argv(curr, shell->tokens[i].value, start);
            start++;
        }
        else if (shell->tokens[i].type == INPUT)
        {
            i++;
            if (!shell->tokens[i].value)
                return (ERR_PARSE);
            curr->infile = ft_strdup(shell->tokens[i].value);
        }
        else if (shell->tokens[i].type == OUTPUT || shell->tokens[i].type == APPEND)
        {
            int is_append = (shell->tokens[i].type == APPEND);
            i++;
            if (!shell->tokens[i].value)
                return (ERR_PARSE);
            curr->outfile = ft_strdup(shell->tokens[i].value);
            curr->append = is_append;
        }
        else if (shell->tokens[i].type == PIPE)
        {
            if (start == 0 || (shell->tokens[i + 1].type == PIPE)
                || !shell->tokens[i + 1].value)
                return (ERR_PARSE);
            curr->next = new_cmd(count_words(shell, i));
            if (!curr->next)
                return (ERR_PARSE);
            curr = curr->next;
            start = 0;
        }
        i++;
    }
    shell->cmds = head;
    return (ERR_NONE);
}
