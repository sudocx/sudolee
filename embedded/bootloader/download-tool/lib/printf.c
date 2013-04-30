#include "type.h"
#include "print.h"
#include "uart.h"

/* Needed when link */
extern void *memcpy(void *dest, const void *src, long count);

#define SIGN_NUM	(1 << 0)

#define ADDCH(__str, __ch) 	do{ \
			*(__str) = (__ch); \
			++(__str); \
		} while (0)

/* we use this so that we can do without the ctype library */
#define is_digit(c) (((c) >= '0') && ((c) <= '9'))

static inline char *string(char *buf, char *end, char *s)
{
	if (s == NULL)
		s = "NULL";

	/* the last one for '\0'  */
	while (!((*s == '\0') || ((buf + 1) > end)))
		ADDCH(buf, *s++);

	return buf;
}

static inline char *number(char *buf, char *end, unsigned long long num, int base,
		  int type)
{
	const char digits[] = "0123456789ABCDEF";

	char temp[24];
	int need_prefix = (base != 10);
	int i;

	i = 0;
	if (num == 0) {
		temp[i++] = '0';
	} else if (base != 10) {	/* olny 16 */
		int mask = base - 1;
		/* it's equal to:
		 *      this = (num & 0xff) % base;
		 *      temp[i] = digits[this];
		 *      num /= base;
		 */
		do {
			temp[i++] = (digits[((unsigned char)num) & mask]);
			num >>= 4;
		} while (num);
	}

	/* "-" prefix */
	if (type & SIGN_NUM) {
		if ((long long)num < 0) {
			ADDCH(buf, '-');
			num = -(long long)num;
		}
	}

	/* "0x" / "0" prefix */
	if (need_prefix) {
		ADDCH(buf, '0');
		ADDCH(buf, 'x');
	}

	/* actual digits of result */
	while (--i >= 0)
		ADDCH(buf, temp[i]);

	return buf;
}

//long vsnprintf(char *buf, u32 size, const char *fmt, va_list args)
long vsnprintf(char *buf, u32 size, const char *fmt, va_list args)
{
	unsigned long long num;
	int base, flags;
	char *end, *str;

	str = buf;
	end = buf + size;

	for (; *fmt; ++fmt) {
		if (*fmt != '%') {
			ADDCH(str, *fmt);
			continue;
		}

		++fmt;
		/* process flags */
		/* TODO: other flags */
		flags = 0;

		/* default base */
		base = 16;

		switch (*fmt) {
		case 'c':
			ADDCH(str, (unsigned char)va_arg(args, char));
			continue;

		case 's':
			str = string(str, end, va_arg(args, char *));
			continue;

		case '%':
			ADDCH(str, '%');
			continue;

		case 'd':
			flags |= SIGN_NUM;
		case 'x':
		case 'X':
			break;

		default:
			ADDCH(str, '%');
			if (*fmt)
				ADDCH(str, *fmt);
			else
				--fmt;
			continue;
		}

		num = va_arg(args, unsigned int);
		if (flags & SIGN_NUM)
			num = (signed int)num;

		str = number(str, end, num, base, flags);
	}

	if (size > 0)
		*str = '\0';

	return str - buf;
}

/* TODO: size seem too little */
#define MAX_PRINTBUF_SIZE 80

long serial_printf(int port_num, const char *format, ...)
{
	va_list args;
	long rv;
	char printbuffer[MAX_PRINTBUF_SIZE];

	va_start(args, format);
	rv = vsnprintf(printbuffer, sizeof(printbuffer), format, args);
	va_end(args);

	puts(port_num, printbuffer);

	return rv;
}