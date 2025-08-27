/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/26 12:22:12 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_key(char *p)
{
	int	i;

	i = 0;
	if (!ft_isalpha(p[0]) && p[0] != '_')
		return (NULL);
	while (p[i] && (ft_isalnum(p[i]) || p[i] == '_'))
		i++;
	return (ft_substr(p, 0, i));
}

static char	*getenv_from_envp(const char *key, char **env)
{
	int	i;
	int	len;

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

static char	*append_env_value(char *key, t_shell *shell, char *result)
{
	char	*value;

	value = getenv_from_envp(key, shell->env);
	if (value)
	{
		result = ft_strjoin_and_free(result, value);
		if (!result)
			return (NULL);
	}
	return (result);
}

char	*expand_key_var(char **str, t_shell *shell, char *result)
{
	char	*key;

	key = extract_key(*str);
	if (key)
	{
		result = append_env_value(key, shell, result);
		if (!result)
			return (free(key), NULL);
		(*str) += ft_strlen(key);
		free(key);
	}
	else
	{
		result = append_char_to_result(**str, result);
		if (!result)
			return (NULL);
		(*str)++;
	}
	return (result);
}

char	*expand_dollar_question(t_shell *shell, char *result)
{
	char	*value;

	value = ft_itoa(shell->error);
	if (!value)
		return (NULL);
	result = ft_strjoin_and_free(result, value);
	if (!result)
		return (free(value), NULL);
	return (free(value), result);
}
