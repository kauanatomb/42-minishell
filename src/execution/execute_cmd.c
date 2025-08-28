/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:32:28 by ktombola          #+#    #+#             */
/*   Updated: 2025/08/12 10:55:19 by ktombola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char *name)
{
    if (!name)
        return (0);
    return (!ft_strcmp(name, "cd") || !ft_strcmp(name, "exit")
        || !ft_strcmp(name, "export") || !ft_strcmp(name, "unset")
        || !ft_strcmp(name, "echo") || !ft_strcmp(name, "pwd")
        || !ft_strcmp(name, "env"));
}

// int exec_builtin_parent(t_cmd *cmd, t_shell *shell)
// {
//     char *name = cmd->argv[0];

//     if (!ft_strcmp(name, "cd"))
//         return builtin_cd(cmd->argv, shell);
//     if (!ft_strcmp(name, "exit"))
//         return builtin_exit(cmd->argv, shell);
//     if (!ft_strcmp(name, "export"))
//         return builtin_export(cmd->argv, shell);
//     if (!ft_strcmp(name, "unset"))
//         return builtin_unset(cmd->argv, shell);
//     return 1;
// }

int exec_builtin_child(t_cmd *cmd, t_shell *shell)
{
    char *name = cmd->argv[0];

    (void)shell;
    if (!ft_strcmp(name, "echo"))
        return builtin_echo(cmd->argv);
    // if (!ft_strcmp(name, "pwd"))
    //     return builtin_pwd();
    // if (!ft_strcmp(name, "env"))
    //     return builtin_env(shell->env);
    return 1;
}

// quick exec external: execvp cannot be used in this project
int exec_external(t_cmd *cmd, t_shell *shell)
{
    pid_t pid;
    int status;

    (void)shell;
    pid = fork();
    if (pid == -1)
        return (perror("fork"), 1);
    if (pid == 0) // child
    {
        execvp(cmd->argv[0], cmd->argv);
        perror(cmd->argv[0]);
        exit(127); // command not found
    }
    else // parent
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        return (1);
    }
}

void execute_cmd(t_cmd *cmd, t_shell *shell)
{
    int status;
    pid_t   pid;

    (void)shell;
    if (!cmd || !cmd->argv || !cmd->argv[0])
        return ;
    if (!ft_strcmp(cmd->argv[0], "cd") || !ft_strcmp(cmd->argv[0], "exit")
        || !ft_strcmp(cmd->argv[0], "export") || !ft_strcmp(cmd->argv[0], "unset"))
    {
        // exec_builtin_parent(cmd, shell);
    }
    else if (!ft_strcmp(cmd->argv[0], "echo") || !ft_strcmp(cmd->argv[0], "pwd")
        || !ft_strcmp(cmd->argv[0], "env"))
    {
        pid = fork();
        if (pid == 0) // child
            exit(exec_builtin_child(cmd, shell));
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            (WEXITSTATUS(status));
    }
    else
        exec_external(cmd, shell);
}

