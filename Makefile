# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qpeng <qpeng@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/04/21 05:24:56 by qpeng             #+#    #+#              #
#    Updated: 2019/04/21 05:31:23 by qpeng            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ping
FLAGS = -Wall -Werror -Wextra -c
SRC = src/helper.c\
		src/icmp.c\
		src/main.c\
		src/readloop.c
OBJ = $(SRC:.c=.o)

all:
	gcc -Iinclude $(FLAGS) $(SRC)
	gcc $(OBJ) -o $(NAME)
clean:
	rm $(OBJ)
fclean: clean
	rm $(NAME)

re: clean all