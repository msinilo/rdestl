#ifndef RDESTL_HASH_MAP_H
#define RDESTL_HASH_MAP_H

#include "rdestl/algorithm.h"
#include "rdestl/allocator.h"
#include "rdestl/functional.h"
#include "rdestl/pair.h"

namespace rde
{

//=============================================================================
class hash_map_base
{
public:
	typedef size_t	size_type;
protected:
	static size_type get_next_capacity(size_type);
};

//=============================================================================
// @todo: 2 allocators? one for buckets/one for single nodes?
template<typename TKey, typename TValue, 
	class THashFunc = rde::hash<TKey>, 
	class TKeyEqualFunc = rde::equal_to<TKey>,
	class TAllocator = rde::allocator>
class hash_map : public hash_map_base
{
public:
	typedef rde::pair<TKey, TValue>						value_type;

private:
	struct node
	{
		node():	next(0), used(false) {/**/}

		value_type		data;
		node*			next;
#if RDE_HASHMAP_CACHE_HASH
		unsigned int	hash;
#endif
		bool			used;
	};

	template<typename TNodePtr, typename TPtr, typename TRef>
	class node_iterator
	{
	public:
		typedef forward_iterator_tag	iterator_category;

		explicit node_iterator(TNodePtr node, hash_map* map)
		:	m_node(node),
			m_map(map)
		{/**/}
		template<typename UNodePtr, typename UPtr, typename URef>
		node_iterator(const node_iterator<UNodePtr, UPtr, URef>& rhs)
		:	m_node(rhs.node()),
			m_map(rhs.get_map())
		{
			/**/
		}

		TRef operator*() const
		{
			RDE_ASSERT(m_node != 0);
			return m_node->data;
		}
		TPtr operator->() const
		{
			return &m_node->data;
		}
		TNodePtr node() const
		{
			return m_node;
		}

		node_iterator& operator++() 
		{
			RDE_ASSERT(m_node != 0);
			TNodePtr next = m_node->next;
			if (next == 0)
				next = find_next_node(m_node);
			m_node = next;
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

		hash_map* get_map() const { return m_map; }
	private:
		TNodePtr find_next_node(TNodePtr node) const
		{
			const size_type nodeBucket = m_map->get_bucket(node);
			const size_type numBuckets = m_map->bucket_count();
			for (size_type i = nodeBucket + 1; i < numBuckets; ++i)
			{
				if (m_map->m_buckets[i].used)
					return &m_map->m_buckets[i];
			}
			return 0;
		}

		TNodePtr	m_node;
		hash_map*	m_map;
	};

public:
	typedef TKey															key_type;
	typedef TValue															mapped_type;
	typedef node_iterator<node*, value_type*, value_type&>					iterator;
	typedef node_iterator<node*, const value_type*, const value_type&>	const_iterator;
	typedef TAllocator														allocator_type;
	static const size_type													kNodeSize = sizeof(node);

	friend class iterator;

	explicit hash_map(const allocator_type& allocator = allocator_type())
	:	m_buckets(0),
		m_capacity(0),
		m_numEntries(0),
		m_numCollisions(0),
		m_allocator(allocator)
	{
		/**/
	}
	explicit hash_map(size_type initial_bucket_count,
		const allocator_type& allocator = allocator_type())
	:	m_buckets(0),
		m_capacity(0),
		m_numEntries(0),
		m_numCollisions(0),
		m_allocator(allocator)
	{
		resize(initial_bucket_count);
	}
	explicit hash_map(size_type initial_bucket_count,
		const THashFunc& hashFunc, 
		const allocator_type& allocator = allocator_type())
	:	m_buckets(0),
		m_capacity(0),
		m_numEntries(0),
		m_numCollisions(0),
		m_hashFunc(hashFunc),
		m_allocator(allocator)
	{
		resize(initial_bucket_count);
	}
	~hash_map()
	{
		delete_buckets();
	}

	iterator begin()
	{
		// gaaah.. slow... record 'lowest used bucket'?
		for (size_type i = 0; i < m_capacity; ++i)
		{
			for (node* it = &m_buckets[i]; it != 0; it = it->next)
			{
				if (it->used)
					return iterator(it, this);
			}
		}
		return end();
	}
	const_iterator begin() const
	{
		for (size_type i = 0; i < m_capacity; ++i)
		{
			for (node* it = &m_buckets[i]; it != 0; it = it->next)
			{
				if (it->used)
					return const_iterator(it, this);
			}
		}
		return end();
	}
	iterator end()				{ return iterator(0, this); }
	const_iterator end() const	{ return const_iterator(0, this); }

	pair<iterator, bool> insert(const value_type& value)
	{
		// Grow at fixed % of size/capacity ratio.
		if (m_numEntries * 4 >= m_capacity * 3)
			grow(m_capacity);
		bool found;
		node* inserted = insert_noresize(value, found);
		++m_numEntries;
		return pair<iterator, bool>(iterator(inserted, this), !found);
	}
	template<class InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		for (InputIterator it = first; it != last; ++it)
			insert(*it);
	}

	iterator find(const key_type& key)
	{
		return iterator(find_node(key), this);
	}
	const_iterator find(const key_type& key) const
	{
		return const_iterator(find_node(key), this);
	}

	void erase(iterator it)
	{
		RDE_ASSERT(!empty());
		node* itNode = it.node();
		const size_type bucket = get_bucket(itNode);
		node* prevNode(0);
		node* nextNode(0);
		for (node* first = &m_buckets[bucket]; first != 0; first = nextNode)
		{
			nextNode = first->next;
			if (first->used && itNode == first)
			{
				first->used = false;
				--m_numEntries;
				if (first != &m_buckets[bucket])
				{
					prevNode->next = first->next;
					destruct_node(first);
				}
				break; // ??
			}
			prevNode = first;
		}
	}
	size_type erase(const key_type& k)
	{
		iterator i(find(k));
        if (i == end()) 
			return 0;
        erase(i);
        return 1;
	}

