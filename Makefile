# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/21 05:24:56 by qpeng             #+#    #+#              #
#    Updated: 2019/04/21 05:43:52 by qpeng            ###   ########.fr        #
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
	gcc -Iinclude $(FLAGS) $(SRCS)
	gcc $(OBJ) -o $(NAME)
clean: $(OBJ)
	rm $(OBJ)
fclean:
	rm $(NAME)

re: clean all