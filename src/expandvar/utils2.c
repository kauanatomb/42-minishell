/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/26 12:22:12 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_filename(t_redir *r, t_shell *shell)
{
	char	*expanded;

	expanded = expand_var(r->filename, shell);
	if (!expanded)
		return (ERR_MEMORY);
	free(r->filename);
	r->filename = expanded;
	return (ERR_NONE);
}

int	expand_delimiter(t_redir *r)
{
	char	*expanded;

	expanded = expand_heredoc(r->delimiter, r);
	if (!expanded)
		return (ERR_MEMORY);
	free(r->delimiter);
	r->delimiter = expanded;
	return (ERR_NONE);
}
