# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/05 17:34:06 by dnakano           #+#    #+#              #
#    Updated: 2021/03/27 21:37:13 by dhasegaw         ###   ########.fr        #
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
				ServerOnlyConfigStore.cpp
OBJS		:=	$(SRCS:%.cpp=%.o)
LIBFT		:=	libft.a
LIBFTDIR	:=	./libft
NAME		:=	nginDX
OUTDIR		:=	.

.PHONY:		all
all:		$(NAME)

$(NAME):	$(LIBFT) $(OBJS)
			$(CXX) -L$(LIBFTDIR) $(patsubst lib%,-l%,$(basename $(LIBFT))) \
			$(SRCS) -o $(NAME) -I$(LIBFTDIR)

$(LIBFT):
			make -C $(LIBFTDIR)

.cpp.o:
			$(CXX) $(CPPFLAGS) -I$(LIBFTDIR) \
			-c $< -o $(patsubst %.cpp,%.o,$<)

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
