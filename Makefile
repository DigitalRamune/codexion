# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inaciri <inaciri@student.42mulhouse.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/20 14:22:45 by inaciri           #+#    #+#              #
#    Updated: 2026/06/24 14:43:42 by inaciri          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = sim/main.c \
		utils/parse.c \
		utils/heap_utils_2.c \
		utils/heap_utils.c \
		sim/simulation.c \
		dongle/dongle.c \
		utils/time.c \
		sim/coders.c \
		utils/edge_case.c \
		utils/burn.c \
		sim/monitor.c \
		dongle/dongle_utils.c \
		utils/free.c \
		utils/print.c

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