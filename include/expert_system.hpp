#ifndef EXPERT_SYSTEM_HPP
# define EXPERT_SYSTEM_HPP

# include "Log.hpp"
# include "Node.hpp"
# include <stack>
# include <iostream>

std::vector<std::string>	*Shunting_Yard(const std::string &s);
t_res						evaluate(std::vector<Node *> &rules, std::vector<t_state> &tokens, const std::string &token);

#endif
