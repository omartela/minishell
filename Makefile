# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/03 22:33:20 by irychkov          #+#    #+#              #
#    Updated: 2024/09/06 15:10:50 by irychkov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = src/main.c src/ft_split_args.c src/exec_pipes.c \
		src/free.c src/init_cmd.c src/parse_redirect.c\
		src/test_split.c src/test_parse_redirect.c

OBJS = $(SRCS:src/%.c=src/%.o)

HEADERS = -Iinclude -I$(LIBFT_DIR)

LIBFT_DIR = ./libft

LIBFT = $(LIBFT_DIR)/libft.a

CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline

CC = cc

all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)
	@echo "Compiling: $<"

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(OBJS) $(LIBFT) -o $(NAME) $(LDFLAGS)

clean:
	@rm -rf $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -rf $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re libft