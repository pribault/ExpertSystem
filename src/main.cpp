/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/08 11:17:27 by pribault          #+#    #+#             */
/*   Updated: 2018/06/08 11:18:50 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expert_system.hpp"

Node	*generate_rule(std::vector<std::string> &rpn)
{
	static const std::string	letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static const std::string	operators = "|^&";
	static const std::string	implies = "=>";
	std::stack<Node *>			stack;
	std::string					token;
	Node						*op1;
	Node						*op2;

	for (size_t i = 0; i < rpn.size(); i++)
	{
		token = rpn[i];
		if (operators.rfind(token) != std::string::npos ||
			token == implies)
		{
			if (stack.size() < 2)
			{
				log << "not enough operands" << std::endl;
				throw (std::exception());
			}
			op2 = stack.top();
			stack.pop();
			op1 = stack.top();
			stack.pop();
			stack.push(new Node(token, op1, op2));
		}
		else if (letters.rfind(token) != std::string::npos)
			stack.push(new Node(token, NULL, NULL));
		else
		{
			log << "invalid token '" << token << "'" << std::endl;
			throw (std::exception());
		}
	}
	if (stack.size() != 1)
	{
		log << "error generating rule x_x" << std::endl;
		throw (std::exception());
	}
	return (stack.top());
}

int		main(int argc, char **argv)
{
	std::vector<std::string>	*rpn;
	Node						*tree;

	for (int i = 1; i < argc; i++)
	{
		try
		{
			rpn = Shunting_Yard(std::string(argv[i]));
			std::cout << "rpn:" << std::endl;
			for (size_t i = 0; i < rpn->size(); i++)
				std::cout << rpn->at(i) << " ";
			std::cout << std::endl;
			tree = generate_rule(*rpn);
			tree->debug(0);
			delete rpn;
		}
		catch (const std::exception &e)
		{
			log << "failed to convert to rpn" << std::endl;
		}
	}
	return (0);
}
