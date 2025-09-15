/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/29 19:11:26 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_error_exp(t_shell *shell, char *arg)
{
	write(2, "bash: export: `", 15);
	write(2, arg, strlen(arg));
	write(2, "': not a valid identifier\n", 26);
	shell->error = 1;
	return (shell->error);
}

bool	is_valid_key(char *key)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(key[0]) || key[0] == '_'))
		return (false);
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

int	builtin_export_print(t_shell *shell)
{
	int		i;
	char	*eq;

	i = 0;
	while (shell->env[i])
	{
		eq = ft_strchr(shell->env[i], '=');
		if (eq)
		{
			*eq = '\0';
			ft_printf("declare -x %s=\"%s\"\n", shell->env[i], eq + 1);
			*eq = '=';
		}
		else
			ft_printf("declare -x %s\n", shell->env[i]);
		i++;
	}
	return (0);
}

int	process_export_arg(char *arg, t_shell *shell)
{
	char	**splitted;
	int		idx;

	splitted = split_key_value_env(arg);
	if (!splitted)
		return (ERR_MEMORY);
	if (!is_valid_key(splitted[0]))
	{
		print_error_exp(shell, arg);
		ft_free_array(splitted);
		return (0);
	}
	idx = is_created(splitted[0], shell->env);
	if (idx >= 0)
		change_value(splitted[0], splitted[1], shell->env, idx);
	else
	{
		if (create_env(splitted[0], splitted[1], &shell->env) != 0)
			return (ft_free_array(splitted), ERR_MEMORY);
	}
	ft_free_array(splitted);
	return (0);
}

int	builtin_export(char **argv, t_shell *shell)
{
	int		i;

	i = 1;
	shell->error = 0;
	if (!argv[i] || is_line_empty(argv[i]))
		return (builtin_export_print(shell));
	while (argv[i])
	{
		if (process_export_arg(argv[i], shell) == ERR_MEMORY)
			return (ERR_MEMORY);
		i++;
	}
	return (shell->error);
}
