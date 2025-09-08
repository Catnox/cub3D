/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:43:24 by radubos           #+#    #+#             */
/*   Updated: 2024/12/27 12:07:31 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file6.txt", O_RDONLY);
	//fd = 42;//pour tester le retour NULL
	//fd = 0;//pour tester STDIN
	line = get_next_line(fd);
	//printf("%s", line);//pour tester le retour NULL
	while (line)//remplacer par 1 pour tester STDIN
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
