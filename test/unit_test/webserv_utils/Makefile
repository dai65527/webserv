# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dhasegaw <dhasegaw@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/09 10:22:35 by dnakano           #+#    #+#              #
#    Updated: 2021/04/15 12:05:16 by dhasegaw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX			:=	clang++
CPPFLAGS	:=	-Wall -Wextra -Werror -D UNIT_TEST

### config ###
SRCNAME		:=	webserv_utils.cpp
SRCDIR		:=	../../..
INCLUDEDIR	:=	../../..
TESTSRCNAME	:=	test_main.cc \
				test_basename.cc \
				test_getIpAddress.cc 
TESTSRCDIR	:=	.
### config ###

# no need to edit
SRCS		:=	$(addprefix $(SRCDIR)/,$(SRCNAME))
TESTSRCS	:=	$(addprefix $(TESTSRCDIR)/,$(TESTSRCNAME))
GTESTSRCNAME:=	gtest-all.cc
GTESTSRCDIR	:=	../gtest
GTESTSRCS	:=	$(addprefix $(GTESTSRCDIR)/,$(GTESTSRCNAME))
OBJS		:=	$(SRCS:%.cpp=%.o) $(TESTSRCS:%.cc=%.o) $(GTESTSRCS:%.cc=%.o)
LIBFT		:=	libft.a
LIBFTDIR	:=	../../../libft
NAME		:=	test.out
OUTDIR		:=	.

.SUFFIXES:	.o .cpp .cc

.PHONY:		all
all:		test

$(NAME):	$(LIBFT) $(OBJS)
			$(CXX) $(CPPFLAGS) $(OBJS) \
			-L$(LIBFTDIR) $(patsubst lib%,-l%,$(basename $(LIBFT))) \
		 	-o $(NAME)

.PHONY:		test
test:		$(NAME)
			$(OUTDIR)/$(NAME)

.cpp.o:
			$(CXX) $(CPPFLAGS) -I$(INCLUDEDIR) -I$(LIBFTDIR) -I$(GTESTSRCDIR) \
			-c $< -o $(patsubst %.cpp,%.o,$<)

.cc.o:
			$(CXX) -std=c++11 -I$(INCLUDEDIR) -I$(GTESTSRCDIR) -I$(LIBFTDIR) \
			-c $< -o $(patsubst %.cc,%.o,$<)

$(LIBFT):
			make -C $(LIBFTDIR) $(LIBFT)

.PHONY:		clean
clean:
			make -C $(LIBFTDIR) fclean
			rm -f $(OBJS)

.PHONY:		fclean
fclean:		clean
			rm -f $(NAME)

.PHONY:		re
re:			fclean all

# for debug of Makefile
.PHONY:		debug
debug:
			$(warning OBJS = $(OBJS))
