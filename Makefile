# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/21 05:24:56 by qpeng             #+#    #+#              #
#    Updated: 2019/05/08 05:39:03 by qpeng            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ping
FLAGS = -Wall -Werror -Wextra -c -g
FILES = helper.c\
		icmp.c\
		main.c\
		readloop.c\
		helper2.c\
		helper3.c
SRCS = $(addprefix src/, $(FILES))
OBJ = $(FILES:.c=.o)

all:
	@gcc -Iinclude $(FLAGS) $(SRCS)
	@gcc $(OBJ) -o $(NAME)
clean:
	@rm -rf *.o
fclean: clean
	@rm -f $(NAME)

re: clean all

.PHONY: all, $(NAME), clean, fclean, re
