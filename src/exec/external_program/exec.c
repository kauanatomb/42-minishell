/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
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
	return (search_in_paths(paths, cmd));
}

void	exec_external_child(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!cmd->argv[0] || !cmd->argv[0][0])
	{
		free_program(shell);
		exit(0);
	}
	if (apply_redirs(cmd->redirs, shell) < 0)
		exit(1);
	cmd_path = find_command_path(cmd->argv[0], shell->env);
	if (!cmd_path)
	{
		if (errno == EACCES)
			handle_permission_denied(cmd->argv[0], shell);
		else
			exit_command_not_found(cmd->argv[0], shell);
	}
	if (is_directory(cmd_path))
		exit_is_directory(cmd_path, shell);
	execve(cmd_path, cmd->argv, shell->env);
	perror(cmd_path);
	free_program(shell);
	free(cmd_path);
	exit(126);
}

int	exec_external(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		signal_happened;

	signal(SIGINT, SIG_IGN);
	signal_happened = 0;
	pid = fork();
	status = 1;
	if (pid == -1)
		return (perror("fork"), signal(SIGINT, handle_sig), ERR_FORK);
	if (pid == 0)
		exec_external_child(cmd, shell);
	status = wait_children(pid, &signal_happened);
	signal(SIGINT, handle_sig);
	handle_pipeline_signal(status, signal_happened);
	return (status);
}
