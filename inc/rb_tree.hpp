#ifndef RB_TREE_HPP
# define RB_TREE_HPP

#include <iostream>
#include "utility.hpp"
#include "tree_iterator.hpp"
#include "reverse_iterator.hpp"

namespace ft
{
	template < class Value, class Allocator = std::allocator<Value> >
	struct Node
	{
		typedef	Allocator														allocator_type;

		/*	CONSTRUCTORS	*/
		Node(const allocator_type& alloc = allocator_type()) : parent(NULL), left(NULL), right(NULL), is_red(false), is_null(true), _al(alloc)
		{
			this->content = this->_al.allocate(1);
			this->_al.construct(this->content, Value());
			return ; 
		}

		Node(const Node &src) : parent(src.parent), left(src.left), right(src.right), is_red(src.is_red), is_null(src.is_null), _al(src._al)
		{
			this->content = this->_al.allocate(1);
			this->_al.construct(this->content, *src.content);
			return ;
		}

		/*	DESTRUCTOR	*/
		virtual ~Node()
		{
			this->_al.destroy(this->content);
			this->_al.deallocate(this->content, 1);	
			return ; 
		}

		/*	FUNCTIONS	*/
		void	insert(Value entry)
		{
			this->_al.destroy(this->content);
			this->_al.construct(this->content, entry);
			return ;
		}

		/*	PUBLIC VAR	*/
		Value 			*content;
		Node			*parent;
		Node			*left;
		Node			*right;
		bool			is_red;
		bool			is_null;

		private:
			/*	PRIVATE VAR	*/
			allocator_type 	_al;

	};

	template < class T, class Compare = std::less<T>, class Allocator = std::allocator<T> >
	class	RBTree
	{
		public:

			/*	MEMBER TYPES	*/
			typedef typename ft::Node<T, Allocator> 								node;
			typedef	T																value_type;
			typedef Compare															key_compare;
			typedef Allocator														allocator_type;
			typedef typename Allocator::template rebind< node >::other				node_allocator;
			typedef typename node_allocator::pointer								node_pointer;
			typedef typename allocator_type::reference								reference;
			typedef typename allocator_type::const_reference						const_reference;
			typedef typename allocator_type::pointer								pointer;
			typedef typename allocator_type::const_pointer							const_pointer;
			typedef std::ptrdiff_t													difference_Type;
			typedef std::size_t														size_type;
			typedef typename ft::tree_iterator<node, value_type>					iterator;
			typedef typename ft::tree_iterator<const node, const value_type>		const_iterator;
			typedef typename ft::reverse_iterator<iterator>							reverse_iterator;
			typedef typename ft::reverse_iterator<const_iterator>					const_reverse_iterator;

