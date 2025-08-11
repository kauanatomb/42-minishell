# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ktombola <ktombola@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/11 17:32:59 by ktombola          #+#    #+#              #
#    Updated: 2025/08/11 17:50:29 by ktombola         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    := minishell

SRCS     := src/main.c

OBJS     := $(SRCS:.c=.o)

CC      := cc
CFLAGS  := -Wall -Wextra -Werror
INCLUDES := -Iincludes -Ilibft

all: $(NAME)

$(NAME): $(OBJS) libft/libft.a
	$(CC) $(CFLAGS) $(OBJS) -Llibft -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

libft/libft.a:
	make -C libft

clean:
	rm -f $(OBJS)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re