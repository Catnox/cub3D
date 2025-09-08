#include "cub3d.h"
#include <math.h>

// Structure pour un nœud dans l'algorithme A*
typedef struct s_node
{
	int			x;
	int			y;
	float		g_cost;		// Coût depuis le début
	float		h_cost;		// Heuristique vers la fin
	float		f_cost;		// g_cost + h_cost
	struct s_node	*parent;
	struct s_node	*next;
}	t_node;

// Liste des nœuds ouverts et fermés
typedef struct s_pathfind
{
	t_node	*open_list;
	t_node	*closed_list;
	t_node	**path;
	int		path_length;
}	t_pathfind;

// Calcule la distance heuristique (Manhattan)
static float	calculate_heuristic(int x1, int y1, int x2, int y2)
{
	return (fabsf((float)(x2 - x1)) + fabsf((float)(y2 - y1)));
}

// Crée un nouveau nœud
static t_node	*create_node(int x, int y, float g_cost, float h_cost, t_node *parent)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->x = x;
	node->y = y;
	node->g_cost = g_cost;
	node->h_cost = h_cost;
	node->f_cost = g_cost + h_cost;
	node->parent = parent;
	node->next = NULL;
	return (node);
}

// Ajoute un nœud à une liste
static void	add_to_list(t_node **list, t_node *node)
{
	node->next = *list;
	*list = node;
}

// Trouve un nœud dans une liste par coordonnées
static t_node	*find_node_in_list(t_node *list, int x, int y)
{
	while (list)
	{
		if (list->x == x && list->y == y)
			return (list);
		list = list->next;
	}
	return (NULL);
}

// Trouve le nœud avec le plus petit f_cost dans la liste ouverte
static t_node	*get_lowest_f_cost(t_node *open_list)
{
	t_node	*current;
	t_node	*lowest;

	if (!open_list)
		return (NULL);
	lowest = open_list;
	current = open_list->next;
	while (current)
	{
		if (current->f_cost < lowest->f_cost)
			lowest = current;
		current = current->next;
	}
	return (lowest);
}

// Retire un nœud d'une liste
static void	remove_from_list(t_node **list, t_node *node_to_remove)
{
	t_node	*current;
	t_node	*prev;

	if (!*list || !node_to_remove)
		return ;
	if (*list == node_to_remove)
	{
		*list = (*list)->next;
		return ;
	}
	current = *list;
	prev = NULL;
	while (current && current != node_to_remove)
	{
		prev = current;
		current = current->next;
	}
	if (current)
		prev->next = current->next;
}

// Vérifie si une case est traversable
static int	is_walkable(t_game *game, int x, int y)
{
	if (x < 0 || y < 0 || x >= game->map->width || y >= game->map->height)
		return (0);
	if (game->map->grid[y][x] == '1')  // Mur
		return (0);
	if (game->map->grid[y][x] == 'D')  // Porte fermée
		return (0);
	return (1);
}

// Libère une liste de nœuds
static void	free_node_list(t_node *list)
{
	t_node	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		free(temp);
	}
}

// Reconstruit le chemin depuis le nœud final
static t_node	**build_path(t_node *end_node, int *path_length)
{
	t_node	*current;
	t_node	**path;
	int		length;
	int		i;

	// Compte la longueur du chemin
	length = 0;
	current = end_node;
	while (current)
	{
		length++;
		current = current->parent;
	}
	
	// Alloue le tableau de chemin
	path = malloc(sizeof(t_node *) * length);
	if (!path)
		return (NULL);
	
	// Remplit le chemin en sens inverse
	current = end_node;
	i = length - 1;
	while (current && i >= 0)
	{
		path[i] = create_node(current->x, current->y, current->g_cost, current->h_cost, NULL);
		current = current->parent;
		i--;
	}
	
	*path_length = length;
	return (path);
}

