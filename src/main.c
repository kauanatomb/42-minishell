/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

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

bool	has_unclosed_quotes(char *line)
{
	bool	single_q;
	bool	double_q;

	single_q = false;
	double_q = false;
	while (*line)
	{
		if (*line == '\'')
			single_q = !single_q;
		else if (*line == '\"')
			double_q = !double_q;
		line++;
	}
	if (single_q || double_q)
	{
		write(2, "unclosed quotes\n", 16);
		return (true);
	}
	return (false);
}

int	ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

bool	is_line_empty(char *line)
{
	int	i;

	if (!line || !*line)
		return (true);
	i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (false);
		i++;
	}
	return (true);
}

int	check_entry(t_shell *shell, char *line)
{
	int	ret;

	ret = lexer(shell, line);
	if (ret != 0)
		return (ret);
	// ret = parse(shell);
	// if (ret != 0)
	// 	return (free_cmd_list(shell->cmds), free_cmd_list(shell->cmds), ret);
	// if (g_signal == 130)
	// {
	// 	shell->error = 130;
	// 	g_signal = 0;
	// }
	// ret = main_expand(shell);
	// if (ret != 0)
	// 	return (free_cmd_list(shell->cmds), free_cmd_list(shell->cmds), ret);
	// return (0);
}

int	read_line(t_shell *shell)
{
	char	*line;

	rl_on_new_line(); // Prepare readline for new input
	line = readline("minishell> ");
	if (!line)
	{
		rl_clear_history(); // Clear previous history
		exit_ctrld(shell);
	}
	if (has_unclosed_quotes(line) || is_line_empty(line))
		return (free(line), 0);
	add_history(line);
	shell->error = check_entry(shell, line);
	if (sell->error)
		return (free(line), 0);
	// more steps: execution and clean
	clean_extra_fds();
	return (free(line), 1);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	shell;

	if (argc != 1)
		return (1);
	init_shell_env(&shell, envp);
	shell.error = 0;
	signal_receiver();
	while (1)
		read_line(&shell);
	rl_clear_history();
	(void)argv;
	return (0);
}
