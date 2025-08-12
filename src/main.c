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

void	clean_extra_fds(void)
{
	int	i;

	i = 3;
	while (i < 1024)
		close(i++);
}

int	exit_ctrld(t_shell *shell)
{
	free_shell_resources(shell); // implement
	write(STDIN_FILENO, "exit\n", 5);
	clean_extra_fds();
	exit(0);
}

int	has_unclosed_quotes(char *line)
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
		write(STDERR_FILENO, "unclosed quotes\n", 17);
		return (1);
	}
	return (0);
}

int	ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

int	is_line_empty(char *line)
{
	int	i;

	if (!line || !*line)
		return (1);
	i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (0);
		i++;
	}
	return (1);
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
	return (1);
}

int	init_shell(t_shell *shell, char **envp)
{
	if (/*function to count array len */)
	{
		shell->env = malloc(sizeof(char *) * 4);
		if (!shell->env)
			return (137);
		if (/*call function to create env, if fail return*/)
			return (137);
	}
	else
	{
		shell->env = malloc(sizeof(char *) * /*function t count array len*/ + 1);
		if (!shell->env)
			return (137);
		if (/*call function to create env, if fail return*/)
			return (137);
	}
	shell->path_flag = 0;
	return (0);
}

void	signal_receiver(void)
{
	
}

int	main(int argc, char *argv[], char **envp)
{
	t_shell	shell;

	if (argc != 1)
		return (1);
	init_shell(&shell, envp);
	shell.error = 0;
	signal_receiver();
	while (1)
		read_line(&shell);
	rl_clear_history();
	(void)argv;
	return (0);
}
