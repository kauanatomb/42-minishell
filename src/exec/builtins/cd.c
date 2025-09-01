/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/29 18:08:31 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*give_env_value(char **env, char *key)
{
	int	i;
	int	len;

	len = ft_strlen(key);
	i = 0;
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], key, len))
			return (ft_strdup(env[i] + len));
		i++;
	}
	return (NULL);
}

static void	p_error_cd(t_shell *shell, char *s, int type)
{
	if (type == 1)
	{
		write(2, "bash: cd: HOME not set\n", 23);
		shell->error = 1;
	}
	else if (type == 2)
	{
		write(2, "bash: cd: too many arguments\n", 29);
		shell->error = 1;
	}
	else if (type == 3)
	{
		shell->error = 1;
		write(2, "bash: cd: ", 10);
		write(2, s, ft_strlen(s));
		write(2, ": No such file or directory\n", 28);
	}
	else if (type == 4)
	{
		shell->error = 1;
		write(2, "bash: cd: ", 10);
		write(2, s, ft_strlen(s));
		write(2, ": Not a directory\n", 18);
	}
}

int	update_env_var(char *key, char *value, char ***env)
{
	int	idx;

	idx = is_created(key, *env);
	if (idx >= 0)
		return (change_value(key, value, *env, idx));
	return (create_env(key, value, env));
}

int	check_directory_access(char *arg)
{
	struct stat	info;

	if (!arg)
		return (0);
	else if (stat(arg, &info) == 0 && S_ISDIR(info.st_mode))
		return (0);
	else if (stat(arg, &info) == 0 && !S_ISDIR(info.st_mode))
		return (2);
	else
		return (1);
}

static int	cd_to_target(t_shell *shell, char *target, char *oldpwd)
{
	char	cwd[10000];
	int		access;

	access = check_directory_access(target);
	if (access == 1)
		return (p_error_cd(shell, target, 3), 1);
	else if (access == 2)
		return (p_error_cd(shell, target, 4), 1);
	if (chdir(target) == -1)
		return (p_error_cd(shell, target, 3), 1);
	update_env_var("OLDPWD", oldpwd, &shell->env);
	if (getcwd(cwd, sizeof(cwd)))
		update_env_var("PWD", cwd, &shell->env);
	return (0);
}

int	builtin_cd(char **args, t_shell *shell)
{
	char	cwd[10000];
	char	*oldpwd;
	char	*target;
	int		ret;

	if (!args[1] || !ft_strcmp(args[1], "--"))
		target = give_env_value(shell->env, "HOME=");
	else if (!ft_strcmp(args[1], "-"))
		target = give_env_value(shell->env, "OLDPWD=");
	else if (args[2])
		return (p_error_cd(shell, NULL, 2), 1);
	else
		target = ft_strdup(args[1]);
	if (!target || !*target)
		return (p_error_cd(shell, args[1], 1), free(target), 1);
	if (getcwd(cwd, sizeof(cwd)))
		oldpwd = ft_strdup(cwd);
	else
		oldpwd = give_env_value(shell->env, "PWD=");
	ret = cd_to_target(shell, target, oldpwd);
	free(target);
	free(oldpwd);
	return (ret);
}
