# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/03 22:33:20 by irychkov          #+#    #+#              #
#    Updated: 2024/10/23 22:34:12 by irychkov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = src/main.c src/split_args.c src/split_args_helper.c \
		src/init_pipes.c src/init_cmd.c src/init_shell.c \
		src/here_doc.c src/here_doc2.c \
		src/parse_add_spaces.c \
		src/parse_redirect.c src/parse_redirect2.c \
		src/check_syntax.c src/check_syntax2.c src/check_syntax3.c\
		src/parse_trim_spaces.c \
		src/exec_child.c src/exec_in_parent.c \
		src/exec_cmd.c src/exec_pipes.c \
		src/errors.c src/sys_errors.c\
		src/free.c src/free_cmd.c src/free_shell.c \
		src/test_split.c src/test_parse_redirect.c \
		src/environment.c src/builtin-export.c \
		src/utilities.c src/builtin-env.c \
		src/cd-command.c \
		src/builtin-pwd.c \
		src/builtin-unset.c \
		src/builtin-echo.c \
		src/builtin-exit.c \
		src/expand-dollar.c \
		src/expand.c \
		src/expand_tilde.c \
		src/expand_utilities.c \
		src/signals.c \
		src/update-underscore-variable.c \
		src/set_environment.c \
		src/init_env.c \
		src/environment_utilities.c \
		src/update_shlvl.c \
		src/execute-builtin-commands.c \

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
