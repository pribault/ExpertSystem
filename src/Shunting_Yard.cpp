#include "expert_system.hpp"

std::string		*get_token(const std::string &s, size_t &i)
{
	static const std::string	families[] = {
		"0123456789.",
		"<=>",
		" \a\b\t\n\v\f\r",
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
		log << e.what() << std::endl;
		return (NULL);
	}

	for (family = 0; family < sizeof(families) / sizeof(std::string); family++)
		if (families[family].find(s[i]) != std::string::npos)
			break ;
	token->push_back(s[i++]);
	if (family < sizeof(families) / sizeof(std::string))
		while (i < s.length() &&
			families[family].find(s[i]) != std::string::npos)
			token->push_back(s[i++]);
	return (token);
}

std::vector<std::string>	*Shunting_Yard(const std::string &s)
{
	static const std::string	letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static const std::string	operators = "|^&";
	static const std::string	implies = "=>";
	std::vector<std::string>	*output;
	std::stack<std::string>		stack;
	std::string					*token;
	size_t						i = 0;

	try
	{
		output = new std::vector<std::string>();
	}
	catch (const std::exception &e)
	{
		throw (e);
	}

	while ((token = get_token(s, i)))
	{
		log << "token: " << *token << std::endl;
		log << "stack:" << std::endl;
		for (size_t i = 0; i < stack.size(); i++)
			log << "\t" << *(&stack.top() - i) << std::endl;
		if (letters.rfind(*token) != std::string::npos)
		{
			log << *token << ": letter" << std::endl;
			output->push_back(*token);
		}
		else if (operators.rfind(*token) != std::string::npos)
		{
			log << *token << ": operator" << std::endl;
			while ((stack.size() && operators.rfind(stack.top()) != std::string::npos &&
				operators.rfind(stack.top()) >= operators.rfind(*token)) && stack.top() != "(")
			{
				output->push_back(stack.top());
				stack.pop();
			}
			stack.push(*token);
		}
		else if (*token == implies)
		{
			log << *token << ": implies" << std::endl;
			while (stack.size() && stack.top() != "(")
			{
				output->push_back(stack.top());
				stack.pop();
			}	
			stack.push(*token);
		}
		else if (*token == "(")
		{
			log << *token << ": bracket" << std::endl;
			stack.push(*token);
		}
		else if (*token == ")")
		{
			log << *token << ": bracket" << std::endl;
			while (stack.size() && stack.top() != "(")
			{
				output->push_back(stack.top());
				stack.pop();
			}
			if (stack.size())
				stack.pop();
			else
			{
				log << "parentheses mismatch" << std::endl;
				throw (std::exception());
			}
		}
		else
		{
			log << "invalid token '" << *token << "'" << std::endl;
			throw (std::exception());
		}
		delete token;
	}
	while (stack.size())
	{
		if (stack.top() == "(" || stack.top() == ")")
		{
			log << "parentheses mismatch" << std::endl;
			throw (std::exception());
		}
		else
		{
			output->push_back(stack.top());
			stack.pop();
		}
	}
	return (output);
}
