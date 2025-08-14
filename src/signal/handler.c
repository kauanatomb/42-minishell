/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 16:53:29 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sig(int sig)
{
	int	ret;

	(void)sig;
	ret = g_signal;
	g_signal = 130;
	write(STDIN_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	if (ret == 0 || ret == 130)
		rl_redisplay();
}

void	signal_receiver(void)
{
	struct sigaction	sa_c;
	struct sigaction	sa_slash;

	ft_bzero(&sa_c, sizeof(sigaction));
	ft_bzero(&sa_slash, sizeof(sigaction));
	sa_slash.sa_handler = SIG_IGN;
	sigemptyset(&sa_slash.sa_mask);
	sa_slash.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_slash, NULL) == -1)
	{
		write(2, "Sigaction failed\n", 17);
		exit(EXIT_FAILURE);
	}
	sa_c.sa_handler = handle_sig;
	sigemptyset(&sa_c.sa_mask);
	sa_c.sa_flags = 0;
	if (sigaction(SIGINT, &sa_c, NULL) == -1)
	{
		write(2, "Sigaction failed\n", 17);
		exit(EXIT_FAILURE);
	}
}
