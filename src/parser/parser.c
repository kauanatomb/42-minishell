/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/21 18:42:49 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_word(t_cmd *cmd, char *word, int *pos, int max_words)
{
	if (*pos >= max_words)
		return (print_parse_error(ERR_PARSE, NULL), ERR_PARSE);
	cmd->argv[*pos] = ft_strdup(word);
	if (!cmd->argv[*pos])
		return (print_parse_error(ERR_MEMORY, NULL), ERR_MEMORY);
	(*pos)++;
	return (ERR_NONE);
}

static int	handle_input(t_shell *shell, t_cmd *curr, int *i)
{
	t_redir	*redir;
	t_redir	*last;

	(*i)++;
	if (*i >= shell->num_tokens || shell->tokens[*i].type != WORD)
		return (print_parse_error(ERR_MISS_FILENAME, NULL), ERR_MISS_FILENAME);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (print_parse_error(ERR_MEMORY, NULL), ERR_MEMORY);
	redir->fd = 0; // stdin
	redir->type = shell->tokens[(*i) - 1].type;
	redir->filename = ft_strdup(shell->tokens[*i].value);
	if (!redir->filename)
		return (free(redir), print_parse_error(ERR_MEMORY, NULL), ERR_MEMORY);
	redir->next = NULL;
	if (!curr->redirs)
		curr->redirs = redir;
	else
	{
		last = curr->redirs;
		while (last->next)
			last = last->next;
		last->next = redir;
	}
	return (ERR_NONE);
}

static int	handle_output(t_shell *shell, t_cmd *curr, int *i)
{
	t_redir	*redir;
	t_redir	*last;

	(*i)++;
	if (*i >= shell->num_tokens || shell->tokens[*i].type != WORD)
		return (print_parse_error(ERR_MISS_FILENAME, NULL), ERR_MISS_FILENAME);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (print_parse_error(ERR_MEMORY, NULL), ERR_MEMORY);
	redir->fd = 1; //stdout
	redir->type = shell->tokens[(*i) - 1].type;
	redir->filename = ft_strdup(shell->tokens[*i].value);
	if (!redir->filename)
		return (free(redir), print_parse_error(ERR_MEMORY, NULL), ERR_MEMORY);
	redir->next = NULL;
	if (!curr->redirs)
		curr->redirs = redir;
	else
	{
		last = curr->redirs;
		while (last->next)
			last = last->next;
		last->next = redir;
	}
	return (ERR_NONE);
}

static int	handle_pipe(t_shell *shell, t_cmd **curr, int *start, int i)
{
	if (*start == 0 || i + 1 >= shell->num_tokens
		|| shell->tokens[i + 1].type == PIPE)
		return (print_parse_error(ERR_UNEXPECTED_TOKEN, &shell->tokens[i]),
			ERR_UNEXPECTED_TOKEN);
	(*curr)->next = new_cmd(count_words(shell, i + 1));
	if (!(*curr)->next)
		return (print_parse_error(ERR_MEMORY, NULL), ERR_MEMORY);
	*curr = (*curr)->next;
	*start = 0;
	return (ERR_NONE);
}

int	parse(t_shell *shell)
{
	t_cmd	*head;
	t_cmd	*curr;
	int	counted_words;
	int		err;

	counted_words = count_words(shell, 0);
	head = new_cmd(counted_words);
	if (!head)
		return (ERR_PARSE);
	shell->cmds = head;
	curr = head;
	int i = 0;
	int start = 0;
	while (i < shell->num_tokens)
	{
		if (shell->tokens[i].type == WORD)
			err = handle_word(curr, shell->tokens[i].value,
				&start, counted_words);
		else if (shell->tokens[i].type == INPUT)
			err = handle_input(shell, curr, &i);
		else if (shell->tokens[i].type == OUTPUT || shell->tokens[i].type == APPEND)
			err = handle_output(shell, curr, &i);
		else if (shell->tokens[i].type == PIPE)
			err = handle_pipe(shell, &curr, &start, i);
		if (err != ERR_NONE)
				return (err);
		i++;
	}
	return (ERR_NONE);
}
