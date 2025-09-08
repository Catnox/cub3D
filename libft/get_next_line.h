/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:53:26 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 16:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4
# endif

# include <unistd.h>
# include <stdlib.h>

char	*get_next_line(int fd);
char	*fill_tank(char **tank, char **buffer, int fd);
char	*end_of_read(char **tank, char **buffer);
int		gnl_strchr(char *s, int c);
char	*gnl_strjoin(char *s1, char *s2);
char	*extract_line(char *tank, char c);
char	*update_tank(char *tank, char c);
void	ft_free(char **tank);

#endif