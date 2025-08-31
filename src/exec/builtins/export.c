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

int	change_value(char *key, char *value, char **env, int i)
{
	char	*new_env;
	char	*tmp;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (ERR_MEMORY);
	new_env = ft_strjoin(tmp, value);
	free(tmp);
	if (!new_env)
		return (ERR_MEMORY);
	free(env[i]);
	env[i] = new_env;
	return (0);
}

int	create_env(char *key, char *value, char ***env)
{
	int		len;
	char	*tmp;
	char	*l_env;
	char	**new_env;

	len = ft_array_len(*env);
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (ERR_MEMORY);
	l_env = ft_strjoin(tmp, value);
	free(tmp);
	if (!l_env)
		return (ERR_MEMORY);
	new_env = malloc(sizeof(char *) * (len + 2));
	if (!new_env)
		return (free(l_env), ERR_MEMORY);
	return (add_to_env(new_env, env, l_env, len));
}

static int	print_error_exp(t_shell *shell, char *arg)
{
	write(2, "bash: export: `", 15);
	write(2, arg, strlen(arg));
	write(2, "': not a valid identifier\n", 26);
	shell->error = 1;
	return (shell->error);
}

int	is_valid_key(char *key)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(key[0]) || key[0] == '_'))
		return (0);
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_export(char **argv, t_shell *shell)
{
	int		i;
	char	**splitted;
	int		idx;

	i = 1;
	shell->error = 0;
	if (!argv[i] || is_line_empty(argv[1]))
		return (0);
	while (argv[i])
	{
		splitted = split_key_value_env(argv[i]);
		if (!splitted)
			return (ERR_MEMORY);
		if (!is_valid_key(splitted[0]))
			return (ft_free_array(splitted), print_error_exp(shell, argv[i]));
		idx = is_created(splitted[0], shell->env);
		if (idx >= 0)
			change_value(splitted[0], splitted[1], shell->env, idx);
		else if (create_env(splitted[0], splitted[1], &shell->env) != 0)
			return (ft_free_array(splitted), ERR_MEMORY);
		free(splitted[0]);
		free(splitted[1]);
		i++;
	}
	return (shell->error);
}
