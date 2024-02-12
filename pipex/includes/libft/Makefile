NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror

PRINTF = ./printf
GNL = ./get_next_line

CFILES = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
         ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
		 ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
		 ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
		 ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
		 ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
		 ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
		 ft_sqrt.c ft_atoll.c $(PRINTF)/ft_printf.c $(PRINTF)/string.c $(PRINTF)/number.c \
		 $(PRINTF)/hexa.c $(PRINTF)/address.c $(GNL)/get_next_line.c

BONUS_CFILES = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c \
			   ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c $(GNL)/get_next_line_bonus.c \

OFILES = $(CFILES:.c=.o)

BONUS_OFILES = $(BONUS_CFILES:.c=.o)

all: $(NAME)

$(NAME) : $(OFILES) $(BONUS_OFILES)
	@ar rcs $(NAME) $(OFILES) $(BONUS_OFILES)

bonus: $(BONUS_OFILES)
	@ar rcs $(NAME) $(BONUS_OFILES)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OFILES) $(BONUS_OFILES)

fclean: clean 
	@rm  -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus