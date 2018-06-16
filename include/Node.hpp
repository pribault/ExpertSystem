#ifndef NODE_HPP
# define NODE_HPP

# include <string>
# include <vector>

typedef enum	e_res
{
	FALSE = 0,
	TRUE = 1,
	UNDEF = 2
}				t_res;

typedef struct	s_state
{
	std::string	token;
	t_res		value;
}				t_state;

class	Node
{
	public:
		Node(const std::string &token, Node *a, Node *b);

		void			deleteAll(void);

		void			setParent(Node *parent);

		std::string		&getToken(void);
		Node			*getParent(void) const;
		Node			*getA(void) const;
		Node			*getB(void) const;
		t_res			render(std::vector<t_state> &tokens) const;

		bool			result(const std::string &token) const;
		bool			need(const std::string &token) const;
		void			setValues(std::vector<t_state> &tokens, t_res value);

		void			debug(size_t level) const;
	private:
		std::string	_token;
		Node		*_parent;
		Node		*_a;
		Node		*_b;
};

#endif
