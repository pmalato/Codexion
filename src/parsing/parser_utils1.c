/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmalato <pmalato@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 12:24:35 by pmalato           #+#    #+#             */
/*   Updated: 2026/07/20 17:44:51 by pmalato          ###   ########.fr       */
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
	return (res);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i])
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
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

#include <stdio.h>
int	main(void)
{
	printf("%d\n", is_fifo_or_edf("fifo"));
	printf("%d\n", is_fifo_or_edf("FIFO"));
	printf("%d\n", is_fifo_or_edf("fiffo"));
	printf("%d\n", is_fifo_or_edf("edf"));
	printf("%d\n", is_fifo_or_edf("EDF"));
	return (0);
}