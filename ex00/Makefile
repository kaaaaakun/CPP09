# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tokazaki <tokazaki@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/07 16:08:51 by tokazaki          #+#    #+#              #
#    Updated: 2024/06/07 15:11:20 by tokazaki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -- program name --
NAME = btc

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
.PHONY: format run

format:
	clang-format --style=Google -i $(SRCS) $(HEADERS)

run: $(NAME)
	./$(NAME) input.txt
	@echo "$(YELLOW) \n--- input.txtの内容 ---$(DEFAULT)"
	@cat input.txt
	@echo "$(YELLOW) \n--- 対応するdata.csvの内容---$(DEFAULT)"
	@for date in $$(seq -f "2011-01-%02g" 1 10) $$(seq -f "2012-01-%02g" 10 13); do \
		grep $$date data.csv; \
	done

-include $(DEPENDS)
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#
