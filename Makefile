# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omartela <omartela@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/03 22:33:20 by irychkov          #+#    #+#              #
#    Updated: 2024/10/07 16:51:26 by omartela         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = src/main.c src/split_args.c src/split_args_helper.c \
		src/init_pipes.c src/init_cmd.c src/here_doc.c \
		src/ft_update_args.c src/parse_redirect.c \
		src/check_syntax.c src/trim_spaces.c \
		src/exec_cmd.c src/exec_pipes.c \
		src/errors.c src/sys_errors.c\
		src/free.c \
		src/test_split.c src/test_parse_redirect.c \
		src/environment.c src/builtin-export.c \
		src/utilities.c src/builtin-env.c \
		src/cd-command.c \
		src/builtin-pwd.c \
		src/builtin-unset.c \
		src/builtin-echo.c \
		src/builtin-exit.c \
		src/expand-dollar.c \
		src/get_pid.c \

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