// Traite les voisins d'un nœud
static void	process_neighbors(t_game *game, t_pathfind *pathfind, t_node *current, int target_x, int target_y)
{
	int		directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
	int		i;
	int		new_x, new_y;
	float		new_g_cost;
	t_node	*neighbor;
	t_node	*existing_node;

	i = 0;
	while (i < 8)
	{
		new_x = current->x + directions[i][0];
		new_y = current->y + directions[i][1];
		
		// Vérifie si la case est traversable
		if (!is_walkable(game, new_x, new_y))
		{
			i++;
			continue ;
		}
		
		// Vérifie si déjà dans la liste fermée
		if (find_node_in_list(pathfind->closed_list, new_x, new_y))
		{
			i++;
			continue ;
		}
		
		// Calcule le nouveau g_cost (diagonale coûte plus cher)
		if (i < 4)  // Mouvement horizontal/vertical
			new_g_cost = current->g_cost + 1.0f;
		else  // Mouvement diagonal
			new_g_cost = current->g_cost + 1.414f;
		
		existing_node = find_node_in_list(pathfind->open_list, new_x, new_y);
		if (!existing_node)
		{
			// Ajoute un nouveau nœud à la liste ouverte
			neighbor = create_node(new_x, new_y, new_g_cost, 
				calculate_heuristic(new_x, new_y, target_x, target_y), current);
			if (neighbor)
				add_to_list(&pathfind->open_list, neighbor);
		}
		else if (new_g_cost < existing_node->g_cost)
		{
			// Met à jour le nœud existant avec un meilleur chemin
			existing_node->g_cost = new_g_cost;
			existing_node->f_cost = existing_node->g_cost + existing_node->h_cost;
			existing_node->parent = current;
		}
		i++;
	}
}

// Algorithme A* principal
t_node	**find_path_astar(t_game *game, int start_x, int start_y, int target_x, int target_y, int *path_length)
{
	t_pathfind	pathfind;
	t_node		*start_node;
	t_node		*current;
	t_node		**result;

	// Initialise la structure de pathfinding
	pathfind.open_list = NULL;
	pathfind.closed_list = NULL;
	pathfind.path = NULL;
	pathfind.path_length = 0;
	*path_length = 0;
	
	// Vérifie que la cible est atteignable
	if (!is_walkable(game, target_x, target_y))
		return (NULL);
	
	// Crée le nœud de départ
	start_node = create_node(start_x, start_y, 0.0f, 
		calculate_heuristic(start_x, start_y, target_x, target_y), NULL);
	if (!start_node)
		return (NULL);
	
	add_to_list(&pathfind.open_list, start_node);
	
	// Boucle principale A*
	while (pathfind.open_list)
	{
		// Trouve le nœud avec le plus petit f_cost
		current = get_lowest_f_cost(pathfind.open_list);
		if (!current)
			break ;
		
		// Déplace le nœud de la liste ouverte à la liste fermée
		remove_from_list(&pathfind.open_list, current);
		add_to_list(&pathfind.closed_list, current);
		
		// Vérifie si on a atteint la cible
		if (current->x == target_x && current->y == target_y)
		{
			result = build_path(current, path_length);
			free_node_list(pathfind.open_list);
			free_node_list(pathfind.closed_list);
			return (result);
		}
		
		// Traite tous les voisins
		process_neighbors(game, &pathfind, current, target_x, target_y);
	}
	
	// Aucun chemin trouvé
	free_node_list(pathfind.open_list);
	free_node_list(pathfind.closed_list);
	return (NULL);
}

// Libère un chemin
void	free_path(t_node **path, int path_length)
{
	int	i;

	if (!path)
		return ;
	i = 0;
	while (i < path_length)
	{
		if (path[i])
			free(path[i]);
		i++;
	}
	free(path);
}

// Obtient la prochaine position sur le chemin
t_vec2	get_next_position_on_path(t_node **path, int path_length, int current_step)
{
	t_vec2	next_pos;

	next_pos.x = 0.0;
	next_pos.y = 0.0;
	
	if (!path || current_step >= path_length - 1)
		return (next_pos);
	
	next_pos.x = (double)path[current_step + 1]->x + 0.5;  // Centre de la case
	next_pos.y = (double)path[current_step + 1]->y + 0.5;
	
	return (next_pos);
}
