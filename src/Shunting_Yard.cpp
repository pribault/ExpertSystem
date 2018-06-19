#include "expert_system.hpp"

std::string		*get_token(const std::string &s, size_t &i)
{
	static const std::string	operators[] = {
		"<=>",
		"ifandonlyif",
		"=>",
		"then",
		"xnor",
		"^",
		"xor",
		"nor",
		"nand",
		"|",
		"or",
		"+",
		"&",
		"and",
		"!",
		"not",
	};

	for (size_t j = 0; j < sizeof(operators) / sizeof(std::string); j++)
		if (!s.compare(i, operators[j].size(), operators[j]))
		{
			i += operators[j].size();
			return (new std::string(operators[j]));
		}

	std::string		*token;

	if (i >= s.length() || s[i] == '#')
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
	token->push_back(s[i++]);
	return (token);
}

std::vector<std::string>	*Shunting_Yard(const std::string &s)
{
	static const std::string	whitespaces = " \a\b\t\n\v\f\r";
	static const std::string	letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static const std::string	operators = "<=>ifandonlyif=>then^xnorxornornand|or+&and!not";
	std::vector<std::string>	*output;
	std::stack<std::string>		stack;
	std::string					*token;
	size_t						i = 0;
	size_t						count = 0;

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
		if (letters.rfind(*token) != std::string::npos)
		{
			output->push_back(*token);
		}
		else if (operators.rfind(*token) != std::string::npos)
		{
			if ((*token == "=>" || *token == "then" ||
				*token == "<=>" || *token == "ifandonlyif") &&
				count++)
			{
				log << "duplicated token '" << *token << "'" << std::endl;
				delete token;
				delete output;
				throw (std::exception());
			}
			while ((stack.size() && operators.rfind(stack.top()) != std::string::npos &&
				operators.rfind(stack.top()) >= operators.rfind(*token)) && stack.top() != "(")
			{
				output->push_back(stack.top());
				stack.pop();
			}
			stack.push(*token);
		}
		else if (*token == "(")
			stack.push(*token);
		else if (*token == ")")
		{
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
				delete token;
				delete output;
				throw (std::exception());
			}
		}
		else if (whitespaces.rfind(*token) != std::string::npos)
			;
		else
		{
			log << "invalid token '" << *token << "'" << std::endl;
			delete token;
			delete output;
			throw (std::exception());
		}
		delete token;
	}
	if (!count)
	{
		log << "no implie" << std::endl;
		delete output;
		throw (std::exception());
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
	log << "\"" << s << "\" converted to \"";
	for (size_t i = 0; i < output->size(); i++)
		if (i + 1 != output->size())
			log << output->at(i) << " ";
		else
			log << output->at(i);
	log << "\"" << std::endl;
	return (output);
}
