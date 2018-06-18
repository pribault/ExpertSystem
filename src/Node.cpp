#include "Node.hpp"
#include "Log.hpp"
#include <cstddef>
#include <iostream>

#define TABLE_SIZE	4

Node::Node(const std::string &token, Node *a, Node *b) : _token(token)
{
	_parent = NULL;
	_a = a;
	_b = b;
}

void	Node::deleteAll(void)
{
	if (_a)
		_a->deleteAll();
	if (_b)
		_b->deleteAll();
	delete this;
}

void			Node::setParent(Node *parent)
{
	_parent = parent;
}

std::string		&Node::getToken(void)
{
	return (_token);
}

Node			*Node::getParent(void) const
{
	return (_parent);
}

Node			*Node::getA(void) const
{
	return (_a);
}

Node			*Node::getB(void) const
{
	return (_b);
}

typedef struct	s_table
{
	std::string	op;
	t_res		a[TABLE_SIZE];
	t_res		b[TABLE_SIZE];
	t_res		out[TABLE_SIZE];
}				t_table;

static const t_table	tables[] = {
	{"+",
		{FALSE,	FALSE,	TRUE,	TRUE},
		{FALSE,	TRUE,	FALSE,	TRUE},
		{FALSE,	FALSE,	FALSE,	TRUE}
	},
	{"&",
		{FALSE,	FALSE,	TRUE,	TRUE},
		{FALSE,	TRUE,	FALSE,	TRUE},
		{FALSE,	FALSE,	FALSE,	TRUE}
	},
	{"and",
		{FALSE,	FALSE,	TRUE,	TRUE},
		{FALSE,	TRUE,	FALSE,	TRUE},
		{FALSE,	FALSE,	FALSE,	TRUE}
	},
	{"|",
		{FALSE,	FALSE,	TRUE,	TRUE},
		{FALSE,	TRUE,	FALSE,	TRUE},
		{FALSE,	TRUE,	TRUE,	TRUE}
	},
	{"or",
		{FALSE,	FALSE,	TRUE,	TRUE},
		{FALSE,	TRUE,	FALSE,	TRUE},
		{FALSE,	TRUE,	TRUE,	TRUE}
	},
	{"nand",
		{FALSE,	FALSE,	TRUE,	TRUE},
		{FALSE,	TRUE,	FALSE,	TRUE},
		{TRUE,	TRUE,	TRUE,	FALSE}
	},
	{"nor",
		{FALSE,	FALSE,	TRUE,	TRUE},
		{FALSE,	TRUE,	FALSE,	TRUE},
		{TRUE,	FALSE,	FALSE,	FALSE}
	},
	{"^",
		{FALSE,	FALSE,	TRUE,	TRUE},
		{FALSE,	TRUE,	FALSE,	TRUE},
		{FALSE,	TRUE,	TRUE,	FALSE}
	},
	{"xor",
		{FALSE,	FALSE,	TRUE,	TRUE},
		{FALSE,	TRUE,	FALSE,	TRUE},
		{FALSE,	TRUE,	TRUE,	FALSE}
	},
	{"xnor",
		{FALSE,	FALSE,	TRUE,	TRUE},
		{FALSE,	TRUE,	FALSE,	TRUE},
		{TRUE,	FALSE,	FALSE,	TRUE}
	},
	{"!",
		{FALSE,	FALSE,	TRUE,	TRUE},
		{FALSE,	TRUE,	FALSE,	TRUE},
		{TRUE,	TRUE,	FALSE,	FALSE}
	},
	{"not",
		{FALSE,	FALSE,	TRUE,	TRUE},
		{FALSE,	TRUE,	FALSE,	TRUE},
		{TRUE,	TRUE,	FALSE,	FALSE}
	}
};

t_res			Node::render(std::vector<t_state> &tokens) const
{
	t_res	res = UNDEF;
	t_res	a, b;

	for (size_t op = 0; op < sizeof(tables) / sizeof(t_table); op++)
		if (tables[op].op == _token)
		{
			if (_a)
				a = _a->render(tokens);
			else
				a = UNDEF;
			if (_b)
				b = _b->render(tokens);
			else
				b = UNDEF;
			for (size_t i = 0; i < TABLE_SIZE; i++)
				if ((a == tables[op].a[i] || a == UNDEF) &&
					(b == tables[op].b[i] || b == UNDEF))
				{
					if (res != UNDEF && res != tables[op].out[i])
						return (UNDEF);
					res = tables[op].out[i];
				}
			return (res);
		}
	for (size_t i = 0; i < tokens.size(); i++)
		if (_token == tokens[i].token)
			return tokens[i].value;
	throw (std::exception());
}

bool			Node::result(const std::string &token) const
{
	if (_token == token)
		return (true);
	if (_token == "=>")
		return ((_b) ? _b->result(token) : false);
	else
		return (((_a && _a->result(token) == true) || (_b && _b->result(token) == true)) ? true : false);
}

bool			Node::need(const std::string &token) const
{
	if (_token == token)
		return (true);
	if (_token == "=>")
		return ((_a) ? _a->result(token) : false);
	else
		return (((_a && _a->result(token) == true) || (_b && _b->result(token) == true)) ? true : false);
}

void			Node::setValues(std::vector<t_state> &tokens, t_res value)
{
	t_res	a = UNDEF;
	t_res	b = UNDEF;

	for (size_t op = 0; op < sizeof(tables) / sizeof(t_table); op++)
		if (tables[op].op == _token)
		{
			for (size_t i = 0; i < TABLE_SIZE; i++)
				if (value == tables[op].out[i] || value == UNDEF)
				{
					if (a != tables[op].a[i])
					{
						if (a == UNDEF)
							a = tables[op].a[i];
						else
						{
							if (_a)
								_a->setValues(tokens, UNDEF);
							if (_b)
								_b->setValues(tokens, UNDEF);
							return ;
						}
					}
					if (b != tables[op].b[i])
					{
						if (b == UNDEF)
							b = tables[op].b[i];
						else
						{
							if (_a)
								_a->setValues(tokens, UNDEF);
							if (_b)
								_b->setValues(tokens, UNDEF);
							return ;
						}
					}
				}
			if (_a)
				_a->setValues(tokens, a);
			if (_b)
				_b->setValues(tokens, b);
			return ;
		}
	for (size_t i = 0; i < tokens.size(); i++)
		if (_token == tokens[i].token)
		{
			tokens[i].value = value;
			return ;
		}
	throw (std::exception());
}

void			Node::debug(size_t level) const
{
	if (_a)
		_a->debug(level + 2);
	for (size_t i = 0; i < level; i++)
		log << " ";
	log << _token << std::endl;
	if (_b)
		_b->debug(level + 2);
}
