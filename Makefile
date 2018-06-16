# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pribault <pribault@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/06/08 11:16:00 by pribault          #+#    #+#              #
#    Updated: 2018/06/08 11:19:06 by pribault         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	exper_system
FLAGS =	-Wall -Wextra
CC = clang++

OBJ_DIR =	.obj
SRC_DIR =	src
INC_DIR =	include

SRC =	main.cpp Shunting_Yard.cpp Log.cpp Node.cpp algo.cpp
OBJ =	$(sort $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC)))
INC =	expert_system.hpp Log.hpp Node.hpp

INCLUDES =	$(sort $(patsubst %.hpp, $(INC_DIR)/%.hpp, $(INC)))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $@ $(FLAGS) $(OBJ)

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDES) Makefile | $(OBJ_DIR)
	$(CC) -o $@ $(FLAGS) -I $(INC_DIR) -c $<

clean:
	rm -rf $(OBJ_DIR) log

fclean: clean
	rm -f $(NAME)

re: fclean all
