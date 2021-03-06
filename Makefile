# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/05 17:34:06 by dnakano           #+#    #+#              #
#    Updated: 2021/05/08 21:08:00 by dnakano          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX			:=	clang++
CPPFLAGS	:=	-Wall -Wextra -Werror
SRCS		:=	main.cpp \
				webserv_utils.cpp \
				Webserv.cpp \
				Session.cpp \
				Socket.cpp \
				Request.cpp \
				Response.cpp \
				CgiHandler.cpp \
				CommonConfigStore.cpp \
				ConfigParser.cpp \
				LocationConfig.cpp \
				MainConfig.cpp \
				MainOnlyConfigStore.cpp \
				ServerConfig.cpp \
				ServerLocationConfigStore.cpp \
				ServerOnlyConfigStore.cpp \
				CgiParams.cpp \
				UrlEncoder.cpp \
				Base64.cpp \
				LogFeeder.cpp
OBJS		:=	$(SRCS:%.cpp=%.o)
LIBFTNAME	:=	libft.a
LIBFTDIR	:=	./libft
LIBFT		:=	$(LIBFTDIR)/$(LIBFTNAME)
NAME		:=	webserv
OUTDIR		:=	.

.PHONY:		all
all:		$(NAME)

$(NAME):	$(OBJS) $(LIBFT)
			$(CXX) $(CPPFLAGS) -L$(LIBFTDIR) $(patsubst lib%,-l%,$(basename $(LIBFTNAME))) \
			 -I$(LIBFTDIR) $(OBJS) -o $(NAME)

$(LIBFT):
			make -C $(LIBFTDIR)

.cpp.o:
			$(CXX) $(CPPFLAGS) -I$(LIBFTDIR) \
			-c $< -o $(patsubst %.cpp,%.o,$<)

.PHONY:		test
test:
			cd ./test && ./run_test.sh

.PHONY:		clean
clean:
			rm -f $(OBJS)
			make fclean -C $(LIBFTDIR)

.PHONY: leak
leak: CPPFLAGS += -g -fsanitize=leak
leak: re

.PHONY: address
address: CPPFLAGS += -g -fsanitize=address
address: re

.PHONY:		fclean
fclean:		clean
			rm -f $(NAME)

.PHONY:		re
re:			fclean all
