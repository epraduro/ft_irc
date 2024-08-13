# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ogregoir <ogregoir@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/08 14:24:51 by rgreiner          #+#    #+#              #
#    Updated: 2024/08/13 17:35:02 by ogregoir         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS    =	ft_irc.cpp \
			Client.cpp \
			Channel.cpp \
			operation_channels.cpp \
			channel_mode.cpp \
			PrivMsg.cpp \

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

.PHONY: all clean fclean re