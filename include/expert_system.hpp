#ifndef EXPERT_SYSTEM_HPP
# define EXPERT_SYSTEM_HPP

# include "Log.hpp"
# include "Node.hpp"
# include <stack>
# include <vector>
# include <iostream>

std::vector<std::string>	*Shunting_Yard(const std::string &s);

typedef struct	s_state
{
	std::string	token;
	Node::t_res	value;
}				t_state;

#endif
