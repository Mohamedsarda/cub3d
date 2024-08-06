#include "../../recasting.h"

t_doors	*ft_lstnew_doors(int x, int y)
{
	t_doors	*stack_2;

	stack_2 = (t_doors *)malloc(sizeof(t_doors));
	if (!stack_2)
		return (NULL);
	stack_2->x = x;
	stack_2->y = y;
	stack_2->next = NULL;
	return (stack_2);
}

void	ft_lstaddback_doors(t_doors **head, t_doors *node)
{
	t_doors	*last;

	if (*head == NULL)
	{
		*head = node;
		return ;
	}
	if (!head || !node)
		return ;
	last = *head;
	while (last->next)
		last = last->next;
	last->next = node;
}
int	ft_doors_size(t_doors *head)
{
	int	i;

	i = 0;
	while (head)
	{
		i++;
		head = head->next;
	}
	return (i);
}
void	ft_lstdel_doors(t_doors *lst)
{
	if (!lst)
		return ;
	free(lst);
}
void	ft_lstclear_doors(t_doors **lst)
{
	t_doors	*cur;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		cur = *lst;
		*lst = (*lst)->next;
		if (cur)
			ft_lstdel_doors(cur);
	}
}
