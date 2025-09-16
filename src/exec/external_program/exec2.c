/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path_from_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*search_in_paths(char **paths, char *cmd)
{
	int		i;
	char	*full_path;
	char	*joined;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		if (!full_path)
			return (ft_free_array(paths), NULL);
		joined = ft_strjoin(full_path, cmd);
		free(full_path);
		if (!joined)
			return (ft_free_array(paths), NULL);
		if (access(joined, X_OK) == 0)
			return (ft_free_array(paths), joined);
		free(joined);
		i++;
	}
	ft_free_array(paths);
	return (NULL);
}

char	*find_command_path(char *cmd, char **env)
{
	char		*path_env;
	char		**paths;
	struct stat	st;

	path_env = get_path_from_env(env);
	if (!path_env)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (stat(cmd, &st) == 0)
		{
			if (S_ISDIR(st.st_mode))
				return (ft_strdup(cmd));
			if (access(cmd, X_OK) == 0)
				return (ft_strdup(cmd));
			errno = EACCES;
			return (NULL);
		}
		errno = ENOENT;
		return (NULL);
	}
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd));
}
