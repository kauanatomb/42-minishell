/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*build_env(const char *key, const char *value)
{
	char	*entry;
	char	*tmp;

	if (!key || !value)
		return (NULL);
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	entry = ft_strjoin(tmp, value);
	free(tmp);
	if (!entry)
		return (NULL);
	return (entry);
}

void	ft_free_array(char **dest)
{
	int	i;

	i = 0;
	if (!dest)
		return ;
	while (dest[i])
		free(dest[i++]);
	free(dest);
}

static int	create_minimal_env(char **env)
{
	char	cwd[10000];

	if (!getcwd(cwd, sizeof(cwd)))
		return (free(env), 137);
	env[0] = build_env("PWD", cwd);
	if (!env[0])
		return (free(env), 137);
	env[1] = build_env("OLDPWD", cwd);
	if (!env[1])
		return (ft_free_array(env), 137);
	env[2] = ft_strdup("_=/usr/bin/env");
	if (!env[2])
		return (ft_free_array(env), 137);
	env[3] = NULL;
	return (ERR_NONE);
}

static int	copy_envp_to_shell(char **env, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			env[i] = NULL;
			ft_free_array(env);
			return (137);
		}
		i++;
	}
	env[i] = NULL;
	return (ERR_NONE);
}

int	init_shell_env(t_shell *shell, char **envp)
{
	if (ft_array_len(envp) == 0)
	{
		shell->env = malloc(sizeof(char *) * 4);
		if (!shell->env)
			return (137);
		if (create_minimal_env(shell->env))
			return (137);
	}
	else
	{
		shell->env = malloc(sizeof(char *) * (ft_array_len(envp) + 1));
		if (!shell->env)
			return (137);
		if (copy_envp_to_shell(shell->env, envp))
			return (137);
	}
	shell->path_flag = 0;
	return (ERR_NONE);
}
