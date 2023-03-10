/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_access_iterator.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: genouf <genouf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 10:05:01 by genouf            #+#    #+#             */
/*   Updated: 2023/02/02 10:05:02 by genouf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RANDOM_ACCESS_ITERATOR_HPP
# define RANDOM_ACCESS_ITERATOR_HPP

#include <iostream>
#include "iterator_traits.hpp"

namespace ft
{
	template < class T >
	struct random_access_iterator 
	{
			typedef	random_access_iterator iterator;

			/*		MEMBER TYPES		*/
			typedef typename ft::iterator_traits<T*>::value_type		value_type;
			typedef typename ft::iterator_traits<T*>::difference_type	difference_type;
			typedef typename ft::iterator_traits<T*>::pointer			pointer;
			typedef typename ft::iterator_traits<T*>::reference			reference;
			typedef typename std::random_access_iterator_tag			iterator_category;

			/*	CONSTRUCTORS	*/
			// default constructor
			random_access_iterator() : _it(NULL) { return ; }

			// initialization constructor
			random_access_iterator(pointer src) : _it(src) { return ; }

			// copy constructor
			random_access_iterator(iterator const & copy) : _it(copy._it) { return ; }

			// copy-assignable constructor
			iterator & operator=(iterator const &rhs)
			{ 
				if (this != &rhs)
					this->_it = rhs._it;
				return (*this);
			}

			/*	DESTRUCTOR	*/
			virtual ~random_access_iterator() { return ; }

			/*	DEREFERENCE	*/
			reference operator*() const { return (*this->_it); }
			pointer operator->() const { return (this->_it); }

			/*	PREFIX INCREMENT	*/
			iterator & operator++()
			{
				++this->_it;

				return (*this);
			}

			iterator & operator--()
			{
				--this->_it;

				return (*this);
			}

			/*	POSTFIX INCREMENT	*/
			iterator operator++(int)
			{
				iterator	tmp(*this);

				++(*this);
				return (tmp);
			}

			iterator operator--(int) 
			{
				iterator	tmp(*this);

				--(*this);
				return (tmp);
			}

			/*	COMPARISON	*/
			bool operator==(iterator const &rhs) const { return (this->_it == rhs._it); }
			bool operator!=(iterator const &rhs) const { return (this->_it != rhs._it); }
			bool operator>(iterator const &rhs) const { return (this->_it > rhs._it); }
			bool operator<(iterator const &rhs) const { return (this->_it < rhs._it); }
			bool operator>=(iterator const &rhs) const { return (this->_it >= rhs._it); }
			bool operator<=(iterator const &rhs) const { return (this->_it <= rhs._it); }

			/*	ADDITION AND SUBTRACTION*/
			iterator & operator+=(int n)
			{
				this->_it += n;
				return (*this);
			}
			iterator & operator-=(int n)
			{
				this->_it -= n;
				return (*this);
			}
			iterator operator+(int n) const
			{
				iterator	tmp(*this);

				return (tmp += n);
			}
			iterator operator-(int n) const
			{
				iterator tmp(*this);

				return (tmp -= n);
			}

			friend iterator operator+(int n, iterator const &it) { return (it + n); }
			friend iterator operator-(int n, iterator const &it) { return (it - n); }

			operator random_access_iterator<const T> () { return (random_access_iterator<const T>(this->_it)); }

			/*	DEREFERENCE OPERATOR	*/
			reference operator[](int n) const { return (this->_it[n]); }

			/*	DISTANCE OPERATOR	*/
			friend difference_type	operator-(iterator const &lhs, iterator const &rhs) { return (lhs.base() - rhs.base()); }
			friend difference_type	operator+(iterator const &lhs, iterator const &rhs) { return (lhs.base() + rhs.base()); }

			pointer	base() const { return (this->_it); }

		private:
			/*	MEMBER VAR	*/
			pointer _it;
	};

	/*	OVERLOAD COMPARISON	*/
	template <class ItLeft, class ItRight>
	bool operator==(ItLeft const &lhs, ItRight const &rhs) { return (lhs.base() == rhs.base()); }

	template <class ItLeft, class ItRight>
	bool operator!=(ItLeft const &lhs, ItRight const &rhs) { return (lhs.base() != rhs.base()); }

	template <class ItLeft, class ItRight>
	bool operator>(ItLeft const &lhs, ItRight const &rhs) { return (lhs.base() > rhs.base()); }

	template <class ItLeft, class ItRight>
	bool operator<(ItLeft const &lhs, ItRight const &rhs) { return (lhs.base() < rhs.base()); }

	template <class ItLeft, class ItRight>
	bool operator>=(ItLeft const &lhs, ItRight const &rhs) { return (lhs.base() >= rhs.base()); }

	template <class ItLeft, class ItRight>
	bool operator<=(ItLeft const &lhs, ItRight const &rhs) { return (lhs.base() <= rhs.base()); }
}

#endif