#define __stm32_platform_set_attribute__(param) __attribute__(param)

#define __stm32_platform_section__(param)       __stm32_platform_set_attribute__((section(param)))

#define __stm32_platform_aligned__(param)       __stm32_platform_set_attribute__((aligned (param)))

#define __stm32_platform_at__(param)            __stm32_platform_set_attribute__((at(param)))

#define __stm32_platform_format__(fmt, m, n)    __stm32_platform_set_attribute__((format(fmt, m, n)))

typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

struct USART_TypeDef
{
    uint16_t SR;
    uint16_t RESERVED0;
    uint16_t DR;
    uint16_t RESERVED1;
    uint16_t BRR;
    uint16_t RESERVED2;
    uint16_t CR1;
    uint16_t RESERVED3;
    uint16_t CR2;
    uint16_t RESERVED4;
    uint16_t CR3;
    uint16_t RESERVED5;
    uint16_t GTPR;
    uint16_t RESERVED6;
};
#define ZEROPAD    1        /* pad with zero */
#define SIGN    2        /* unsigned/signed long */
#define PLUS    4        /* show plus */
#define SPACE    8        /* space if plus */
#define LEFT    16        /* left justified */
#define SMALL    32        /* Must be 32 == 0x20 */
#define SPECIAL    64        /* 0x */
#define noinline __attribute__((noinline))

#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< SRAM base address in the bit-band region */
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define USART1_BASE           (APB2PERIPH_BASE + 0x3800)
#define USART1              ((volatile struct USART_TypeDef *) USART1_BASE)

#define _U      0x01    /* upper */
#define _L      0x02    /* lower */
#define _D      0x04    /* digit */
#define _C      0x08    /* cntrl */
#define _P      0x10    /* punct */
#define _S      0x20    /* white space (space/lf/tab) */
#define _X      0x40    /* hex digit */
#define _SP     0x80    /* hard space (0x20) */

unsigned char _ctype[] = {
_C,_C,_C,_C,_C,_C,_C,_C,            /* 0-7 */
_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,        /* 8-15 */
_C,_C,_C,_C,_C,_C,_C,_C,            /* 16-23 */
_C,_C,_C,_C,_C,_C,_C,_C,            /* 24-31 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,            /* 32-39 */
_P,_P,_P,_P,_P,_P,_P,_P,            /* 40-47 */
_D,_D,_D,_D,_D,_D,_D,_D,            /* 48-55 */
_D,_D,_P,_P,_P,_P,_P,_P,            /* 56-63 */
_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,    /* 64-71 */
_U,_U,_U,_U,_U,_U,_U,_U,            /* 72-79 */
_U,_U,_U,_U,_U,_U,_U,_U,            /* 80-87 */
_U,_U,_U,_P,_P,_P,_P,_P,            /* 88-95 */
_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,    /* 96-103 */
_L,_L,_L,_L,_L,_L,_L,_L,            /* 104-111 */
_L,_L,_L,_L,_L,_L,_L,_L,            /* 112-119 */
_L,_L,_L,_P,_P,_P,_P,_C,            /* 120-127 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        /* 128-143 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        /* 144-159 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,   /* 160-175 */
_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,       /* 176-191 */
_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,       /* 192-207 */
_U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,       /* 208-223 */
_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,       /* 224-239 */
_L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L};      /* 240-255 */

static unsigned int strnlen(const char * s,  unsigned int  count)
{
    const char *sc;

    for (sc = s; count-- && *sc != '\0'; ++sc)
        /* nothing */;
    return sc - s;
}

static char *string(char *buf, char *s, int field_width, int precision, int flags)
{
    int len, i;

    if (s == 0)
        s = "<NULL>";

    len = strnlen(s, precision);

    if (!(flags & LEFT))
        while (len < field_width--)
            *buf++ = ' ';
    for (i = 0; i < len; ++i)
        *buf++ = *s++;
    while (len < field_width--)
        *buf++ = ' ';
    return buf;
}

