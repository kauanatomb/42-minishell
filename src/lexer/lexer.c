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

int lexer(t_shell *shell, char *line)
{
    shell->tokens = malloc(sizeof(t_token));
    if (!shell->tokens)
        return (137);
    shell->tokens->l_tokens = split_custom(line); // implem function: array of all possible tokens
    if (!shell->tokens->l_tokens)
        return (137);
    shell->tokens->l_types = malloc(sizeof(char *)
            * (ft_array_len(shell->tokens->l_tokens) + 3));
    if (!shell->tokens->l_types)
        return (ft_free_array(shell->tokens->l_tokens), free(shell->tokens), 137)
    if (give_types(shell)) // implem function: save types of tokens
        return (ft_free_tokens(shell->tok), 137); //implem function free just tokens
}