/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/29 19:11:26 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_error_unset(t_shell *shell, char *arg)
{
	write(2, "bash: unset: `", 15);
	write(2, arg, strlen(arg));
	write(2, "': not a valid identifier\n", 26);
	shell->error = 1;
	return (shell->error);
}

static int	delete_key(char ***env, int idx)
{
	char	**new_env;
	int		i;
	int		len;
	int		j;

	len = ft_array_len(*env);
	new_env = malloc(sizeof(char *) * (len + 1));
	if (!new_env)
		return (ERR_MEMORY);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (i != idx)
			new_env[j++] = (*env)[i];
		else
			free((*env)[i]);
		i++;
	}
	new_env[j] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}

int	builtin_unset(char **argv, t_shell *shell)
{
	int		i;
	char	*key;
	int		idx;

	i = 1;
	shell->error = 0;
	if (!argv[i] || is_line_empty(argv[1]))
		return (0);
	while (argv[i])
	{
		key = argv[i];
		if (!is_valid_key(key))
			print_error_unset(shell, key);
		else
		{
			idx = is_created(key, shell->env);
			if (idx >= 0)
				delete_key(&shell->env, idx);
		}
		i++;
	}
	return (shell->error);
}

int	builtin_unset_child(char **argv, t_shell *shell)
{
	int		i;
	char	*key;

	i = 1;
	shell->error = 0;
	if (!argv[i] || is_line_empty(argv[1]))
		return (0);
	while (argv[i])
	{
		key = argv[i];
		if (!is_valid_key(key))
			print_error_unset(shell, key);
		i++;
	}
	return (shell->error);
}
