/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emmodogl <rootkalixox@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:54:33 by emmodogl          #+#    #+#             */
/*   Updated: 2023/09/11 19:17:53 by emmodogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

char	**free_problem(char **arg, int position)
{
	int	i;

	i = 0;
	while (i < position)
	{
		free(arg[i]);
		i++;
	}
	free(arg);
	return (0);
}

char	**ft_malloc(char **arg, char *argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	arg = (char **)malloc(sizeof(char *) * 3);
	if (arg == 0)
		return (0);
	arg[0] = (char *)malloc(sizeof(char) * 5);
	if (arg[0] == 0)
		return (free_problem(arg, 0));
	arg[1] = (char *)malloc(sizeof(char) * 3);
	if (arg[1] == 0)
		return (free_problem(arg, 1));
	arg[2] = (char *)malloc(sizeof(char) * (i + 1));
	if (arg[2] == 0)
		return (free_problem(arg, 2));
	arg[0] = "bash";
	arg[1] = "-c";
	arg[2] = argv;
	return (arg);
}

int	process_read(char **argv, int fd[2])
{
	char	**arg;
	int		file;

	arg = NULL;
	arg = ft_malloc(arg, argv[2]);
	file = open(argv[1], O_RDONLY);
	if (file == -1)
	{
		perror(argv[1]);
		return (-1);
	}
	close(fd[0]);
	dup2(file, 0);
	dup2(fd[1], 1);
	execve("/bin/bash", arg, NULL);
	perror("execve");
	exit (0);
}

int	process_write(char **argv, int fd[2])
{
	char	**arg2;
	int		file;

	arg2 = NULL;
	file = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (file == -1)
	{
		perror(argv[4]);
		return (-1);
	}
	close(fd[1]);
	dup2(fd[0], 0);
	dup2(file, 1);
	arg2 = ft_malloc(arg2, argv[3]);
	execve("/bin/bash", arg2, NULL);
	perror("execve");
	exit (0);
}

int	main(int argc, char *argv[])
{
	int		fd[2];
	int		pid[2];

	if (argc != 5)
		return (0);
	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid[0] = fork();
	if (pid[0] == 0)
		process_read(argv, fd);
	else
	{
		pid[1] = fork();
		if (pid[1] == 0)
			process_write(argv, fd);
	}
	wait (NULL);
	close(fd[0]);
	close(fd[1]);
}
