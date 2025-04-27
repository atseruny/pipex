/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atseruny <atseruny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:39:27 by atseruny          #+#    #+#             */
/*   Updated: 2025/04/27 19:55:32 by atseruny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init(int argc, char **argv, char **env, t_pipex *pipex)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			pipex->path = ft_split(env[i] + 5, ':');
		i++;
	}
	check_files(argv[1], argv[argc - 1], pipex);
	pipex->count_cmd = argc - 3;
	pipex->current_cmd = 0;
	pipex->argv = argv;
	pipex->env = env;
	pipex->infile = open(argv[1], O_RDONLY);
	pipex->outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);

}

int	main(int argc, char **argv, char **env)
{
	t_pipex	pipex;

	if (argc < 5)
		return(write(2, "Wrong number of arguments\n", 26), 1);
	init(argc, argv, env, &pipex);
	while (pipex.current_cmd < pipex.count_cmd)
	{
		pipex.cmd = ft_split((pipex.argv)[pipex.current_cmd + 2], ' ');
		if (pipex.current_cmd == 0)
			first(&pipex);
		else if (pipex.current_cmd == pipex.count_cmd - 1)
			last(&pipex);
		else
			mid(&pipex);
		free_double(pipex.cmd);
		(pipex.current_cmd)++;
	}
	free_double(pipex.path);
	int status;
	while (waitpid(-1, &status, 0) > 0)
		;
}