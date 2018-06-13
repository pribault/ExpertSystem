#include "Node.hpp"
#include "Log.hpp"
#include <cstddef>

Node::Node(const std::string &token, Node *a, Node *b) : _token(token)
{
	_parent = NULL;
	_a = a;
	_b = b;
	_result = UNDEF;
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

Node::t_res		Node::getResult(void)
{
	return (_result);
}

void			Node::debug(size_t level)
{
	if (_a)
		_a->debug(level + 2);
	for (size_t i = 0; i < level; i++)
		log << " ";
	log << _token << std::endl;
	if (_b)
		_b->debug(level + 2);
}
