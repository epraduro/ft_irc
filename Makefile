# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgreiner <rgreiner@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/08 14:24:51 by rgreiner          #+#    #+#              #
#    Updated: 2024/05/21 18:17:43 by rgreiner         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS    =	test.cpp \
			Client.cpp \

OBJS    =   ${SRCS:.c=.o}
RM      =   rm -f
CFLAGS  =   -Wall -Wextra -Werror -std=c++98
NAME	= 	ircserv
.cpp.o:
		c++ ${CFLAGS} -c $< -o $@
${NAME}:	${OBJS}
	c++ ${CFLAGS} ${OBJS} -o ${NAME}
all:	${NAME}
clean:
	${RM} ${NAME}
fclean: clean
	${RM} ${NAME}
re:	fclean all
.PHONY: bonus all clean fclean re