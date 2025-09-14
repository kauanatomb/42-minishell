/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_n_flag(char *arg)
{
	int	i;

	if (arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	if (i == 1)
		return (0);
	return (1);
}

int	builtin_echo(char **argv)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (argv[i] && is_valid_n_flag(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (0);
}

int	builtin_pwd(t_shell *shell)
{
	char	*cwd;
	char	buf[1024];

	cwd = getcwd(buf, sizeof(buf));
	if (!cwd)
	{
		shell->error = 1;
		write(2, "pwd: error retrieving current directory: ", 41);
		write(2, "getcwd: cannot access parent directories:", 41);
		write(2, " No such file or directory\n", 28);
		return (1);
	}
	write(STDOUT_FILENO, cwd, ft_strlen(cwd));
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int	builtin_env(char **env)
{
	int		i;
	char	*ptr;

	i = 0;
	while (env[i])
	{
		ptr = ft_strrchr(env[i], '=');
		if (ptr)
			ft_printf("%s\n", env[i]);
		i++;
	}
	return (0);
}
