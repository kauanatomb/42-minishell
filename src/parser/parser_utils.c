/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_cmd_list(t_cmd *cmds)
{
    int     i;
    t_cmd   *curr;
    t_cmd   *next;

    if (!cmds)
        return ;
    curr = cmds;
    while (curr)
    {
        next = curr->next;
        if (curr->argv)
        {
            i = 0;
            while (curr->argv[i])
                free(curr->argv[i++]);
            free(curr->argv);
        }
        free(curr->infile);
        free(curr->outfile);
        free(curr);
        curr = curr->next;
    }
}