/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Created: 14/08/2021 Niklas Neronin
 * Updated: 08/04/2023 Niklas Neronin
 */

static int	ft_get_size(int nbr)
{
	int	size;

	size = 0;
	if (nbr <= 0)
		size++;
	while (nbr != 0) {
		nbr = nbr / 10;
		size++;
	}
	return size;
}

void	size_to_str(char *str, int nbr)
{
	int	i;
	int	size;

	i = 0;
	size = ft_get_size(nbr);
	if (nbr < 0) {
		str[0] = '-';
		i = 1;
		nbr = -nbr;
	}
	str[size] = '\0';
	while (size > i) {
		str[size - 1] = nbr % 10 + '0';
		nbr = nbr / 10;
		size--;
	}
}
