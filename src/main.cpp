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
			try
			{
				stack.push(new Node(token, op1, op2));
			}
			catch (const std::exception &e)
			{
				throw (e);
			}
		}
		else if (letters.rfind(token) != std::string::npos)
		{
			try
			{
				stack.push(new Node(token, NULL, NULL));
			}
			catch (const std::exception &e)
			{
				throw (e);
			}
		}
		else
		{
			log << "invalid token '" << token << "'" << std::endl;
			throw (std::exception());
		}
	}
	if (stack.size() != 1)
	{
		log << "too much operands" << std::endl;
		throw (std::exception());
	}
	log << "\"";
	for (size_t i = 0; i < rpn.size(); i++)
		if (i + 1 != rpn.size())
			log << rpn.at(i) << " ";
		else
			log << rpn.at(i);
	log << "\" converted to tree:" << std::endl;
	stack.top()->debug(2);
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
		}
		catch (const std::exception &e)
		{
			log << "failed to convert to rpn" << std::endl;
			rpn = NULL;
		}
		if (rpn)
		{
			try
			{
				tree = generate_rule(*rpn);
			}
			catch (const std::exception &e)
			{
				log << "failed to generate tree" << std::endl;
				tree = NULL;
			}
			if (tree)
			{
				tree->deleteAll();
			}
			delete rpn;
		}
	}
	return (0);
}