	void resize(size_type new_bucket_count_hint)
	{
		grow(new_bucket_count_hint);
	}

	size_type size() const				{ return m_numEntries; }
	bool empty() const					{ return m_numEntries == 0; }
	size_type bucket_count() const		{ return m_capacity; }
	size_type collisions() const		{ return m_numCollisions; }

	void clear()
	{
		for (size_type i = 0; i < m_capacity; ++i)
		{
			for (node* it = &m_buckets[i]; it != 0; it = it->next)
				it->used = false;
		}
		m_numEntries = 0;
		m_numCollisions = 0;
	}

	size_type used_memory() const
	{
		size_type bytes(m_capacity * kNodeSize);
		for (size_type i = 0; i < m_capacity; ++i)
		{
			for (const node* it = m_buckets[i].next; it != 0; it = it->next)
				bytes += kNodeSize;
		}
		return bytes;
	}
	size_type longest_cluster() const
	{
#if RDE_HASHMAP_TRACK_LONGEST_CLUSTER
		return m_longestCluster;
#else
		return 0xFFFFFFFF;
#endif
	}

private:
	node* insert_noresize(const value_type& data, bool& found)
	{
		return insert(m_buckets, data, m_capacity, found);
	}
	node* insert(node* buckets, const value_type& data, size_type capacity, bool& found)
	{
		const size_type bucket = get_bucket(data.first, capacity);
		node* first = &buckets[bucket];
		node* freeNode(0);
		found = true;
		for (node* cur = first; cur != 0; cur = cur->next)
		{
			if (!cur->used)
				freeNode = cur;
			else if (m_keyEqualFunc(data.first, cur->data.first))
				return cur;
		}
		found = false;
		if (freeNode == 0)
		{
			++m_numCollisions;

			freeNode = construct_node();
			freeNode->next = first->next;
			first->next = freeNode;
#if RDE_HASHMAP_TRACK_LONGEST_CLUSTER
			size_type longestCluster(0);
			for (node* cur = first; cur != 0; cur = cur->next)
				++longestCluster;
			if (longestCluster > m_longestCluster)
				m_longestCluster = longestCluster;
#endif
		}
		freeNode->data = data;
#if RDE_HASHMAP_CACHE_HASH
		freeNode->hash = (unsigned int)m_hashFunc(data.first);
#endif
		freeNode->used = true;
		return freeNode;
	}
	// @note: avoid!
	void grow(size_type new_capacity_hint)
	{
		new_capacity_hint = get_next_capacity(new_capacity_hint);
#if RDE_HASHMAP_TRACK_LONGEST_CLUSTER
		m_longestCluster = 0;
#endif
		if (new_capacity_hint > m_capacity)
		{
			node* newBuckets = allocate_buckets(new_capacity_hint);
			m_numCollisions = 0;
			for (size_type bucket = 0; bucket < m_capacity; ++bucket)
			{
				node* first = &m_buckets[bucket];
				while (first != 0)
				{
					if (first->used)
					{
						bool found;
						insert(newBuckets, first->data, new_capacity_hint, found);
					}
					first = first->next;
				}
			}
			delete_buckets();
			m_buckets = newBuckets;
			m_capacity = new_capacity_hint;
		}
	}

	node* find_node(const key_type& key) const
	{
		const size_type bucket = get_bucket(key);
		node* first;
		for (first = &m_buckets[bucket]; 
			first && (!m_keyEqualFunc(key, first->data.first) || !first->used); first = first->next)
		{
			// empty
		}
		return first;
	}

	RDE_FORCEINLINE size_type get_bucket(const key_type& key) const
	{
		return (m_hashFunc(key) & 0x7FFFFFFF) % m_capacity;
	}
	RDE_FORCEINLINE size_type get_bucket(const node* n) const
	{
#if RDE_HASHMAP_CACHE_HASH
		return (n->hash & 0x7FFFFFFF) % m_capacity;
#else
		return (m_hashFunc(n->data.first) & 0x7FFFFFFF) % m_capacity;
#endif
	}
	RDE_FORCEINLINE size_type get_bucket(const key_type& key, size_type n) const
	{
		return (m_hashFunc(key) & 0x7FFFFFFF) % n;
	}

	node* construct_node()
	{
		void* mem = m_allocator.allocate(sizeof(node));
		return new (mem) node();
	}
	void destruct_node(node* n)
	{
		n->~node();
		m_allocator.deallocate(n, sizeof(node));
	}
	node* allocate_buckets(size_type n)
	{
		node* buckets = static_cast<node*>(m_allocator.allocate(n * sizeof(node)));
		rde::construct_n(buckets, n);
		return buckets;
	}
	void delete_buckets()
	{
		for (size_type i = 0; i < m_capacity; ++i)
		{
			for (node* it = m_buckets[i].next; it != 0; /**/)
			{
				node* itNext = it->next;
				destruct_node(it);
				it = itNext;
			}
		}
		rde::destruct_n(m_buckets, m_capacity);
		m_allocator.deallocate(m_buckets, sizeof(node) * m_capacity);
	}

	node*			m_buckets;
	size_type		m_capacity;
	size_type		m_numEntries;
	size_type		m_numCollisions;
	THashFunc		m_hashFunc;
	TKeyEqualFunc	m_keyEqualFunc;
	allocator_type	m_allocator;
#if RDE_HASHMAP_TRACK_LONGEST_CLUSTER
	size_type		m_longestCluster;
#endif
};

} // namespace rde

//-----------------------------------------------------------------------------
#endif // #ifndef RDESTL_HASH_MAP_H
