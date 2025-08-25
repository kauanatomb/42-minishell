/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *extract_key(char *p)
{
    int     i;

    i = 0;
    if (!ft_isalpha(p[0]) && p[0] != '_')
        return (NULL);
    while ((p[i] && ft_isalnum(p[i])) || p[i] == '_')
        i++;
    return (ft_substr(p, 0, i));
}

char    *getenv_from_envp(const char *key, char **env)
{
    int     i;
    int     len;

    len = ft_strlen(key);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
            return (env[i] + len + 1);
        i++;
    }
    return (NULL);
}

char *expand_var(char *str, t_shell *shell)
{
    char    *result;
    char    *p;
    char    *key;
    char    *value;
    char    tmp[2];

    result = ft_strdup("");
    if (!result)
        return (NULL);
    p = str;
    while (*p)
    {
        if (*p == '$')
        {
            p++;
            // special case: $?
            if (*p == '?')
            {
                value = ft_itoa(shell->error); // to check status
                result = ft_strjoin_and_free(result, value);
                p++;
                continue;
            }
            // extract key (ex: PATH, HOME)
            key = extract_key(p);
            if (key)
            {
                value = getenv_from_envp(key, shell->env);
                if (value)
                    result = ft_strjoin_and_free(result, value);
                p += ft_strlen(key);
                free(key);
            }
            else
            {
                // copy literal
                p++;
                tmp[0] = *p;
                tmp[1] = '\0';
                result = ft_strjoin_and_free(result, tmp);
                p++;
            }
        }
        else
        {
            // copy literal
            tmp[0] = *p;
            tmp[1] = '\0';
            result = ft_strjoin_and_free(result, tmp);
            p++;
        }
    }
    return (result);
}

int main_expand(t_shell *shell)
{
    t_cmd   *cmd;
    int     tok_i;
    int     i;
    char    *expanded;
    t_redir *r;

    cmd = shell->cmds;
    tok_i = 0;
    while (cmd)
    {
        i = 0;
        while (cmd->argv[i])
        {
            if (shell->tokens[tok_i].quote_type != SINGLE)
            {
                expanded = expand_var(cmd->argv[i], shell);
                free(cmd->argv[i]);
                cmd->argv[i] = expanded;
            }
            i++;
            tok_i++;
        }
        r = cmd->redirs;
        while (r)
        {
            if (r->type != HEREDOC && r->filename)
            {
                expanded = expand_var(r->filename, shell);
                free(r->filename);
                r->filename = expanded;
            }
            r = r->next;
            tok_i++;
        }
        cmd = cmd->next;
    }
    return (ERR_NONE);
}