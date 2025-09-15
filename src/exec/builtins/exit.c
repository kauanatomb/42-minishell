/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/29 19:11:26 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_exit_code(const char *s, long long *code)
{
	long long	res;
	int			sign;

	res = 0;
	sign = 1;
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s++ == '-')
			sign = -1;
	}
	if (!ft_isdigit(*s))
		return (1);
	while (ft_isdigit(*s))
	{
		if (res > (LLONG_MAX - (*s - '0')) / 10)
			return (1);
		res = res * 10 + (*s++ - '0');
	}
	if (*s != '\0')
		return (1);
	*code = res * sign;
	return (0);
}

void	free_program(t_shell *shell)
{
	if (shell->env)
		free_shell_env(shell->env);
	if (shell->cmds)
		free_cmd_list(shell->cmds);
	if (shell->tokens)
		free_tokens(shell->tokens);
	clean_extra_fds();
}

int	builtin_exit(char **argv, t_shell *shell)
{
	long long	code;

	write(STDOUT_FILENO, "exit\n", 5);
	if (!argv[1] || !argv[1][0])
		code = shell->error;
	else if (argv[2])
	{
		write(2, "bash: exit: too many arguments\n",
			strlen("bash: exit: too many arguments\n"));
		return (1);
	}
	else if (parse_exit_code(argv[1], &code))
	{
		write(2, "bash: exit: ", strlen("bash: exit: "));
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": numeric argument required\n",
			strlen(": numeric argument required\n"));
		code = 2;
	}
	shell->error = code;
	free_program(shell);
	exit(code);
}

void	builtin_exit_child(char **argv, t_shell *shell)
{
	long long	code;

	if (!argv[1] || !argv[1][0])
		code = shell->error;
	else if (argv[2])
	{
		write(2, "bash: exit: too many arguments\n", 36);
		exit(1);
	}
	else if (parse_exit_code(argv[1], &code))
	{
		write(2, "bash: exit: ", 17);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": numeric argument required\n", 28);
		exit(2);
	}
	exit((int)code);
}
