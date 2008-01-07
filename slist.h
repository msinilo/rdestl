#ifndef RDESTL_SLIST_H
#define RDESTL_SLIST_H

#include "rdestl/allocator.h"
#include "rdestl/iterator.h"

namespace rdestl
{
//=============================================================================
template<typename T, class TAllocator = rdestl::allocator>
class slist
{
	private:
	// @todo: consider dividing into 2 classes.
	// base_node with next + node with value.
	// move some code to .cpp -> smaller coder -> win?
	struct node
	{
		node() 
		{
#if RDE_DEBUG
			reset();
#endif
		}
		explicit node(const T& v):	value(v) 
		{
#if RDE_DEBUG
			reset();
#endif
		}

		void reset()
		{
			next = this;
		}
		bool in_list() const { return this != next; }

		void link_after(node* prevNode)
		{
			RDE_ASSERT(!in_list());
			next = prevNode->next;
			prevNode->next = this;
		}
		void unlink(node* prevNode)
		{
			RDE_ASSERT(in_list());
			RDE_ASSERT(prevNode->next == this);
			prevNode->next = next;
			next = this;
		}

		node*	next;
		T		value;
	};
	template<typename TNodePtr, typename TPtr, typename TRef>
	class node_iterator
	{
	public:
		typedef forward_iterator_tag	iterator_category;

		explicit node_iterator(TNodePtr node):	m_node(node) {/**/}
		template<typename UNodePtr, typename UPtr, typename URef>
		node_iterator(const node_iterator<UNodePtr, UPtr, URef>& rhs)
		:	m_node(rhs.node())
		{
			/**/
		}

		TRef operator*() const
		{
			RDE_ASSERT(m_node != 0);
			return m_node->value;
		}
		TPtr operator->() const
		{
			return &m_node->value;
		}
		TNodePtr node() const
		{
			return m_node;
		}
		TNodePtr next() const
		{
			return m_node->next;
		}
		
		node_iterator& operator++()
		{
			m_node = m_node->next;
			return *this;
		}
		node_iterator operator++(int)
		{
			node_iterator copy(*this);
			++(*this);
			return copy;
		}

		bool operator==(const node_iterator& rhs) const
		{
			return rhs.m_node == m_node;
		}
		bool operator!=(const node_iterator& rhs) const
		{
			return !(rhs == *this);
		}

	private:
		TNodePtr	m_node;
	};

public:
	typedef T												value_type;
	typedef TAllocator										allocator_type;
	typedef size_t											size_type;
	typedef node_iterator<node*, T*, T&>					iterator;
	typedef node_iterator<const node*, const T*, const T&>	const_iterator;
	static const size_t										kNodeSize = sizeof(node);

	explicit slist(const allocator_type& allocator = allocator_type())
	:	m_allocator(allocator)
	{
		m_root.reset();
	}
	template<class InputIterator>
	slist(InputIterator first, InputIterator last, 
		const allocator_type& allocator = allocator_type())
	:	m_allocator(allocator)
	{
		m_root.reset();
		assign(first, last);
	}
	slist(const slist& rhs, const allocator_type& allocator = allocator_type())
	:	m_allocator(allocator)
	{
		m_root.reset();
		assign(rhs.begin(), rhs.end());
	}
	~slist()
	{
		clear();
	}

	slist& operator=(const slist& rhs)
	{
		if (this != &rhs)
		{
			assign(rhs.begin(), rhs.end());
		}
		return *this;
	}

	iterator begin()				{ return iterator(m_root.next); }
	const_iterator begin() const	{ return const_iterator(m_root.next); }
	iterator end()					{ return iterator(&m_root); }
	const_iterator end() const		{ return const_iterator(&m_root); }

	const T& front() const	{ RDE_ASSERT(!empty()); return m_root.next->value; }
	T& front()				{ RDE_ASSERT(!empty()); return m_root.next->value; }

	void push_front(const T& value)
	{
		node* newNode = construct_node(value);
		newNode->link_after(&m_root);
	}
	void pop_front()
	{
		RDE_ASSERT(!empty());
		m_root.next->unlink(&m_root);
	}
	void insert_after(iterator pos, const T& value)
	{
		node* newNode = construct_node(value);
		newNode->link_after(pos.node());
	}

	template<class InputIterator>
	void assign(InputIterator first, InputIterator last) 
	{
		clear();
		iterator it(&m_root);
		while (first != last)
		{
			insert_after(it, *first);
			++it;
			++first;
		}
	}

	void clear()
	{
		// quicker then erase(begin(), end())
		node* it = m_root.next;
		while (it != &m_root)
		{
			node* nextIt = it->next;
			destruct_node(it);
			it = nextIt;
		}
		m_root.reset();
	}	
	bool empty() const	{ return !m_root.in_list(); }
	// @todo: consider keeping size member, would make this O(1)
	// as a policy? via preprocessor macro? TBD
	size_type size() const
	{
		const node* it = m_root.next;
		size_type size(0);
		while (it != &m_root)
		{
			++size;
			it = it->next;
		}
		return size;
	}

	// @return iterator prev such that ++prev == nextIt
	// @note linear complexity
	static iterator previous(iterator nextIt)
	{
		RDE_ASSERT(nextIt.node()->in_list());
		iterator prevIt = nextIt;
		while (nextIt.node() != prevIt.next())
			++prevIt;
		return prevIt;
	}
	static const_iterator previous(const_iterator nextIt)
	{
		RDE_ASSERT(nextIt.node()->in_list());
		const_iterator prevIt = nextIt;
		while (nextIt.node() != prevIt.next())
			++prevIt;
		return prevIt;
	}


private:
	node* construct_node(const T& value)
	{
		void* mem = m_allocator.allocate(sizeof(node));
		return new (mem) node(value);
	}
	void destruct_node(node* n)
	{
		n->~node();
		m_allocator.deallocate(n, sizeof(node));
	}

	allocator_type	m_allocator;
	node			m_root;
};

} // namespace rdestl

//-----------------------------------------------------------------------------
#endif // RDESTL_SLIST_H
