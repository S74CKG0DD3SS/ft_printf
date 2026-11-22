#include "ft_printf.h"

int	dispatch(char spec, va_list *ap)
{
	if (spec == 'c')
		return (ft_putchar(va_arg(*ap, int)));
	if (spec == 's')
		return (ft_putstr(va_arg(*ap, char *)));
	if (spec == 'p')
		return (hex_format((unsigned long)va_arg(*ap, void *)));
	if (spec == 'd' || spec == 'i')
		return (ft_putnbr(va_arg(*ap, int)));
	if (spec == 'u')
		return (ft_putnbr_un(va_arg(*ap, unsigned int)));
	if (spec == 'x')
		return (ft_hex(va_arg(*ap, unsigned int), "0123456789abcdef"));
	if (spec == 'X')
		return (ft_hex(va_arg(*ap, unsigned int), "0123456789ABCDEF"));
	if (spec == '%')
		return (ft_putchar('%'));
	return (-1);
}