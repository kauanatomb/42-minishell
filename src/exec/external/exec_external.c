/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_path_from_env(char **envp)
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

static char	*search_in_paths(char **paths, char *cmd)
{
	int		i;
	char	*full_path;
	char	*tmp;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		tmp = full_path;
		full_path = ft_strjoin(full_path, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_array(paths);
	return (NULL);
}

static char	*find_command_path(char *cmd, char **env)
{
	char	*path_env;
	char	**paths;

	path_env = get_path_from_env(env);
	if (!path_env)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	paths = ft_split(path_env, ':');
	return (search_in_paths(paths, cmd));
}

static void	exec_external_child(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;

	if (apply_redirs(cmd->redirs) < 0)
		exit(1);
	cmd_path = find_command_path(cmd->argv[0], shell->env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execve(cmd_path, cmd->argv, shell->env);
	perror(cmd_path);
	free(cmd_path);
	exit(126);
}

int	exec_external(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	status = 1;
	if (pid == -1)
		return (perror("fork"), status);
	if (pid == 0)
		exec_external_child(cmd, shell);
	if (waitpid(pid, &status, 0) == -1)
		return (perror("waitpid"), status);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (status);
}
