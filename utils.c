/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anush <anush@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 15:38:56 by atseruny          #+#    #+#             */
/*   Updated: 2025/05/01 01:22:10 by anush            ###   ########.fr       */
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

void	free_double(char **s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return ;
	while (s[i] != NULL)
		free(s[i++]);
	free(s);
}

void	err_exit(char *message, t_pipex *pipex)
{
	ft_putstr_fd(message, 2);
	free_double(pipex->cmd);
	free(pipex->pid);
	free_double(pipex->path);
	exit(0);
}

void	check_files(char *infile, char *outfile, t_pipex *pipex)
{
	char	*m;

	if (access(infile, F_OK) == -1)
	{
		m = ft_strjoin("no such file or directory: ", (pipex->argv)[1]);
		ft_putstr_fd(m, 2);
		free(m);
		err_exit("\n", pipex);
	}
	if (access(infile, R_OK) == -1)
	{
		m = ft_strjoin("permission denied: ", (pipex->argv)[1]);
		ft_putstr_fd(m, 2);
		free(m);
		err_exit("\n", pipex);
	}
	if (access(outfile, F_OK) != -1 && access(outfile, W_OK) == -1)
	{
		m = ft_strjoin("permission denied: ", (pipex->argv)[pipex->argc - 1]);
		ft_putstr_fd(m, 2);
		free(m);
		err_exit("\n", pipex);
	}
}
