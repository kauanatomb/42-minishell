/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/29 18:14:28 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	builtin_cd_child(char **args, t_shell *shell)
{
	char	*target;
	int		access;

	if (args[1] && args[2])
		return (p_error_cd(shell, NULL, 2), 1);
	if (!args[1] || !ft_strcmp(args[1], "--"))
		target = give_env_value(shell->env, "HOME=");
	else if (!ft_strcmp(args[1], "-"))
		target = give_env_value(shell->env, "OLDPWD=");
	else
		target = ft_strdup(args[1]);
	if (!target || !*target)
		return (p_error_cd(shell, args[1], 1), free(target), 1);
	access = check_directory_access(target);
	if (access == 1)
		return (p_error_cd(shell, target, 3), free(target), 1);
	else if (access == 2)
		return (p_error_cd(shell, target, 4), free(target), 1);
	free(target);
	return (0);
}
