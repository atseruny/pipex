/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anush <anush@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 15:38:56 by atseruny          #+#    #+#             */
/*   Updated: 2025/04/28 00:51:05 by anush            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	if (pipex != NULL)
		free_double(pipex->path);
	exit(1);
}

void	check_files(char *infile, char *outfile, t_pipex *pipex)
{
	if (access(infile, F_OK | R_OK) == -1)
		err_exit("Infile is invalid\n", pipex);
	if (access(outfile, F_OK) != -1 && access(outfile, W_OK) == -1)
		err_exit("Outfile is invalid\n", pipex);
}
