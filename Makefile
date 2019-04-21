# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/21 05:24:56 by qpeng             #+#    #+#              #
#    Updated: 2019/04/21 05:46:06 by qpeng            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ping
FLAGS = -Wall -Werror -Wextra -c
FILES = helper.c\
		icmp.c\
		main.c\
		readloop.c
SRCS = $(addprefix src/, $(FILES))
OBJ = $(FILES:.c=.o)

all:
	@gcc -Iinclude $(FLAGS) $(SRCS)
	@gcc $(OBJ) -o $(NAME)
clean: 
	@rm *.o
fclean:
	@rm $(NAME)

re: clean fclean all

.PHONY: all, $(NAME), clean, fclean, re