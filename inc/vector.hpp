#ifndef VECTOR_HPP
# define VECTOR_HPP

#include "random_access_iterator.hpp"

namespace ft
{
	template <class T, class Allocator = std::allocator<T> >
	class vector
	{
		public: 

		/*		MEMBER TYPES		*/
		typedef T 												value_type;
		typedef Allocator 										allocator_type;
		typedef typename allocator_type::reference 				reference;
		typedef typename allocator_type::const_reference 		const_reference;
		typedef typename allocator_type::pointer 				pointer;
		typedef typename allocator_type::const_pointer 			const_pointer;
		typedef typename ft::random_access_iterator<T> 			iterator;
		typedef typename ft::random_access_iterator<const T> 	const_iterator;
		typedef std::ptrdiff_t 									difference_type;
		typedef std::size_t 									size_type;

		/*		CONSTRUCTORS		*/
		/*	Empty container constructor	*/
		explicit vector (const allocator_type& alloc = allocator_type()) : _size(0), _capacity(0), _container(0), _al(alloc) { return ; };
		
		/*	Fill constructor	*/
		explicit vector(size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()) : _size(n), _capacity(n), _al(alloc)
		{
			this->_container = this->_al.allocate(_capacity);
			for (size_type i = 0; i < this->_size; i++)
				this->_al.construct(this->_container + i, val);
			return ;
		}

		/*	Range constructor	*/
		// template <class InputIterator>
		// vector (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type())
		// {

		// }

		/*	Copy constructor	*/
		vector (const vector& x) : _size(x._size), _capacity(x._capacity), _al(x._al)
		{
			this->_container = this->_al.allocate(_capacity);
			for (size_type i = 0; i < this->_size; i++)
				this->_al.construct(this->_container + i, x._container[i]);
		}

		/*	DESTRUCTOR	*/
		virtual ~vector()
		{
			for (size_type i = 0; i < this->_size; i--)
				this->_al.destroy(this->_container + i);
			if (this->_container)
				this->_al.deallocate(this->_container, this->_capacity);
			return ;
		}

		/*	OPERATORS	*/
		vector& operator=(const vector& x)
		{
			if (this != &x)
			{
				for (size_type i = 0; i < this->_size; i--)
					this->_al.destroy(this->_container + i);
				if (this->_capacity < x._capacity)
				{
					if (this->_container)
						this->_al.deallocate(this->_container, this->_capacity);
					this->_capacity = x._capacity;
					this->_container = this->_al.allocate(this->_capacity);
				}
				this->_size = x._size;
				for (size_type i = 0; i < this->_size; i++)
					this->_al.construct(this->_container + i, x._container[i]);
			}
			return (*this);
		}

		reference operator[] (size_type n) { return (this->_container[n]); }

		/*	FUNCTIONS	*/

		/*	Iterators :	*/
		iterator 		begin() { return (iterator(this->_container)); }
		const_iterator	begin() const { return (iterator(this->_container)); }

		iterator		end() { return (iterator(this->_container + this->_size)); }
		const_iterator	end() const { return (iterator(this->_container + this->_size)); }

		/*	Capacity :	*/
		size_type	size() const { return (this->_size); }

		size_type	max_size() const { return (this->_al.max_size()); }

		void	resize(size_type n, value_type val = value_type())
		{
			if (n < this->_size)
			{
				for (size_type i = this->_size - 1; i < n; i++)
					this->_al.destroy(this->_container + i);
				this->_size = n;
			}
			if (n > this->_size)
			{
				reserve(n);
				for (size_type i = this->_size; i < n; i++)
					this->_al.construct(this->_container + i, val);
				this->_size = n;
			}
			return ;
		}

		size_type	capacity() const { return (this->_capacity); }

		bool	empty() const { return ((this->_size) ? false : true); }

		void	reserve (size_type n)
		{
			value_type	*tmp;

			if (n > this->_capacity)
			{
				tmp = this->_al.allocate(n);
				for (size_type i = 0; i < this->_size; i++)
					this->_al.construct(tmp + i, this->_container[i]);
				for (size_type i = 0; i < this->_size; i++)
					this->_al.destroy(this->_container + i);
				if (this->_container)
					this->_al.deallocate(this->_container, this->_capacity);
				this->_container = tmp;
				this->_capacity = n;
			}
			return ;
		}

		/*	Element access :	*/
		reference at(size_type n)
		{
			if (n > this->_size - 1)
				throw std::out_of_range();
			return (*(this->_container + n));
		}

		/*	Modifiers	*/

		// range version
		template <class InputIterator>
		void	assign(InputIterator first, InputIterator last)
		{
			const size_type	range = distance(first, last);
			size_type		i = 0;

			this->clear();
			if (range > this->_capacity)
				reserve(range);
			for (InputIterator tmp = first; tmp != last; tmp++)
			{
				this->_al.construct(this->_container + i, *tmp);
				i++;
			}
			this->_size = range;
			return ;
		}

		void	assign(size_type n, const value_type& val)
		{
			this->clear();
			if (n > this->_capacity)
				reserve(n);
			for (size_type i = 0; i < n; i++)
				this->_al.construct(this->_container + i, val);
			this->_size = n;
			return ;
		}

		void	push_back(const value_type& val)
		{
			if (this->_size == this->_capacity)
			{
				if (this->_capacity == 0)
					reserve(1);
				else
					reserve(this->_capacity * 2);
			}
			this->_al.construct(this->_container + this->_size, val);
			this->_size++;
			return ;
		}

		void	pop_back()
		{
			this->_al.destroy(this->_container + this->_size - 1);
			this->_size--;
		}

		void	clear()
		{
			for (size_type i = 0; i < this->_size; i++)
				this->_al.destroy(this->_container + i);
			this->_size = 0;
			return ;
		}

		private:
			/*	MEMBER VAR	*/
			size_type		_size;
			size_type		_capacity;
			value_type 		*_container;
			allocator_type	_al;

			template <class InputIterator>
			size_type	distance(InputIterator first, InputIterator last)
			{
				size_type	i = 0;

				for (InputIterator tmp = first; tmp != last; tmp++)
					i++;
				return (i);
			}
	};
}

#endif