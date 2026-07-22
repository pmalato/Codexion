/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 12:24:35 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/22 11:28:18 by pmalato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../codexion.h"

int	is_nmb_of_args_valid(int ac)
{
	if (ac != 9)
		return (0);
	else
		return (1);
}

int	codex_atoi(char *str)
{
	long	res;
	int		i;

	res = 0;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	if (res > 2147483647)
		return (0);
	return (res);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i] || s2[j])
	{
		if (s1[i] != s2[j])
			return (0);
		i++;
		j++;
	}
	return (1);
}

int	is_numeric_arg_valid(char *str)
{
	int	test;

	test = codex_atoi(str);
	if (!test)
		return (0);
	else
		return (test);
}

int	is_fifo_or_edf(char *str)
{
	int	test1;
	int	test2;

	test1 = ft_strcmp(str, "fifo");
	test2 = ft_strcmp(str, "edf");
	if (!(test1 || test2))
		return (0);
	return (1);
}
