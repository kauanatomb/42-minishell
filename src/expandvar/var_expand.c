/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/29 12:04:38 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_mode(char c, int mode)
{
	if (c == '\'' && mode == 0)
		return (1);
	if (c == '\'' && mode == 1)
		return (0);
	if (c == '"' && mode == 0)
		return (2);
	if (c == '"' && mode == 2)
		return (0);
	return (mode);
}

static char	*handle_dollar(char **str, char *result, t_shell *shell, int mode)
{
	if (mode == 1)
	{
		result = append_char_to_result(**str, result);
		(*str)++;
		return (result);
	}
	(*str)++;
	if (**str == '?')
	{
		result = expand_dollar_question(shell, result);
		if (result)
			(*str)++;
	}
	else
		result = expand_key_var(str, shell, result);
	return (result);
}

static char	*append_and_check(char c, char *result)
{
	result = append_char_to_result(c, result);
	if (!result)
		return (NULL);
	return (result);
}

static char	*expand_var_loop(char *str, t_shell *shell, char *result, int mode)
{
	int	new_mode;

	while (*str)
	{
		new_mode = update_mode(*str, mode);
		if (new_mode != mode)
			mode = new_mode;
		else if (*str == '$')
		{
			result = handle_dollar(&str, result, shell, mode);
			if (!result)
				return (NULL);
			continue ;
		}
		else
		{
			result = append_and_check(*str, result);
			if (!result)
				return (NULL);
		}
		str++;
	}
	return (result);
}

char	*expand_var(char *str, t_shell *shell)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	return (expand_var_loop(str, shell, result, 0));
}
