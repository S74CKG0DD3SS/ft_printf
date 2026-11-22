#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

int	ft_printf(const char *format, ...);
int	ft_putchar(char c);
int	ft_putstr(char *str);
int	ft_putnbr_un(unsigned int n);
int	ft_putnbr(int n);
int	hex_format(unsigned long n);
int	ft_hex(unsigned long n, const char *base);
int	dispatch(char spec, va_list *ap);

#endif

