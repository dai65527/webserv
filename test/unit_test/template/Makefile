# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dnakano <dnakano@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/09 10:22:35 by dnakano           #+#    #+#              #
#    Updated: 2021/03/09 18:13:27 by dnakano          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX			:=	clang++
CPPFLAGS	:=	-Wall -Wextra -Werror

### config ###
SRCNAME		:=	target.cpp
SRCDIR		:=	.
INCLUDEDIR	:=	.
TESTSRCNAME	:=	test.cc			# must be *.cc NOT *.cp
TESTSRCDIR	:=	.
### config ###

# no need to edit
SRCS		:=	$(addprefix $(SRCDIR)/,$(SRCNAME))
TESTSRCS	:=	$(addprefix $(TESTSRCDIR)/,$(TESTSRCNAME))
GTESTSRCNAME:=	gtest-all.cc
GTESTSRCDIR	:=	../gtest
GTESTSRCS	:=	$(addprefix $(GTESTSRCDIR)/,$(GTESTSRCNAME))
OBJS		:=	$(SRCS:%.cpp=%.o) $(TESTSRCS:%.cc=%.o) $(GTESTSRCS:%.cc=%.o)
NAME		:=	test.out
OUTDIR		:=	.

.SUFFIXES:	.o .cpp .cc

.PHONY:		all
all:		test

$(NAME):	$(OBJS)
			$(CXX) $(CPPFLAGS) $(OBJS) -o $(NAME)

.PHONY:		test
test:		$(NAME)
			$(OUTDIR)/$(NAME)

.cpp.o:
			$(CXX) $(CPPFLAGS) -I$(INCLUDEDIR) -I$(GTESTSRCDIR) -c $< -o $(patsubst %.cpp,%.o,$<)

.cc.o:
			$(CXX) -std=c++11 -I$(GTESTSRCDIR) -c $< -o $(patsubst %.cc,%.o,$<)

.PHONY:		clean
clean:
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