			/*	CONSTRUCTORS	*/
			RBTree(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : _al(alloc), _al_node(node_allocator()), _compare(comp)
			{ 
				this->_begin = this->_al_node.allocate(1);
				this->_al_node.construct(this->_begin, node());
				this->_root = this->_al_node.allocate(1);
				this->_al_node.construct(this->_root, node());
				this->_begin->left = this->_root;
				this->_root->parent = this->_begin;
				this->_root->left = create_node(this->_root);
				this->_root->right = create_node(this->_root);
				return ;
			}

			/*	DESTRUCTOR	*/
			virtual ~RBTree()
			{
				clean_tree(this->_begin->left);
				this->_al_node.destroy(this->_begin);
				this->_al_node.deallocate(this->_begin, 1);
				return ; 
			}

			/*	PUBLIC FUNCTIONS	*/
			void	insert(value_type entry)
			{
				if (this->_begin->left->is_null == true)
				{
					this->_begin->left->insert(entry);
					this->_begin->left->is_red = false;
					this->_begin->left->is_null = false;
				} 
				else
				{
					node	*new_one = search(entry);
					if (new_one == NULL)
						return ;
					assign_node(new_one, entry);
					check_insert(new_one);
				}
			}

			void	delete_sheet(node *n)
			{
				clean(n->left);
				clean(n->right);
				n->left = NULL;
				n->right = NULL;
				this->_al_node.destroy(n->content);
				n->is_red = false;
				n->is_null = true;
			}

			void	transfer_node(node *n1, node *n2, bool is_left)
			{
				if (child_of_who(n1) == 0)
					n1->parent->left = n2;
				else if (child_of_who(n2) == 1)
					n1->parent->right = n2;
				n2->parent = n1->parent;
				is_left ? clean(n1->right) : clean(n1->left);
				clean(n1);
				return ;
			}

			void	move_node(node *n, node *to_delete)
			{
				clean(n->left);
				n->right->parent = n->parent;
				if (child_of_who(n) == 0)
					n->parent->left = n->right;
				else if (child_of_who(n) == 1)
					n->parent->right = n->right;
				n->parent = to_delete->parent;
				n->left = to_delete->left;
				n->right = to_delete->right;
				n->is_red = to_delete->is_red;
				n->is_null = to_delete->is_null;
				return ;
			}

			void	recolor(node *n)
			{
				if (n->is_red == true)
					n->is_red = false;
				else if (n == this->_begin->left)
					return ;
				else
				{
					if (child_of_who(n) == 0)
					{
						if (n->parent->right->is_red == true)
						{
							n->parent->right->is_red = false;
							n->parent->is_red = true;
							rotate_left(n->parent);
						}
					}
				}
				return ;
			}

			void	pop(value_type entry)
			{
				node	*to_delete = search(entry);

				if (to_delete)
				{
					node	*was_black;

					if (is_a_sheet(to_delete))
					{
						to_delete->is_red == false ? was_black = to_delete : was_black = NULL;
						delete_sheet(to_delete);
					}
					else if (has_one_child(to_delete))
					{
						if (to_delete->left->is_null == false)
						{
							to_delete->is_red == false ? was_black = to_delete->left : was_black = NULL;
							transfer_node(to_delete, to_delete->left, true);
						}
						else
						{
							to_delete->is_red == false ? was_black = to_delete->right : was_black = NULL;
							transfer_node(to_delete, to_delete->right, false);
						}
					}
					else
					{
						node *close_value = tree_min(to_delete->right);

						close_value->is_red == false ? was_black = close_value->right : was_black = NULL;
						move_node(close_value, to_delete);
					}
					if (was_black)
						recolor(was_black);
				}
				return ;
			}

			node	*begin() { return (tree_min(this->_begin->left)); }
			node	*end() { return (this->_begin); }

		private:
			/*	PRIVATE VAR	*/
			node			*_begin;
			node			*_root;
			allocator_type	_al;
			node_allocator	_al_node;
			key_compare		_compare;

			/*	PRIVATE FUNCTIONS	*/
			/*	For iterators	*/
			node 	*tree_min(node  *node) const
			{
				while (node->left != NULL && node->left->is_null == false)
					node = node->left;
				return (node);
			}

			node 	*tree_max(node  *node) const
			{
				while (node->right != NULL && node->right->is_null == false)
					node = node->right;
				return (node);
			}

			/*	Initialization	*/
			node	*create_node(node *parent)
			{
				node	*new_node;

				new_node = this->_al_node.allocate(1);
				this->_al_node.construct(new_node, node());
				new_node->parent = parent;
				return (new_node);
			}

			void	assign_node(node *node, value_type content)
			{
				node->insert(content);
				node->left = create_node(node);
				node->right = create_node(node);
				node->is_null = false;
				node->is_red = true;
				return ;
			}

			node	*search(value_type entry)
			{
				node	*i = this->_begin->left;
				
				while (i->is_null != true)
				{
					if (this->_compare(entry, *i->content) == false && this->_compare(*i->content, entry) == false)
						return (NULL);
					else if (this->_compare(entry, *i->content) == true)
						i = i->left;
					else if (this->_compare(entry, *i->content) == false)
						i = i->right;
				}
				return (i);
			}

			/*	Clean allocations	*/
			void	clean(node *n)
			{
				this->_al_node.destroy(n);
				this->_al_node.deallocate(n, 1);
				return ;
			}

			void	clean_tree(node *n)
			{
				if (n->left)
					clean_tree(n->left);
				if (n->right)
					clean_tree(n->right);
				clean(n);
				return ;
			}

			/*	Insertions	*/
			node	*find_uncle(node *n)
			{
				node	*gp = n->parent->parent;

				if (n->parent == gp->left)
					return (gp->right);
				else if (n->parent == gp->right)
					return (gp->left);
				return (NULL);
			}

			void	check_insert(node *new_one)
			{
				while (new_one->parent->is_red == true)
				{
					node	*uncle = find_uncle(new_one);
					node	*gp = new_one->parent->parent;

					if (uncle->is_red == true)
					{
						uncle->is_red = false;
						new_one->parent->is_red = false;
						gp->is_red = true;
						new_one = gp;
					}
					else
					{
						// Le parent est un enfant gauche
						if (child_of_who(new_one->parent) == 0) 
						{

							if (child_of_who(new_one) == 1)
							{
								rotate_left(new_one->parent);
								new_one = new_one->left;
							}
							else if (child_of_who(new_one) == 0)
							{
								new_one->parent->is_red = false;
								gp->is_red = true;
								rotate_right(gp);
							}
						}
						// Cas symetrique
						else if (child_of_who(new_one->parent) == 1)
						{
							if (child_of_who(new_one) == 0)
							{
								rotate_right(new_one->parent);
								new_one = new_one->right;
							}
							else if (child_of_who(new_one) == 1)
							{
								new_one->parent->is_red = false;
								gp->is_red = true;
								rotate_left(gp);
							}
						}
					}
					this->_begin->left->is_red = false;
				}
				return ;
			}

			/*	Rotations	*/
			void	rotate_left(node *n)
			{
				node *parent = n->parent;
				node *right = n->right;

				if (n == parent->left)
					parent->left = right;
				else if (n == parent->right)
					parent->right = right;
				right->parent = parent;
				node *tmp = right->left;
				right->left = n;
				n->parent = right;
				n->right = tmp;
				tmp->parent = n;
				return ;
			}

			void	rotate_right(node *n)
			{
				node *parent = n->parent;
				node *left = n->left;
				
				if (n == parent->left)
					parent->left = left;
				else if (n == parent->right)
					parent->right = left;
				left->parent = parent;
				node *tmp = left->right;
				left->right = n;
				n->parent = left;
				n->left = tmp;
				tmp->parent = n;
				return ;
			}

			/*	Infos	*/
			int	child_of_who(node *n)
			{
				if (n == n->parent->left)
					return (0);
				if (n == n->parent->right)
					return (1);
				return (-1);
			}

			bool	is_a_sheet(node *n)
			{
				if (n->left->is_null == true && n->right->is_null == true)
					return (true);
				return (false);
			}

			bool	has_one_child(node *n)
			{
				if (n->left->is_null == true && n->right->is_null == false)
					return (true)
				if (n->left->is_null == false && n->right->is_null == true)
					return (true);
				return (false);
			}
	};
}

#endif
