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
		free_tokens(shell->tokens);
		free_cmd_list(shell->cmds);
		return (ret);
	}
	// verify the g_signal == 130, why??
	int i = 0;
	int j;
	int y = 0;
	t_redir *last;
	t_cmd *curr = shell->cmds;
	while (curr)
	{
		j = 0;
		printf("Command[%d]: argv list: ", i);
		while (curr->argv[j])
		{
			printf("%s, ", curr->argv[j]);
			j++;
		}
		printf("\n");
		last = curr->redirs;
		while (last)
		{
			printf("Redirect[%d], fd[%d], name: %s, type: %d \n", y, last->fd, last->filename, last->type);
			y++;
			last = last->next;
		}
		i++;
		curr = curr->next;
	}
	printf("Total cmd: %d\n", i);
	ret = main_expand(shell);
	if (ret != ERR_NONE)
	{
		free_tokens(shell->tokens);
		free_cmd_list(shell->cmds);
		return (ret);
	}
	return (0);
}

int	read_line(t_shell *shell)
{
	char	*line;

	rl_on_new_line();
	line = readline("minishell> ");
	if (!line)
	{
		rl_clear_history();
		exit_ctrld(shell);
	}
	if (has_unclosed_quotes(line) || is_line_empty(line))
		return (free(line), 0);
	add_history(line);
	shell->error = check_entry(shell, line);
	if (shell->error)
		return (free(line), 0);
	// more steps: check redir and execution
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
