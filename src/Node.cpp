#include "Node.hpp"
#include "Log.hpp"
#include <cstddef>
#include <iostream>

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
	t_res		a[9];
	t_res		b[9];
	t_res		out[9];
}				t_table;

t_res			Node::render(std::vector<t_state> &tokens) const
{
	static const t_table	tables[] = {
		{"+",
			{FALSE,	FALSE,	FALSE,	TRUE,	TRUE,	TRUE,	UNDEF,	UNDEF,	UNDEF},
			{FALSE,	TRUE,	UNDEF,	FALSE,	TRUE,	UNDEF,	FALSE,	TRUE,	UNDEF},
			{FALSE,	FALSE,	FALSE,	FALSE,	TRUE,	UNDEF,	FALSE,	UNDEF,	UNDEF}
		},
		{"&",
			{FALSE,	FALSE,	FALSE,	TRUE,	TRUE,	TRUE,	UNDEF,	UNDEF,	UNDEF},
			{FALSE,	TRUE,	UNDEF,	FALSE,	TRUE,	UNDEF,	FALSE,	TRUE,	UNDEF},
			{FALSE,	FALSE,	FALSE,	FALSE,	TRUE,	UNDEF,	FALSE,	UNDEF,	UNDEF}
		},
		{"|",
			{FALSE,	FALSE,	FALSE,	TRUE,	TRUE,	TRUE,	UNDEF,	UNDEF,	UNDEF},
			{FALSE,	TRUE,	UNDEF,	FALSE,	TRUE,	UNDEF,	FALSE,	TRUE,	UNDEF},
			{FALSE,	TRUE,	UNDEF,	TRUE,	TRUE,	TRUE,	UNDEF,	UNDEF,	UNDEF}
		},
		{"^",
			{FALSE,	FALSE,	FALSE,	TRUE,	TRUE,	TRUE,	UNDEF,	UNDEF,	UNDEF},
			{FALSE,	TRUE,	UNDEF,	FALSE,	TRUE,	UNDEF,	FALSE,	TRUE,	UNDEF},
			{FALSE,	TRUE,	UNDEF,	TRUE,	FALSE,	UNDEF,	UNDEF,	UNDEF,	UNDEF}
		},
		{"!",
			{FALSE,	FALSE,	FALSE,	TRUE,	TRUE,	TRUE,	UNDEF,	UNDEF,	UNDEF},
			{FALSE,	TRUE,	UNDEF,	FALSE,	TRUE,	UNDEF,	FALSE,	TRUE,	UNDEF},
			{TRUE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	UNDEF,	UNDEF,	UNDEF}
		}
	};
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
			for (size_t i = 0; i < 9; i++)
				if (a == tables[op].a[i] &&
					b == tables[op].b[i])
					return (tables[op].out[i]);
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
	static const t_table	tables[] = {
		{"+",
			{UNDEF,	TRUE,	UNDEF},
			{UNDEF,	TRUE,	UNDEF},
			{FALSE,	TRUE,	UNDEF}
		},
		{"&",
			{UNDEF,	TRUE,	UNDEF},
			{UNDEF,	TRUE,	UNDEF},
			{FALSE,	TRUE,	UNDEF}
		},
		{"|",
			{FALSE,	UNDEF,	UNDEF},
			{FALSE,	UNDEF,	UNDEF},
			{FALSE,	TRUE,	UNDEF}
		},
		{"^",
			{UNDEF,	UNDEF,	UNDEF},
			{UNDEF,	UNDEF,	UNDEF},
			{FALSE,	TRUE,	UNDEF}
		},
		{"!",
			{TRUE,	FALSE,	UNDEF},
			{TRUE,	FALSE,	UNDEF},
			{FALSE,	TRUE,	UNDEF}
		}
	};

	for (size_t op = 0; op < sizeof(tables) / sizeof(t_table); op++)
		if (tables[op].op == _token)
			for (size_t i = 0; i < 3; i++)
				if (value == tables[op].out[i])
				{
					if (_a)
						_a->setValues(tokens, tables[op].a[i]);
					if (_b)
						_b->setValues(tokens, tables[op].b[i]);
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
