#include "ft_printf.h"

int	ft_hex(unsigned long n, const char *base)
{
	int	count;
	int	check;

	count = 0;
	if (n >= 16)
	{
		check = ft_hex(n / 16, base);
		if (check == -1)
			return (-1);
		count += check;
	}
	if (ft_putchar(base[n % 16]) == -1)
		return (-1);
	return (count + 1);
}

int	hex_format(unsigned long n)
{
	int	count;
	int	check;

	if (n == 0)
		return (ft_putstr("(nil)"));
	count = ft_putstr("0x");
	if (count == -1)
		return (-1);
	check = ft_hex(n, "0123456789abcdef");
	if (check == -1)
		return (-1);
	return (count + check);
}