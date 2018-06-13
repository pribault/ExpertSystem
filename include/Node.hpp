#ifndef NODE_HPP
# define NODE_HPP

# include <string>

class	Node
{
	public:
		Node(const std::string &token, Node *a, Node *b);

		void	deleteAll(void);

		void			setParent(Node *parent);

		typedef enum	e_res
		{
			TRUE = true,
			FALSE = false,
			UNDEF
		}				t_res;

		t_res			getResult(void);

		void			debug(size_t level);
	private:
		std::string	_token;
		Node		*_parent;
		Node		*_a;
		Node		*_b;
		t_res		_result;
};

#endif
