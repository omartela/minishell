# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: irychkov <irychkov@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/22 16:29:24 by irychkov          #+#    #+#              #
#    Updated: 2024/06/12 21:14:47 by irychkov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC			= ./ft_atoi.c ./ft_bzero.c ./ft_calloc.c ./ft_isalnum.c \
			./ft_isalpha.c ./ft_isascii.c ./ft_isdigit.c ./ft_isprint.c \
			./ft_itoa.c ./ft_memchr.c ./ft_memcmp.c ./ft_memcpy.c \
			./ft_memmove.c ./ft_memset.c ./ft_putchar_fd.c ./ft_putendl_fd.c \
			./ft_putnbr_fd.c ./ft_putstr_fd.c ./ft_split.c ./ft_strchr.c \
			./ft_strdup.c ./ft_striteri.c ./ft_strjoin.c ./ft_strlcat.c \
			./ft_strlcpy.c ./ft_strlen.c ./ft_strmapi.c ./ft_strncmp.c \
			./ft_strnstr.c ./ft_strrchr.c ./ft_strtrim.c ./ft_substr.c \
			./ft_tolower.c ./ft_toupper.c \
			./ft_lstadd_back_bonus.c ./ft_lstadd_front_bonus.c \
			./ft_lstclear_bonus.c ./ft_lstdelone_bonus.c ./ft_lstiter_bonus.c \
			./ft_lstlast_bonus.c ./ft_lstmap_bonus.c ./ft_lstnew_bonus.c \
			./ft_lstsize_bonus.c \
			./get_next_line_bonus.c ./get_next_line_utils_bonus.c

PRINTF_DIR	= printf

PRINTF_SRC	= $(PRINTF_DIR)/ft_printf.c $(PRINTF_DIR)/ft_print_char.c \
			  $(PRINTF_DIR)/ft_print_str.c $(PRINTF_DIR)/ft_print_nbr.c \
			  $(PRINTF_DIR)/ft_print_unsnbr.c $(PRINTF_DIR)/ft_print_hex.c \
			  $(PRINTF_DIR)/ft_print_ptr.c

OBJS		= $(SRC:.c=.o) $(PRINTF_SRC:.c=.o)

NAME		= libft.a

AR			= ar rcs

CC			= cc

CFLAGS		= -Wall -Wextra -Werror

all:		$(NAME)

.c.o:	
			$(CC) $(CFLAGS) -c -I. -I./printf/ $< -o $@

$(NAME):	$(OBJS)
			$(AR) $(NAME) $(OBJS)

clean:	
			rm -f $(OBJS) $(PRINTF_SRC:.c=.o)

fclean:		clean
			rm -f $(NAME)

re:			fclean all

.PHONY:	all clean fclean re