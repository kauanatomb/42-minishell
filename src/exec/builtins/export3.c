/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/29 19:11:26 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_export_child(char **argv, t_shell *shell)
{
	int		i;
	char	**splitted;

	i = 1;
	shell->error = 0;
	if (!argv[i] || is_line_empty(argv[1]))
		return (builtin_export_print(shell));
	while (argv[i])
	{
		splitted = split_key_value_env(argv[i]);
		if (!splitted)
			return (ERR_MEMORY);
		if (!is_valid_key(splitted[0]))
		{
			print_error_exp(shell, argv[i]);
			ft_free_array(splitted);
			i++;
			continue ;
		}
		ft_free_array(splitted);
		i++;
	}
	return (shell->error);
}
