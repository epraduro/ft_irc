# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: epraduro <epraduro@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/08 14:24:51 by rgreiner          #+#    #+#              #
#    Updated: 2024/06/12 14:20:56 by epraduro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS    =	ft_irc.cpp \
			Client.cpp \
			Channel.cpp \
			operation_channels.cpp \
			channel_mode.cpp \

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