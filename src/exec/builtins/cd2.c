/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/29 18:14:28 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_directory_access(char *arg)
{
	struct stat	info;

	if (!arg)
		return (0);
	else if (stat(arg, &info) == 0 && S_ISDIR(info.st_mode))
		return (0);
	else if (stat(arg, &info) == 0 && !S_ISDIR(info.st_mode))
		return (2);
	else
		return (1);
}
