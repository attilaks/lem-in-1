/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map_info.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabshire <gabshire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 19:23:43 by gabshire          #+#    #+#             */
/*   Updated: 2019/05/15 19:26:36 by gabshire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static void	quantity_of_ants(t_map *map, char *line)
{
	char	*str;

	map->ants = ft_atoi(line);
	str = ft_itoa(map->ants);
	if (ft_strcmp(str, line) || map->ants < 1)
	{
		ft_printf(ER "Incorrect number of ants!");
		map->error++;
	}
	else
		ft_printf("%s\n", line);
	if (str)
		free(str);
	str = NULL;
}

static	void start_map_info(t_map *map, int fd, char *line)
{
	if (get_next_line(fd, &line) == 1)
	{
		quantity_of_ants(map, line);
		free(line);
		line = NULL;
	}
}

static void	organize_room(e_sort *time_to_sort, t_map *map, char *line, t_list *room_list)
{
	if (*time_to_sort == not_sorted)
	{
		map->rooms = sorted_rooms_ptr_array(map, &room_list);
		check_name_duplicates(map);
	}
	(*time_to_sort) = sorted;
	organize_links(map, line);
}

static	int lstadd_room(e_sort time_to_sort, t_list **room_list, t_map *map, char *line)
{
	if (time_to_sort == sorted)
	{
		ft_printf(ER "No more rooms after connections have started!\n");
		map->error++;
		return (1);
	}
	ft_lstadd(room_list, ft_lstnew_ptr(create_room(map, line)));
	return (0);
}

void	get_map_info(t_map *map, int fd)
{
	char	*line;
	t_list	*room_list;
	e_sort	time_to_sort;

	room_list = NULL;
	line = NULL;
	time_to_sort = not_sorted;
	start_map_info(map, fd, line);
	while (get_next_line(fd, &line) > 0)
	{
		ft_printf("%s\n", line);
		if (!ft_strcmp(line, "##start"))
			start_init(map, line, fd);
		else if (!ft_strcmp(line, "##end"))
			end_init(map, line, fd);
		else if (ft_strchr(line, ' ') && ft_strncmp(line, "#", 1))
		{
			if (lstadd_room(time_to_sort, &room_list, map, line) == 1)
				continue ;
		}
		else if (ft_strchr(line, '-') && ft_strncmp(line, "#", 1))
			organize_room(&time_to_sort, map, line, room_list);
		free(line);
	}
	if (line)
		free(line);
}