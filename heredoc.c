/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atseruny <atseruny@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:05:02 by atseruny          #+#    #+#             */
/*   Updated: 2025/04/29 20:27:37 by atseruny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_doc(int argc, char **argv, char **env, t_pipex *heredoc)
{
	int	i;

	i = 0;
	heredoc->cmd = NULL;
	heredoc->path = NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			heredoc->path = ft_split(env[i] + 5, ':');
		i++;
	}
	heredoc->argc = argc;
	heredoc->count_cmd = argc - 4;
	heredoc->current_cmd = 0;
	heredoc->argv = argv;
	heredoc->env = env;
	heredoc->pid = (int *)malloc(heredoc->count_cmd * sizeof(int));
	heredoc->infile = open("file.txt", O_RDONLY | O_CREAT | O_WRONLY | O_TRUNC, 0777);
	check_files("file.txt", argv[argc - 1], heredoc);
	heredoc->outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (heredoc->infile == -1 || heredoc->outfile == -1)
		err_exit("Error opening\n", heredoc);
}

int	ft_strcmp(char *s1, char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] == s2[i]) && (s1[i] != '\0') && s2[i] != '\n')
		i++;
	if (s2[i] == '\n' && s1[i] == '\0')
		return (0);
	return (1);
}

void	here_doc(int argc, char **argv, char **env, t_pipex *heredoc)
{
	char	*str;
	int		i;
	int		status;

	init_doc(argc, argv, env, heredoc);
	str = get_next_line(0);
	while (str != NULL && ft_strcmp(argv[2], str) != 0)
	{
		write(heredoc->infile, str, ft_strlen(str));
		free(str);
		str = get_next_line(0);
	}
	free(str);
	while (heredoc->current_cmd < heredoc->count_cmd)
	{
		heredoc->cmd = ft_split((heredoc->argv)[heredoc->current_cmd + 3], ' ');
		if (heredoc->cmd == NULL)
			err_exit("Command is empty\n", heredoc);
		if (heredoc->current_cmd == 0)
			first(heredoc);
		else if (heredoc->current_cmd == heredoc->count_cmd - 1)
			last(heredoc);
		else
			mid(heredoc);
		free_double(heredoc->cmd);
		(heredoc->current_cmd)++;
	}
	i = 0;
	while (i < heredoc->count_cmd)
		waitpid(heredoc->pid[i++], &status, 0);
	free_double(heredoc->path);
	free(heredoc->pid);
}
