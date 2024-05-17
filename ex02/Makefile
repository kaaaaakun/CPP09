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
DEBUG_FLAGS = -fsanitize=address -fsanitize=bounds
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

# Other targets +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
.PHONY: format run test

format:
	clang-format --style=Google -i $(SRCS) $(HEADERS)

run: $(NAME)
	#./$(NAME) 21 20 17 16 19 18 15 14 13 12 11 10 9 5 3 4 1 6 7 2 8
	./$(NAME) 21 1 20 2 19 3 18 4 17 5 16 6 15 7 14 8 13 9 12 10 11

run2: $(NAME)
	./$(NAME) 1 21 20 19 17 10 16 15 14 13 12 11 18 9 8 7 6 5 4 3 2 

test: $(NAME)
	@ echo "---  subject nomal test "
	./$(NAME) 3 5 9 7 4
	@ echo "---  subject huge test"
	./$(NAME) `shuf -i 1-100000 -n 3000 | tr "\n" " "`
	@ echo "---  subject error test"
	./$(NAME) "-1" "2"
	
-include $(DEPENDS)
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
