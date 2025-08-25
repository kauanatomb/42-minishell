/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_array_len(char **arr)
{
	int	size;

	size = 0;
	while (arr && arr[size])
		size++;
	return (size);
}

int	ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

void	clean_extra_fds(void)
{
	int	i;

	i = 3;
	while (i < 1024)
		close(i++);
}

void	free_shell_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
}

int	exit_ctrld(t_shell *shell)
{
	free_shell_env(shell->env);
	write(STDIN_FILENO, "exit\n", 5);
	clean_extra_fds();
	exit(0);
}
