/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atseruny <atseruny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:39:35 by atseruny          #+#    #+#             */
/*   Updated: 2025/04/27 19:13:05 by atseruny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>


typedef struct s_pipex
{
	int				fds[2];
	char			**path;
	char			**argv;
	char			**env;
	int				infile;
	int				outfile;
	char			**cmd;
	int				current_cmd;
	int				count_cmd;
}					t_pipex;

void	check_files(char *infile, char *outfile, t_pipex *pipex);
void	err_exit(char *message, t_pipex *pipex);
void	free_double(char **s);
void	first(t_pipex *pipex);
void	last(t_pipex *pipex);
void	mid(t_pipex *pipex);



#endif