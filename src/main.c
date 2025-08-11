/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/11 11:53:48 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exit_ctrld(t_shell *shell)
{
    (void)shell;
    write(STDIN_FILENO, "exit\n", 5);
    ft_clean_fd(0);
    exit(0);
}

int verify_line(char *line)
{
    bool single_q;
    bool double_q;

    single_q = false;
    double_q = false;
    while (*line)
    {
        if (*line == '\'')
            single_q = !single_q;
        else if (*line == '\"')
            double_q = !double_q;
        line++;
    }
    if (single_q || double_q)
    {
        write(STDERR_FILENO, "unclosed quotes\n", 17);
        return (1);
    }
    return (0);
}

int empty_line(char *line)
{
    int i;

    if (!line || !*line)
        return (0);
    i = 0;
    while (line[i])
    {
        if (!ft_isspace(line[i]))
            return (1);
        i++;
    }
    return (0);
}

int read_line(t_shell *shell)
{
    char *line;

    rl_on_new_line(); // Prepare readline for new input
    line = readline("minishell> ");
    if (!line)
    {
        // free args from shell
        rl_clear_history(); // Clear previous history
        exit_ctrld(shell);
    }
    if (verify_line(line) || empty_line(line))
        return (free(line), 0);
    add_history(line);
    return (1);
}

int init_shell(t_shell *shell, char **envp)
{
    if (/*function to count array len */)
    {
        shell->env = malloc(sizeof(char *) * 4);
        if (!shell->env)
            return (137);
        if (/*call function to create env, if fail return*/)
            return (137);
    }
    else
    {
        shell->env = malloc(sizeof(char *) * /*function t count array len*/ + 1);
        if (!shell->env)
            return (137);
        if (/*call function to create env, if fail return*/)
            return (137);
    }
    shell->path_flag = 0;
    return (0);
}

void    signal_receiver(void)
{
    
}

int main(int argc, char *argv[], char **envp)
{
    t_shell shell;

	if (argc != 1)
        return (1);
    init_shell(&shell, envp);
    shell.error = 0;
    signal_receiver();
    while (1)
        read_line(&shell);
    rl_clear_history();
    (void)argv;
    return (0);
}