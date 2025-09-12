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

bool	has_unclosed_quotes(char *line)
{
	bool	single_q;
	bool	double_q;

	single_q = false;
	double_q = false;
	while (*line)
	{
		if (*line == '\'' && !double_q)
			single_q = !single_q;
		else if (*line == '\"' && !single_q)
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

	ret = lex_line(shell, line);
	if (ret != ERR_NONE)
		return (print_parse_error(ret, NULL), ERR_MEMORY);
	ret = parse(shell);
	if (ret != ERR_NONE)
	{
		free_cmd_list(shell->cmds);
		free_tokens(shell->tokens);
		return (ret);
	}
	if (g_signal == 130)
	{
		shell->error = 130;
		g_signal = 0;
	}
	ret = main_expand(shell);
	if (ret != ERR_NONE)
	{
		free_cmd_list(shell->cmds);
		free_tokens(shell->tokens);
		return (ret);
	}
	return (0);
}

int	read_line(t_shell *shell)
{
	char	*line;
	t_cmd	*tmp;

	rl_on_new_line();
	line = readline("minishell> ");
	if (!line)
		exit_ctrld(shell);
	if (has_unclosed_quotes(line) || is_line_empty(line))
		return (free(line), 0);
	add_history(line);
	shell->error = check_entry(shell, line);
	if (shell->error)
		return (free(line), 0);
	tmp = shell->cmds;
	while (tmp)
	{
		clean_argv_empty_cmds(tmp);
		tmp = tmp->next;
	}
	exec_cmd(shell->cmds, shell);
	free_cmd_list(shell->cmds);
	free_tokens(shell->tokens);
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
