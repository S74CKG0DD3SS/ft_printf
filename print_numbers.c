#include "ft_printf.h"

int	ft_putnbr_un(unsigned int n)
{
	int	count;
	int	check;

	count = 0;
	if (n >= 10)
	{
		check = ft_putnbr_un(n / 10);
		if (check == -1)
			return (-1);
		count += check;
	}
	if (ft_putchar((n % 10) + '0') == -1)
		return (-1);
	return (count + 1);
}

int	ft_putnbr(int n)
{
	int		count;
	int		check;
	long	nb;

	count = 0;
	nb = n;
	if (nb < 0)
	{
		if (ft_putchar('-') == -1)
			return (-1);
		count++;
		nb = -nb;
	}
	check = ft_putnbr_un(nb);
	if (check == -1)
		return (-1);
	return (count + check);
}