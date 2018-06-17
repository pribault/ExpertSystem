#include "expert_system.hpp"

void		render_rule(Node *rule, std::vector<t_state> &tokens)
{
	if (rule->getToken() == "=>")
	{
		try
		{
			t_res	res = rule->getA()->render(tokens);
			if (res == TRUE || res == UNDEF)
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
		try
		{
			t_res	res = rule->getA()->render(tokens);
			if (res == TRUE || res == UNDEF)
			{
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
			else
			{
				try
				{
					t_res	res = rule->getB()->render(tokens);
					if (res == TRUE || res == UNDEF)
						rule->getA()->setValues(tokens, res);

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
		}
		catch (const std::exception &e)
		{
			try
			{
				t_res	res = rule->getB()->render(tokens);
				if (res == TRUE)
					rule->getA()->setValues(tokens, res);

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
	}
	else
		throw (std::exception());
}

t_res		evaluate(std::vector<Node *> &rules, std::vector<t_state> &tokens, const std::string &token)
{
	static std::vector<Node *>	previous;
	t_res						res = UNDEF;
	t_res						tmp;
	bool						loop;

	for (size_t j = 0; j < tokens.size(); j++)
		if (tokens[j].token == token)
		{
			res = tokens[j].value;
			break;
		}
	for (size_t i = 0; i < rules.size(); i++)
	{
		loop = false;
		for (size_t j = 0; j < previous.size(); j++)
			if (rules[i] == previous[j])
				loop = true;
		if (loop == false)
		{
			previous.push_back(rules[i]);
			if (rules[i]->result(token))
			{
				for (size_t j = 0; j < tokens.size(); j++)
					if (tokens[j].token != token && rules[i]->need(tokens[j].token))
					{
						try
						{
							log << "[" << i << "] needing " << tokens[j].token << std::endl;
							tokens[j].value = evaluate(rules, tokens, tokens[j].token);
							log <<"[" << i << "] got " << tokens[j].value << std::endl;
							log << tokens[j].token << " is " << tokens[j].value << std::endl;
						}
						catch (const std::exception &e)
						{
							log << "[" << i << "] cannot get " << token << std::endl;
							// previous.pop_back();
							// throw (e);
						}
					}
				render_rule(rules[i], tokens);
				for (size_t j = 0; j < tokens.size(); j++)
					if (tokens[j].token == token)
					{
						tmp = tokens[j].value;
						break;
					}
				if (tmp != res && res == TRUE)
				{
					previous.pop_back();
					throw (std::exception());
				}
				res = tmp;
			}
			previous.pop_back();
		}
	}
	return (res);
}
