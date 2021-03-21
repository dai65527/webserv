# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/05 17:34:06 by dnakano           #+#    #+#              #
#    Updated: 2021/03/19 23:25:00 by dhasegaw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX			:=	clang++
CPPFLAGS	:=	-Wall -Wextra -Werror

SRCS		:=	main.cpp Webserv.cpp Sessions.cpp Socket.cpp Request.cpp Response.cpp CgiHandler.cpp
OBJS		:=	$(SRCS:%.cpp=%.o)
NAME		:=	nginDX
OUTDIR		:=	.

.PHONY:		all
all:		$(NAME)

$(NAME):	$(OBJS)
			$(CXX) $(CPPFLAGS) $(OBJS) -o $(NAME)

.PHONY:		unittest
unittest:
			$(CXX) $(SRCS) -o $(NAME) -D UNIT_TEST

.PHONY:		test
test:		$(NAME)
			$(OUTDIR)/$(NAME)

.PHONY:		clean
clean:
			rm -f $(OBJS)

.PHONY:		fclean
fclean:		clean
			rm -f $(NAME)

.PHONY:		re
re:			fclean all
