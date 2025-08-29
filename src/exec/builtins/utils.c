/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/29 18:14:28 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **split_key_value_env(char *s)
{
    char **d;
    char *equal;

    equal = ft_strchr(s, '=');
    d = malloc(sizeof(char *) * 3);
    if (!d)
        return (NULL);
    if (equal)
        d[0] = ft_substr(s, 0, equal - s);
    else
        d[0] = ft_strdup(s);
    if (!d[0])
        return (free(d), NULL);
    if (!equal)
        d[1] = ft_strdup("");
    else
        d[1] = ft_substr(equal + 1, 0, ft_strlen(equal + 1));
    if (!d[1])
        return (free(d[0]), free(d), NULL);
    d[2] = NULL;
    return (d);
}

int	is_created(char *key, char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (-1);
	while (env[i])
	{
        if (!ft_strncmp(env[i], key, ft_strlen(key)) && env[i][ft_strlen(key)] == '=')
            return (i);
		i++;
	}
	return (-1);
}

int	add_to_env(char **new_env, char ***env, char *l_env, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[len] = l_env;
	new_env[len + 1] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}
