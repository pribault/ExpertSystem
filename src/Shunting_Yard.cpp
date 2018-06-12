#include "expert_system.hpp"

/*
	while there are tokens to be read:
		read a token.
		if the token is a number, then:
			push it to the output queue.
		if the token is a function then:
			push it onto the operator stack 
		if the token is an operator, then:
			while ((there is a function at the top of the operator stack)
					or (there is an operator at the top of the operator stack with greater precedence)
					or (the operator at the top of the operator stack has equal precedence and is left associative))
					and (the operator at the top of the operator stack is not a left bracket):
				pop operators from the operator stack onto the output queue.
			push it onto the operator stack.
		if the token is a left bracket (i.e. "("), then:
			push it onto the operator stack.
		if the token is a right bracket (i.e. ")"), then:
			while the operator at the top of the operator stack is not a left bracket:
				pop the operator from the operator stack onto the output queue.
			pop the left bracket from the stack.
	if there are no more tokens to read:
		while there are still operator tokens on the stack:
			pop the operator from the operator stack onto the output queue.
	exit.
*/

std::string		*get_token(const std::string &s, size_t &i)
{
	static const std::string	families[] = {
		"<=>",
		" \a\b\t\n\v\f\r"
	};
	size_t			family;
	std::string		*token;

	if (i >= s.length())
		return (NULL);
	try
	{
		token = new std::string();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (NULL);
	}
	
	return (token);
}

std::stack<std::string>	Shunting_Yard(const std::string &s)
{
	std::stack<std::string>	rpn;
	std::string				*token;
	size_t					i = 0;

	while ((token = get_token(s, i)))
	{
		delete token;
	}
	return (rpn);
}
