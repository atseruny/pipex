/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anush <anush@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:33:30 by atseruny          #+#    #+#             */
/*   Updated: 2025/04/28 00:46:27 by anush            ###   ########.fr       */
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

void	execute_cmd(t_pipex *pipex)
{
	int		j;
	char	*full_path;

	j = 0;
	while (pipex->path[j])
	{
		if (ft_strncmp(pipex->cmd[0], "./", 2) == 0\
		&& access(pipex->cmd[0], F_OK | X_OK) == 0)
			full_path = ft_strdup(pipex->cmd[0]);
		else
			full_path = ft_join(pipex->path[j], pipex->cmd[0], '/');
		execve(full_path, pipex->cmd, pipex->env);
		j++;
		free(full_path);
	}
	free_double(pipex->cmd);
	free_double(pipex->path);
	perror("Execve failed");
	exit(1);
}

void	mid(t_pipex *pipex)
{
	int	f;
	int	fders[2];

	if (pipe(fders) == -1)
		err_exit("Error pipeing\n", pipex);
	f = fork();
	if (f == -1)
		err_exit("Error forking\n", pipex);
	if (f == 0)
	{
		// close(pipex->fds[1]);
		close(fders[0]);
		dup2(pipex->fds[0], STDIN_FILENO); 
		dup2(fders[1], STDOUT_FILENO);
		close(pipex->fds[0]);
		close(fders[1]);
		execute_cmd(pipex);
	}
	close(pipex->fds[0]);
	// close(fders[1]);
	pipex->fds[0] = fders[0];
	pipex->fds[1] = fders[1];
	close(pipex->fds[1]);

}

void	first(t_pipex *pipex)
{
	int	f;
	
	if (pipe(pipex->fds) == -1)
		err_exit("Error pipeing\n", pipex);
	f = fork();
	if (f == -1)
		err_exit("Error forking\n", pipex);
	if (f == 0)
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
	int		f;

	f = fork();
	if (f == -1)
		err_exit("Error forking\n", pipex);
	if (f == 0)
	{
		// close(pipex->fds[1]);
		dup2(pipex->fds[0], STDIN_FILENO);
		dup2(pipex->outfile, STDOUT_FILENO);
		close(pipex->fds[0]);
		close(pipex->outfile);
		execute_cmd(pipex);
	}
	close(pipex->fds[0]);
	close(pipex->outfile);
}
