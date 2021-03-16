# libft
Re-implementation of some functions in libc and some convenient functions

# available functions
## Re-implementation of libc
Call it as `ft_<function name in libc>`.

### string utilities
- strlen
- strlcpy
- strlcat
- strncmp
- strchr
- strrchr
- strnstr
- strdup
- isalpha
- isupper
- islower
- isdigit
- isalnum
- isascii
- isprint
- isspace
- toupper
- tolower

### memory and array utilities
- bzero
- memset
- memcpy
- memccpy
- memmove
- memchr
- memcmp

### conversion and io function
- atoi
- atof
- itoa
- printf (available conversions: `cspdiuxXnfge%`, flags: `lh-0.*# +’`)

## added function
Refer to subject of 42cursus for detail.

### io utils
- get_next_line: get a new line from input indicated by file discriptor
- ft_putchar_fd: put a ascii charactor to output indicated by file discriptor
- ft_putstr_fd: put a ascii string to output indicated by file discriptor
- ft_putendl_fd: put a ascii string and new line to output indicated by file discriptor
- ft_putnbr_fd: put a integer to output indicated by file discriptor
- ft_putstrn_fd: put a n charactors form ascii string to output indicated by file discriptor

### string utils
- substr: returns newly allocated sub-string from string
- strjoin: connect two strings and returns it
- strtrim: trim specified charactor sets from a string and return it
- split: sprit a string by a charactor and returns a string array as a result
- strmapi: applys a fuction to each charactor of a string and returns the string

### forward list utils
Utility functions to handle a forward list struct.

```c
typedef struct      s_list
{
    void            *content;
    struct s_list   *next;
}                   t_list;
```

- ft_lstnew: create new list
- ft_lstsize: return size of list
- ft_lstlast: return last element of list
- ft_lstadd_front: add new element to first of list
- ft_lstadd_back: add new element to last of list
- ft_lstdelone: delete one element from list
- ft_lstclear: delete all elements of list
- ft_lstiter: apply a function to each element of list
- ft_lstmap: apply a function to each element of list and return a new list as a result
- ft_lstpop: delete last element of list
- ft_lstsort: sort elements of list
- ft_lstrm_if: delete elements if function returns true
- ft_lstdup: dupulicate list

# conpile and use
### compile
Just execute make and libft.a will be created.

```
$ make
```

### how to use
Include `libft.h` and conpile with `libft.a`.

```c
/* main.c */
#include "libft.h"

int main(void)
{
    ft_printf("hello world!\n");
    return (0);
}
```

```
$ gcc -L. -lft main.c
$ ./a.out
hello world!
```

# note
This is result of 42cursus project. But some fuctions are bonus and some are original.