static void serial_putc(const char c)
{
    #define USART_SR_TC            ((uint16_t)0x0040)
    while((USART1->SR & USART_SR_TC) == 0);
    USART1->DR = (c & (unsigned short)0x01FF);
}

static void puts(const char *s)
{
    while (*s) {
        serial_putc(*s++);
    }
}


#define do_div(n,base) ({                \
    unsigned int __base = (base);            \
    unsigned int __rem;                    \
    (void)(((typeof((n)) *)0) == ((unsigned long *)0));    \
    if (((n) >> 32) == 0) {            \
        __rem = (unsigned int)(n) % __base;        \
        (n) = (unsigned int)(n) / __base;        \
    } else                        \
        __rem = __div64_32(&(n), __base);    \
    __rem;                        \
 })

 
unsigned int __div64_32(unsigned long *n, unsigned int base)
{
    unsigned long rem = *n;
    unsigned long b = base;
    unsigned long res, d = 1;
    unsigned int high = rem >> 32;

    /* Reduce the thing a bit first */
    res = 0;
    if (high >= base) {
        high /= base;
        res = (unsigned long) high << 32;
        rem -= (unsigned long) (high*base) << 32;
    }

    while ((signed long)b > 0 && b < rem) {
        b = b+b;
        d = d+d;
    }

    do {
        if (rem >= b) {
            rem -= b;
            res += d;
        }
        b >>= 1;
        d >>= 1;
    } while (d);

    *n = res;
    return rem;
}
#define is_digit(c)    ((c) >= '0' && (c) <= '9')

static int skip_atoi(const char **s)
{
    int i=0;

    while (is_digit(**s))
        i = i*10 + *((*s)++) - '0';
    return i;
}
static char* put_dec_trunc(char *buf, unsigned q)
{
    unsigned d3, d2, d1, d0;
    d1 = (q>>4) & 0xf;
    d2 = (q>>8) & 0xf;
    d3 = (q>>12);

    d0 = 6*(d3 + d2 + d1) + (q & 0xf);
    q = (d0 * 0xcd) >> 11;
    d0 = d0 - 10*q;
    *buf++ = d0 + '0'; /* least significant digit */
    d1 = q + 9*d3 + 5*d2 + d1;
    if (d1 != 0) {
        q = (d1 * 0xcd) >> 11;
        d1 = d1 - 10*q;
        *buf++ = d1 + '0'; /* next digit */

        d2 = q + 2*d2;
        if ((d2 != 0) || (d3 != 0)) {
            q = (d2 * 0xd) >> 7;
            d2 = d2 - 10*q;
            *buf++ = d2 + '0'; /* next digit */

            d3 = q + 4*d3;
            if (d3 != 0) {
                q = (d3 * 0xcd) >> 11;
                d3 = d3 - 10*q;
                *buf++ = d3 + '0';  /* next digit */
                if (q != 0)
                    *buf++ = q + '0';  /* most sign. digit */
            }
        }
    }
    return buf;
}
static char* put_dec_full(char *buf, unsigned q)
{
    /* BTW, if q is in [0,9999], 8-bit ints will be enough, */
    /* but anyway, gcc produces better code with full-sized ints */
    unsigned d3, d2, d1, d0;
    d1 = (q>>4) & 0xf;
    d2 = (q>>8) & 0xf;
    d3 = (q>>12);

    /*
     * Possible ways to approx. divide by 10
     * gcc -O2 replaces multiply with shifts and adds
     * (x * 0xcd) >> 11: 11001101 - shorter code than * 0x67 (on i386)
     * (x * 0x67) >> 10:  1100111
     * (x * 0x34) >> 9:    110100 - same
     * (x * 0x1a) >> 8:     11010 - same
     * (x * 0x0d) >> 7:      1101 - same, shortest code (on i386)
     */

    d0 = 6*(d3 + d2 + d1) + (q & 0xf);
    q = (d0 * 0xcd) >> 11;
    d0 = d0 - 10*q;
    *buf++ = d0 + '0';
    d1 = q + 9*d3 + 5*d2 + d1;
        q = (d1 * 0xcd) >> 11;
        d1 = d1 - 10*q;
        *buf++ = d1 + '0';

        d2 = q + 2*d2;
            q = (d2 * 0xd) >> 7;
            d2 = d2 - 10*q;
            *buf++ = d2 + '0';

            d3 = q + 4*d3;
                q = (d3 * 0xcd) >> 11; /* - shorter code */
                /* q = (d3 * 0x67) >> 10; - would also work */
                d3 = d3 - 10*q;
                *buf++ = d3 + '0';
                    *buf++ = q + '0';
    return buf;
}

