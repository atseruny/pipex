/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anush <anush@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:33:30 by atseruny          #+#    #+#             */
/*   Updated: 2025/05/01 01:20:34 by anush            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	bash_script(t_pipex *pipex)
{
	char	*m;

	if (access(pipex->cmd[0], F_OK | X_OK) == 0)
		if (execve(pipex->cmd[0], pipex->cmd, pipex->env) == -1)
			err_exit("execve failed\n", pipex);
	m = ft_strjoin("command not found: ", pipex->argv[pipex->current_cmd + 2]);
	ft_putstr_fd(m, 2);
	free(m);
	err_exit("\n", pipex);
}

void	execute_cmd(t_pipex *pipex)
{
	int		j;
	char	*full_path;
	char	*m;

	j = 0;
	if (pipex->path != NULL)
	{
		if (execve(pipex->cmd[0], pipex->cmd, pipex->env) == -1)
			err_exit("execve failed\n", pipex);
		while (pipex->path[j])
		{
			if (ft_strncmp(pipex->cmd[0], "./", 2) == 0)
				bash_script(pipex);
			else
			{
				full_path = ft_join(pipex->path[j], pipex->cmd[0], '/');
				if (execve(full_path, pipex->cmd, pipex->env) == -1)
					err_exit("execve failed\n", pipex);
			}
			j++;
			free(full_path);
		}
	}
	m = ft_strjoin("command not found: ", pipex->argv[pipex->current_cmd + 3]);
	ft_putstr_fd(m, 2);
	free(m);
	err_exit("\n", pipex);
}

void	mid(t_pipex *pipex)
{
	int	fders[2];

	if (pipe(fders) == -1)
		err_exit("Error pipeing\n", pipex);
	pipex->pid[pipex->current_cmd] = fork();
	if (pipex->pid[pipex->current_cmd] == -1)
		err_exit("Error forking\n", pipex);
	if (pipex->pid[pipex->current_cmd] == 0)
	{
		close(fders[0]);
		dup2(pipex->fds[0], STDIN_FILENO);
		dup2(fders[1], STDOUT_FILENO);
		close(pipex->fds[0]);
		close(fders[1]);
		execute_cmd(pipex);
	}
	close(pipex->fds[0]);
	pipex->fds[0] = fders[0];
	close(pipex->fds[1]);
}

void	first(t_pipex *pipex)
{
	if (pipe(pipex->fds) == -1)
		err_exit("Error pipeing\n", pipex);
	pipex->pid[pipex->current_cmd] = fork();
	if (pipex->pid[pipex->current_cmd] == -1)
		err_exit("Error forking\n", pipex);
	if (pipex->pid[pipex->current_cmd] == 0)
	{
		close(pipex->fds[0]);
		dup2(pipex->infile, STDIN_FILENO);
		dup2(pipex->fds[1], STDOUT_FILENO);
		close(pipex->infile);
		close(pipex->fds[1]);
		execute_cmd(pipex);
	}
	close(pipex->infile);
	close(pipex->fds[1]);
}

void	last(t_pipex *pipex)
{
	pipex->pid[pipex->current_cmd] = fork();
	if (pipex->pid[pipex->current_cmd] == -1)
		err_exit("Error forking\n", pipex);
	if (pipex->pid[pipex->current_cmd] == 0)
	{
		dup2(pipex->fds[0], STDIN_FILENO);
		dup2(pipex->outfile, STDOUT_FILENO);
		close(pipex->fds[0]);
		close(pipex->outfile);
		execute_cmd(pipex);
	}
	close(pipex->fds[0]);
	close(pipex->outfile);
}
