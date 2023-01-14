#ifndef RB_TREE_HPP
# define RB_TREE_HPP

#include <iostream>
#include "utility.hpp"
#include "tree_iterator.hpp"
#include "reverse_iterator.hpp"

namespace ft
{
	template < class Value >
	struct Node
	{
		Node() : content(NULL), parent(NULL), left(NULL), right(NULL), is_red(true), is_null(true) { return ; }
		virtual ~Node() { return ; }

		Value 	*content;
		Node	*parent;
		Node	*left;
		Node	*right;
		bool	is_red;
		bool	is_null;
	};

	template < class T, class Compare = std::less<T>, class Allocator = std::allocator<T> >
	class	RBTree
	{
		public:

			/*	MEMBER TYPES	*/
			typedef typename ft::Node<T> 											node;
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
			typedef typename ft::tree_iterator<node, value_type>								iterator;
			typedef typename ft::tree_iterator<const node, const value_type>							const_iterator;
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
			virtual ~RBTree() { return ; }

			/*	FUNCTIONS	*/
			node	*create_node(node *parent)
			{
				node	*new_node;

				new_node = this->_al_node.allocate(1);
				this->_al_node.construct(new_node, node());
				new_node->parent = parent;
				return (new_node);
			}

			void	assign_node(node *node, value_type *content)
			{
				node->content = content;
				node->left = create_node(node);
				node->right = create_node(node);
				node->is_null = false;
				std::cout << "NEW NODE < " << node->content->first << " > => PARENT IS < " << node->parent->content->first << " >" << std::endl;  
				return ;
			}

			node	*search(value_type *entry)
			{
				node	*i = this->_root;
				
				while (i->is_null != true)
				{
					if (this->_compare(*entry, *i->content) == false && this->_compare(*i->content, *entry) == false)
						return (NULL);
					else if (this->_compare(*entry, *i->content) == true)
						i = i->left;
					else if (this->_compare(*entry, *i->content) == false)
						i = i->right;
				}
				return (i);
			}

			void	insert(value_type entry)
			{
				if (this->_root->is_null == true)
				{
					this->_root->content = &entry;
					this->_root->is_red = false;
					this->_root->is_null = false;
				} 
				else
				{
					node	*new_one = search(&entry);
					if (new_one == NULL)
					{
						std::cout << "IMPOSSIBLE D'INSERER CETTE CLE !" << std::endl;
						return ;
					}
					assign_node(new_one, &entry);
				}
			}

			node	*begin() { return (tree_min(this->_root)); }
			node	*end() { return (this->_begin); }
			// void	delete(key_type key)
			// {
			// 	node	*node_to_delete = search(key);
			// 	if (node == NULL)
			// 	{
			// 		std::cout << "IMPOSSIBLE DE SUPPRIMER CETTE CLE !" << std::endl;
			// 		return ;
			// 	}

			// }

		private:
			/*	MEMBER VAR	*/
			node			*_begin;
			node			*_root;
			allocator_type	_al;
			node_allocator	_al_node;
			key_compare		_compare;

			/*	FUNCTIONS	*/
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
	};
}

#endif