static noinline char* put_dec(char *buf, unsigned long long num)
{
    while (1) {
        unsigned  long long  rem;
        if (num < 100000)
            return put_dec_trunc(buf, num);
        rem = do_div(num, 100000);
        buf = put_dec_full(buf, rem);
    }
}
static char *number(char *buf, unsigned long long num, int base, int size, int precision, int type)
{
    /* we are called with base 8, 10 or 16, only, thus don't need "G..."  */
    static const char digits[16] = "0123456789ABCDEF"; /* "GHIJKLMNOPQRSTUVWXYZ"; */

    char tmp[66];
    char sign;
    char locase;
    int need_pfx = ((type & SPECIAL) && base != 10);
    int i;

    /* locase = 0 or 0x20. ORing digits or letters with 'locase'
     * produces same digits or (maybe lowercased) letters */
    locase = (type & SMALL);
    if (type & LEFT)
        type &= ~ZEROPAD;
    sign = 0;
    if (type & SIGN) {
        if ((signed long long) num < 0) {
            sign = '-';
            num = - (signed  long long) num;
            size--;
        } else if (type & PLUS) {
            sign = '+';
            size--;
        } else if (type & SPACE) {
            sign = ' ';
            size--;
        }
    }
    if (need_pfx) {
        size--;
        if (base == 16)
            size--;
    }

    /* generate full string in tmp[], in reverse order */
    i = 0;
    if (num == 0)
        tmp[i++] = '0';
    /* Generic code, for any base:
    else do {
        tmp[i++] = (digits[do_div(num,base)] | locase);
    } while (num != 0);
    */
    else if (base != 10) { /* 8 or 16 */
        int mask = base - 1;
        int shift = 3;
        if (base == 16) shift = 4;
        do {
            tmp[i++] = (digits[((unsigned char)num) & mask] | locase);
            num >>= shift;
        } while (num);
    } else { /* base 10 */
        i = put_dec(tmp, num) - tmp;
    }

    /* printing 100 using %2d gives "100", not "00" */
    if (i > precision)
        precision = i;
    /* leading space padding */
    size -= precision;
    if (!(type & (ZEROPAD+LEFT)))
        while(--size >= 0)
            *buf++ = ' ';
    /* sign */
    if (sign)
        *buf++ = sign;
    /* "0x" / "0" prefix */
    if (need_pfx) {
        *buf++ = '0';
        if (base == 16)
            *buf++ = ('X' | locase);
    }
    /* zero or space padding */
    if (!(type & LEFT)) {
        char c = (type & ZEROPAD) ? '0' : ' ';
        while (--size >= 0)
            *buf++ = c;
    }
    /* hmm even more zero padding? */
    while (i <= --precision)
        *buf++ = '0';
    /* actual digits of result */
    while (--i >= 0)
        *buf++ = tmp[i];
    /* trailing space padding */
    while (--size >= 0)
        *buf++ = ' ';
    return buf;
}
static char *pointer(const char *fmt, char *buf, void *ptr, int field_width, int precision, int flags)
{
    if (!ptr)
        return string(buf, "(null)", field_width, precision, flags);
    flags |= SMALL;
    if (field_width == -1) {
        field_width = 2*sizeof(void *);
        flags |= ZEROPAD;
    }
    return number(buf, (unsigned long) ptr, 16, field_width, precision, flags);
}
int stm32_vsprintf(char *buf, const char *fmt, __builtin_va_list args)
{
    unsigned long long num;
    int base;
    char *str;
    int flags;
    int field_width;
    int precision;

    int qualifier;

    str = buf;

    for (; *fmt ; ++fmt) {
        if (*fmt != '%') {
            *str++ = *fmt;
            continue;
        }


        flags = 0;
        repeat:
            ++fmt;
            switch (*fmt) {
                case '-': flags |= 16; goto repeat;
                case '+': flags |= 4; goto repeat;
                case ' ': flags |= 8; goto repeat;
                case '#': flags |= 64; goto repeat;
                case '0': flags |= 1; goto repeat;
            }


        field_width = -1;
        if (((*fmt) >= '0' && (*fmt) <= '9'))
            field_width = skip_atoi(&fmt);
        else if (*fmt == '*') {
            ++fmt;

            field_width = __builtin_va_arg(args,int);
            if (field_width < 0) {
                field_width = -field_width;
                flags |= 16;
            }
        }


        precision = -1;
        if (*fmt == '.') {
            ++fmt;
            if (((*fmt) >= '0' && (*fmt) <= '9'))
                precision = skip_atoi(&fmt);
            else if (*fmt == '*') {
                ++fmt;

                precision = __builtin_va_arg(args,int);
            }
            if (precision < 0)
                precision = 0;
        }


        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
            *fmt == 'Z' || *fmt == 'z' || *fmt == 't') {
            qualifier = *fmt;
            ++fmt;
            if (qualifier == 'l' && *fmt == 'l') {
                qualifier = 'L';
                ++fmt;
            }
        }


        base = 10;

        switch (*fmt) {
        case 'c':
            if (!(flags & 16))
                while (--field_width > 0)
                    *str++ = ' ';
            *str++ = (unsigned char) __builtin_va_arg(args,int);
            while (--field_width > 0)
                *str++ = ' ';
            continue;

        case 's':
            str = string(str, __builtin_va_arg(args,char *), field_width, precision, flags);
            continue;

        case 'p':
            str = pointer(fmt+1, str,
                    __builtin_va_arg(args,void *),
                    field_width, precision, flags);

            while ((((_ctype[(int)(unsigned char)(fmt[1])])&(0x01|0x02|0x04)) != 0))
                fmt++;
            continue;

        case 'n':
            if (qualifier == 'l') {
                long * ip = __builtin_va_arg(args,long *);
                *ip = (str - buf);
            } else {
                int * ip = __builtin_va_arg(args,int *);
                *ip = (str - buf);
            }
            continue;

        case '%':
            *str++ = '%';
            continue;


        case 'o':
            base = 8;
            break;

        case 'x':
            flags |= 32;
        case 'X':
            base = 16;
            break;

        case 'd':
        case 'i':
            flags |= 2;
        case 'u':
            break;

        default:
            *str++ = '%';
            if (*fmt)
                *str++ = *fmt;
            else
                --fmt;
            continue;
        }
        if (qualifier == 'L')
            num = __builtin_va_arg(args,unsigned long long);
        else if (qualifier == 'l') {
            num = __builtin_va_arg(args,unsigned long);
            if (flags & 2)
                num = (signed long) num;
        } else {
            num = __builtin_va_arg(args,unsigned int);
            if (flags & 2)
                num = (signed int) num;
        }
        str = number(str, num, base, field_width, precision, flags);
    }
    *str = '\0';
    return str-buf;
}

static  void stm32_put(const char *fmt, ...)
{
    __builtin_va_list args;
    char printbuffer[256];
    __builtin_va_start(args,fmt);
    stm32_vsprintf(printbuffer, fmt, args);
    __builtin_va_end(args);
    puts(printbuffer);
}

__stm32_platform_section__(".isr_vector") void _start(int zero, int arch, unsigned int params)
{
    stm32_put("arch %d\n", arch);
    stm32_put("parms 0x%08x\n", params);
    while(1) {

    }
}
