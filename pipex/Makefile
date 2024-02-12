# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/11 12:48:59 by cgerling          #+#    #+#              #
#    Updated: 2024/02/09 14:00:58 by cgerling         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

LIBFT = ./includes/libft
SRC = ./src
OBJDIR = ./obj
OBJDIR_BONUS = ./obj_bonus

SRCF = $(SRC)/pipex.c $(SRC)/utils.c $(SRC)/split.c
SRCFB = $(SRC)/here_doc.c $(SRC)/pipex.c $(SRC)/utils.c $(SRC)/split.c
OBJF = $(addprefix $(OBJDIR)/, $(SRCF:.c=.o))
OBJFB = $(addprefix $(OBJDIR_BONUS)/, $(SRCFB:.c=.o))

GREEN=\033[0;32m
NC=\033[0m

all: $(NAME)

$(NAME): $(OBJF)
	@rm -rf $(OBJDIR_BONUS)
	@echo "${GREEN}Compiling pipex${NC}"
	@make -C $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJF) -L $(LIBFT) -lft

bonus: CFLAGS += -D BONUS=1
bonus: $(OBJFB)
	@rm -rf $(OBJDIR)
	@echo "${GREEN}Compiling pipex with BONUS${NC}"
	@make -C $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJFB) -L $(LIBFT) -lft

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR_BONUS)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make -C $(LIBFT) clean
	@rm -rf $(OBJDIR)
	@rm -rf $(OBJDIR_BONUS)

fclean: clean
	@make -C $(LIBFT) fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus