/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/22 16:14:04 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_w(t_cmd *cmd, char *word, int *pos, int max_words)
{
	if (*pos >= max_words)
		return (print_parse_error(ERR_PARSE, NULL), ERR_PARSE);
	cmd->argv[*pos] = ft_strdup(word);
	if (!cmd->argv[*pos])
		return (print_parse_error(ERR_MEMORY, NULL), ERR_MEMORY);
	(*pos)++;
	return (ERR_NONE);
}

static int	handle_pipe(t_shell *shell, t_cmd **curr, int *start, int i)
{
	if ((*start == 0 && !(*curr)->redirs) || i + 1 >= shell->num_tokens
		|| shell->tokens[i + 1].type == PIPE)
		return (print_parse_error(ERR_UNEXPECTED_TOKEN, &shell->tokens[i]),
			ERR_UNEXPECTED_TOKEN);
	(*curr)->next = new_cmd(countword(shell, i + 1));
	if (!(*curr)->next)
		return (print_parse_error(ERR_MEMORY, NULL), ERR_MEMORY);
	*curr = (*curr)->next;
	*start = 0;
	return (ERR_NONE);
}

static int	handle_redir(t_cmd *curr, t_token *redir_tok, t_token *file_tok)
{
	t_redir	*redir;
	int		err;

	if (!file_tok || file_tok->type != WORD)
		return (print_parse_error(ERR_MISS_FILENAME, NULL), ERR_MISS_FILENAME);
	redir = init_redir(redir_tok);
	if (!redir)
		return (print_parse_error(ERR_MEMORY, NULL), ERR_MEMORY);
	err = set_redir_target(redir, file_tok);
	if (err != ERR_NONE)
		return (free(redir), print_parse_error(ERR_MEMORY, NULL), ERR_MEMORY);
	attach_redir(redir, curr);
	return (ERR_NONE);
}

static int	parse_tokens(t_shell *shell, int i, int start, int cmd_start)
{
	t_token		tok;
	t_cmd		*curr;
	int			err;

	if (i >= shell->num_tokens)
		return (ERR_NONE);
	curr = get_last_cmd(shell);
	tok = shell->tokens[i];
	err = 0;
	if (tok.type == WORD)
		err = handle_w(curr, tok.value, &start, countword(shell, cmd_start));
	else if (tok.type == INPUT || tok.type == HEREDOC
		|| tok.type == OUTPUT || tok.type == APPEND)
	{
		err = handle_redir(curr, &shell->tokens[i], &shell->tokens[i + 1]);
		i++;
	}
	else if (tok.type == PIPE)
	{
		err = handle_pipe(shell, &curr, &start, i);
		cmd_start = i + 1;
	}
	if (err != ERR_NONE)
		return (err);
	return (parse_tokens(shell, i + 1, start, cmd_start));
}

int	parse(t_shell *shell)
{
	t_cmd	*curr;

	curr = new_cmd(countword(shell, 0));
	if (!curr)
		return (ERR_PARSE);
	shell->cmds = curr;
	return (parse_tokens(shell, 0, 0, 0));
}
