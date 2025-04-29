/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atseruny <atseruny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:33:30 by atseruny          #+#    #+#             */
/*   Updated: 2025/04/28 20:27:56 by atseruny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_join(char *s1, char *s2, char ch)
{
	char	*s;
	int		l1;
	int		l2;
	int		i;

	i = 0;
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	s = (char *)malloc((l1 + l2 + 2) * sizeof(char));
	if (!s)
		return (NULL);
	while (i < l1)
	{
		s[i] = s1[i];
		i++;
	}
	s[i] = ch;
	i = 0;
	while (i < l2)
	{
		s[l1 + i + 1] = s2[i];
		i++;
	}
	s[l1 + l2 + 1] = '\0';
	return (s);
}

void	bash_script(t_pipex *pipex)
{
	char	*mess;
	int		j;

	j = 0;
	if (access(pipex->cmd[0], F_OK | X_OK) == 0)
		execve(pipex->cmd[0], pipex->cmd, pipex->env);
	mess = ft_strjoin("command not found: ", pipex->argv[pipex->current_cmd + 2]);
	ft_putstr_fd(mess, 2);
	free(mess);
	err_exit("\n", pipex);
}

void	execute_cmd(t_pipex *pipex)
{
	int		j;
	char	*full_path;
	char	*mess;

	j = 0;
	if (pipex->path != NULL)
	{
		execve(pipex->cmd[0], pipex->cmd, pipex->env);
		while (pipex->path[j])
		{
			if (ft_strncmp(pipex->cmd[0], "./", 2) == 0)
				bash_script(pipex);
			else
			{
				full_path = ft_join(pipex->path[j], pipex->cmd[0], '/');
				execve(full_path, pipex->cmd, pipex->env);
			}
			j++;
			free(full_path);
		}
	}
	mess = ft_strjoin("command not found: ", pipex->argv[pipex->current_cmd + 2]);
	ft_putstr_fd(mess, 2);
	free(mess);
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
	pipex->fds[1] = fders[1];
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
