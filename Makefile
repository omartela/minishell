# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/03 22:33:20 by irychkov          #+#    #+#              #
#    Updated: 2024/11/04 13:06:39 by irychkov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = src/main.c \
		src/process_input.c src/process_input_utilities.c \
		src/process_input_utilities2.c \
		src/split_args.c src/split_args_utilities.c \
		src/init_pipes.c src/init_cmd.c \
		src/init_shell.c src/init_shell_utilities.c \
		src/here_doc.c src/here_doc_utilities.c src/here_doc_utilities2.c \
		src/parse_add_spaces.c \
		src/parse_redirect.c src/parse_redirect_utilities.c \
		src/check_syntax.c \
		src/check_syntax_utilities.c src/check_syntax_utilities2.c\
		src/parse_trim_spaces.c \
		src/exec_child.c src/exec_in_parent.c \
		src/exec_cmd.c src/exec_pipes.c \
		src/errors.c src/sys_errors.c\
		src/free.c src/free_cmd.c src/free_shell.c \
		src/environment.c \
		src/builtin-export.c \
		src/builtin-export-argument-validation.c \
		src/builtin-export-utilities.c \
		src/utilities.c src/builtin-env.c \
		src/cd-command.c \
		src/cd-command-utilities.c \
		src/builtin-pwd.c \
		src/builtin-unset.c \
		src/builtin-echo.c \
		src/builtin-exit.c \
		src/expand-dollar.c \
		src/expand.c \
		src/expand_tilde.c \
		src/expand_utilities.c \
		src/signals.c src/signals_utilities.c \
		src/update-underscore-variable.c \
		src/set_environment.c \
		src/init_env.c \
		src/environment_utilities.c \
		src/update_shlvl.c \
		src/execute-builtin-commands.c \
		src/execute-builtin-commands-utilities.c \
		src/execute-builtin-exit-cd-export-env.c \

OBJS = $(SRCS:src/%.c=src/%.o)

HEADERS = -Iinclude -I$(LIBFT_DIR)

LIBFT_DIR = ./libft

LIBFT = $(LIBFT_DIR)/libft.a

CFLAGS = -Wall -Wextra -Werror
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
