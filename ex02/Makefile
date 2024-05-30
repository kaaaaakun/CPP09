# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tokazaki <tokazaki@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/07 16:08:51 by tokazaki          #+#    #+#              #
#    Updated: 2024/05/05 09:35:37 by tokazaki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -- program name --
NAME = PmergeMe

# -- directories --
INCLUDE = -I ./
SRCDIR  = ./
OBJDIR  = obj

# -- files --
SRCS    = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)
OBJS    = $(addprefix $(OBJDIR)/, $(SRCDIR)/$(SRCS:.cpp=.o))
DEPENDS	= $(OBJS:.o=.d)

# -- compile flags --
CXX      = c++
CXXFLAGS = -g -Wall -Wextra -Werror -std=c++98 -MMD -MP

# -- debug flags --
NAME_DEBUG  = $(NAME)_debug
DEBUG_FLAGS = -fsanitize=address -fsanitize=bounds -DDEBUG
OBJS_DEBUG  = $(addprefix $(OBJDIR)/, $(SRCDIR)/$(SRCS:.cpp=.o.debug))

# -- command define	--
RM = rm -rf

# Mandatory target ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(NAME_DEBUG)

re: fclean all

# Debug targets +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
.PHONY: debug
debug: $(NAME_DEBUG)

$(NAME_DEBUG): $(OBJS_DEBUG)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(INCLUDE) -o $@ $(OBJS_DEBUG)

$(OBJDIR)/%.o.debug: $(SRCDIR)/%.cpp
	@ mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -o $@ -c $<

# Define ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
YELLOW = \x1b[38;5;228m
DEFAULT = \x1b[0m

# Other targets +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
.PHONY: format run test

format:
	clang-format --style=Google -i $(SRCS) $(HEADERS)

debug_test: $(NAME_DEBUG)
	@ echo "$(YELLOW) \n---  要素数が21個の時のdebug付きのテスト ---$(DEFAULT)"
	./$(NAME_DEBUG) 21 1 20 2 19 3 18 4 17 5 16 6 15 7 14 8 13 9 12 10 11
	@echo "\n $(YELLOW)比較回数 : "
	@./$(NAME_DEBUG) 21 1 20 2 19 3 18 4 17 5 16 6 15 7 14 8 13 9 12 10 11 | grep 比較|wc -l
	@echo "-------------------$(DEFAULT)"

run: $(NAME)
	@ echo "$(YELLOW) \n---  要素数が21個の時のテスト ---$(DEFAULT)"
	./$(NAME) 21 1 20 2 19 3 18 4 17 5 16 6 15 7 14 8 13 9 12 10 11

test: $(NAME)
	@ echo "$(YELLOW) \n---  subject nomal test$(DEFAULT)"
	./$(NAME) 3 5 9 7 4
	@ echo "$(YELLOW) \n---  subject huge  test$(DEFAULT)"
	./$(NAME) `shuf -i 1-100000 -n 3000 | tr "\n" " "`
	@ echo "$(YELLOW) \n---  subject error test$(DEFAULT)"
	./$(NAME) "-1" "2"; echo "$(YELLOW)--- $(DEFAULT)"
	
-include $(DEPENDS)
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
