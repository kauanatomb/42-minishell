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

SRCS     := src/main/main.c src/main/utils.c src/main/struct.c \
		   	src/signal/handler.c src/lexer/lexer.c src/lexer/tokenizer.c \
			src/lexer/utils.c src/parser/parser.c src/parser/utils.c src/parser/utils2.c \
			src/expandvar/main_expand.c src/expandvar/utils.c \
			src/expandvar/var_expand.c src/exec/cmd.c \
			src/exec/builtins/external.c src/exec/builtins/cd.c \
			src/exec/builtins/export.c src/exec/builtins/exit.c \
			src/exec/builtins/export2.c src/exec/builtins/cd2.c \
			src/exec/builtins/unset.c src/exec/redir.c \
			src/exec/external_program/exec.c src/exec/pipeline.c \
			src/exec/builtins/export3.c src/exec/heredoc.c \

OBJS     := $(SRCS:.c=.o)

CC      := cc
CFLAGS  := -Wall -Wextra -Werror
INCLUDES := -Iincludes -Ilibft

all: $(NAME)

$(NAME): $(OBJS) libft/libft.a
	$(CC) $(CFLAGS) $(OBJS) -Llibft -lft -lreadline -o $(NAME)

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