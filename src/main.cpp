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
	static const std::string	operators = "<=>ifandonlyif=>then^xnorxornornand|or+&and!not";
	std::stack<Node *>			stack;
	std::string					token;
	Node						*op1;
	Node						*op2;

	for (size_t i = 0; i < rpn.size(); i++)
	{
		token = rpn[i];
		if (operators.rfind(token) != std::string::npos)
		{
			if (token == "!" && stack.size() >= 1)
			{
				op1 = stack.top();
				stack.pop();
				op2 = NULL;
			}
			else if (stack.size() >= 2)
			{
				op2 = stack.top();
				stack.pop();
				op1 = stack.top();
				stack.pop();
			}
			else
			{
				log << "not enough operands" << std::endl;
				throw (std::exception());
			}
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
	std::vector<t_state>		tokens;
	std::vector<std::string>	*rule;
	std::vector<Node *>			rules;
	std::ifstream				file;
	std::string					line;

	line.push_back('^');
	for (char c = 'A'; c < 'Z'; c++)
	{
		line[0] = c;
		tokens.push_back((t_state){line, FALSE});
	}
	for (int i = 1; i < argc; i++)
	{
		file.open(argv[i]);
		if (!file.is_open())
		{
			std::cerr << "cannot open " << argv[i] << std::endl;
			log << "cannot open " << argv[i] << std::endl;
		}
		else
		{
			for (size_t i = 0; i < tokens.size(); i++)
				tokens[i].value = FALSE;
			while (std::getline(file, line))
			{
				if (line.length())
				{
					switch (line[0])
					{
						case '=':
						{
							for (size_t i = 1; i < line.length(); i++)
								for (size_t j = 0; j < tokens.size(); j++)
									if (line[i] == tokens[j].token[0])
									{
										log << line[i] << " set to true" << std::endl;
										tokens[j].value = TRUE;
										break;
									}
							break;
						}
						case '?':
						{
							for (size_t i = 1; i < line.length(); i++)
								for (size_t j = 0; j < tokens.size(); j++)
									if (line[i] == tokens[j].token[0])
									{
										log << "searching for " << tokens[j].token << std::endl;
										if (tokens[j].value == TRUE)
										{
											log << tokens[j].token << " is true" << std::endl;
											std::cout << tokens[j].token << " is true" << std::endl;
										}
										else
										{
											try
											{
												evaluate(rules, tokens, tokens[j].token);
												switch (tokens[j].value)
												{
													case TRUE:
													{
														log << tokens[j].token << " is true" << std::endl;
														std::cout << tokens[j].token << " is true" << std::endl;
														break;
													}
													case FALSE:
													{
														log << tokens[j].token << " is false" << std::endl;
														std::cout << tokens[j].token << " is false" << std::endl;
														break;
													}
													case UNDEF:
													{
														log << tokens[j].token << " is undefined" << std::endl;
														std::cout << tokens[j].token << " is undefined" << std::endl;
														break;
													}
												}
											}
											catch (const std::exception &e)
											{
												log << "error, cannot solve " << tokens[j].token << std::endl;
												std::cout << "error, cannot solve " << tokens[j].token << std::endl;
											}
										}
										break;
									}
							break;
						}
						default:
						{
							try
							{
								rule = Shunting_Yard(std::string(line));
							}
							catch (const std::exception &e)
							{
								log << "failed to convert to rpn" << std::endl;
								rule = NULL;
							}
							if (rule)
							{
								try
								{
									rules.push_back(generate_rule(*rule));
									log << "rule " << rules.size() - 1 << " added" << std::endl;
								}
								catch (const std::exception &e)
								{
									log << "failed to generate tree" << std::endl;
								}
								delete rule;
							}
							break;
						}
					}
				}
			}
			for (size_t j = 0; j < rules.size(); j++)
			{
				log << "deleting rule " << j << std::endl;
				rules[j]->deleteAll();
			}
			rules.clear();
			file.close();
		}
	}
	return (0);
}
