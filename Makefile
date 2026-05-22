# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/20 14:22:45 by inaciri           #+#    #+#              #
#    Updated: 2026/05/22 15:34:41 by inaciri          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRCS = main.c \
		parse.c \

OBJS := ${SRCS:.c=.o}

NAME = codexion

all: ${NAME}

${NAME}: ${OBJS}
	@echo Compiling...
	@${CC} ${CFLAGS} ${OBJS} -o ${NAME}

%.o: %.c
	@${CC} ${CFLAGS} -c $< -o $@
	
run:
	./$(NAME)
clean :
	rm -rf ${OBJS}

fclean : clean
	rm -rf ${NAME}

re : fclean all

.PHONY: all clean fclean re run