#include "expert_system.hpp"

void		render_rule(Node *rule, std::vector<t_state> &tokens)
{
	if (rule->getToken() == "=>")
	{
		try
		{
			log << "a:" << rule->getA()->getToken() << " b:" << rule->getB()->getToken() << std::endl;
			for (size_t j = 0; j < tokens.size(); j++)
				if (tokens[j].token == "A")
					log << tokens[j].token << " -> " << tokens[j].value << std::endl;
			t_res	res = rule->getA()->render(tokens);
			for (size_t j = 0; j < tokens.size(); j++)
				if (tokens[j].token == "A")
					log << tokens[j].token << " -> " << tokens[j].value << std::endl;
			if (res == TRUE)
				rule->getB()->setValues(tokens, res);

			log << "rule result: ";
			switch (res)
			{
				case TRUE:
				{
					log << "true";
					break;
				}
				case FALSE:
				{
					log << "false";
					break;
				}
				case UNDEF:
				{
					log << "undefined";
					break;
				}
			}
			log << std::endl;
		}
		catch (const std::exception &e)
		{
			log << e.what() << std::endl;
		}
	}
	else if (rule->getToken() == "<=>")
	{
	}
	else
		throw (std::exception());
}

t_res		evaluate(std::vector<Node *> &rules, std::vector<t_state> &tokens, const std::string &token)
{
	std::vector<std::string>	previous;
	t_res						res = UNDEF;
	t_res						tmp;

	for (size_t j = 0; j < tokens.size(); j++)
		if (tokens[j].token == "A")
			log << tokens[j].token << " -> " << tokens[j].value << std::endl;
	for (size_t i = 0; i < previous.size(); i++)
		if (previous[i] == token)
			throw (std::exception());
	previous.push_back(token);
	for (size_t i = 0; i < rules.size(); i++)
		if (rules[i]->result(token))
		{
			for (size_t j = 0; j < tokens.size(); j++)
				if (rules[i]->need(tokens[j].token))
				{
					try
					{
						log << "needing " << tokens[j].token << std::endl;
						tokens[j].value = evaluate(rules, tokens, tokens[j].token);
						log << tokens[j].token << " is " << tokens[j].value << std::endl;
					}
					catch (const std::exception &e)
					{
						throw (e);
					}
				}
			render_rule(rules[i], tokens);
			for (size_t j = 0; j < tokens.size(); j++)
				if (tokens[j].token == token)
				{
					tmp = tokens[j].value;
					break;
				}
			if (tmp != res && res != UNDEF)
				throw (std::exception());
			res = tmp;
		}
	previous.pop_back();
	return (res);
}
