#include "ft_printf.h"

static int	parse_percent(const char *format, int i, va_list *ap)
{
	if (format[i + 1] == '\0')
		return (-1);
	return (dispatch(format[i + 1], ap));
}

static int	parsing(const char *format, va_list *ap)
{
	int	i;
	int	check;
	int	total;

	i = 0;
	total = 0;
	while (format[i])
	{
		if (format[i] != '%')
		{
			check = ft_putchar(format[i]);
			i++;
		}
		else
		{
			check = parse_percent(format, i, ap);
			i += 2;
		}
		if (check == -1)
			return (-1);
		total += check;
	}
	return (total);
}

int	ft_printf(const char *format, ...)
{
	int		total;
	va_list	ap;

	va_start(ap, format);
	total = parsing(format, &ap);
	va_end(ap);
	return (total);
}