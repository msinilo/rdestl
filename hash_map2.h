#pragma once

#include "rdestl/algorithm.h"
#include "rdestl/allocator.h"
#include "rdestl/functional.h"
#include "rdestl/pair.h"

namespace rde
{
class hash_map_base2
{
public:
	typedef size_t	size_type;
};

template<typename TKey, typename TValue, 
	class THashFunc = rde::hash<TKey>, 
	class TKeyEqualFunc = rde::equal_to<TKey>,
	class TAllocator = rde::allocator>
class hash_map2 : public hash_map_base2
{
public:
	typedef rde::pair<TKey, TValue>						value_type;

private:
	struct node
	{
		node():	used(false) {}

		value_type		data;
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

		explicit node_iterator(TNodePtr node, size_type index, hash_map2* map)
		:	m_node(node),
			m_index(index),
			m_map(map)
		{/**/}
		template<typename UNodePtr, typename UPtr, typename URef>
		node_iterator(const node_iterator<UNodePtr, UPtr, URef>& rhs)
		:	m_node(rhs.node()),
			m_index(rhs.m_index),
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
			m_node = update_next_node();
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

		hash_map2* get_map() const { return m_map; }
		RDE_FORCEINLINE size_type get_index() const { return m_index; }
	private:
		TNodePtr update_next_node() 
		{
			const size_type nodeBucket = m_index;
			const size_type numBuckets = m_map->bucket_count();
			for (size_type i = nodeBucket + 1; i < numBuckets; ++i)
			{
				if (m_map->m_buckets[i].used)
				{
					m_index = i;
					return &m_map->m_buckets[i];
				}
			}
			return 0;
		}

		TNodePtr	m_node;
		size_type	m_index;
		hash_map2*	m_map;
	};

public:
	typedef TKey															key_type;
	typedef TValue															mapped_type;
	typedef node_iterator<node*, value_type*, value_type&>					iterator;
	typedef node_iterator<node*, const value_type*, const value_type&>	const_iterator;
	typedef TAllocator														allocator_type;
	static const size_type													kNodeSize = sizeof(node);

	friend class iterator;

	explicit hash_map2(const allocator_type& allocator = allocator_type())
	:	m_buckets(0),
		m_capacity(0),
		m_numEntries(0),
		m_numCollisions(0),
		m_allocator(allocator)
	{
		/**/
	}
	explicit hash_map2(size_type initial_bucket_count,
		const allocator_type& allocator = allocator_type())
	:	m_buckets(0),
		m_capacity(0),
		m_numEntries(0),
		m_numCollisions(0),
		m_allocator(allocator)
	{
		resize(initial_bucket_count);
	}
	explicit hash_map2(size_type initial_bucket_count,
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
	~hash_map2()
	{
		delete_buckets();
	}

	iterator begin() 
	{
		for (size_type i = 0; i < m_capacity; ++i)
		{
			if (m_buckets[i].used)
				return iterator(&m_buckets[i], i, this);
		}
		return end();
	}
	const_iterator begin() const
	{
		for (size_type i = 0; i < m_capacity; ++i)
		{
			if (m_buckets[i].used)
				return const_iterator(&m_buckets[i], i, this);
		}
		return end();
	}
	iterator end()				{ return iterator(0, m_capacity, this); }
	const_iterator end() const	{ return const_iterator(0, m_capacity, this); }

	pair<iterator, bool> insert(const value_type& value)
	{
		RDE_ASSERT(invariant());
		//if (m_numEntries * 4 >= m_capacity * 3)
		if (m_numEntries >= m_capacity / 2)
			grow(m_capacity);

		const size_type hash = m_hashFunc(value.first);
		size_t i(hash);
		node* currentNode(0);
		size_type hadCollisions(0);
		while (true)
		{
			i = get_bucket(i);

			currentNode = &m_buckets[i];
			// Empty node with proper hashing?
			if (!currentNode->used)
				break;

			// Entry with this key already exists?
			if (m_keyEqualFunc(value.first, currentNode->data.first))
				break;

			hadCollisions = 1;

			++i;
		}
		const bool found = currentNode->used;
		if (found)
			RDE_ASSERT(m_keyEqualFunc(value.first, currentNode->data.first));
		m_numCollisions += hadCollisions;
		currentNode->data = value;
		currentNode->used = true;
#if RDE_HASHMAP_CACHE_HASH
		currentNode->hash = (unsigned int)hash;
#endif
		++m_numEntries;
		RDE_ASSERT(currentNode == &m_buckets[i]);
		return pair<iterator, bool>(iterator(currentNode, i, this), !found);
	}
	template<class InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		for (InputIterator it = first; it != last; ++it)
			insert(*it);
	}

	iterator find(const key_type& key)
	{
		const size_type index = find_bucket(key);
		node* keyNode = m_buckets[index].used ? &m_buckets[index] : 0;
		return iterator(keyNode, index, this);
	}
	const_iterator find(const key_type& key) const
	{
		const size_type index = find_bucket(key);
		node* keyNode = m_buckets[index].used ? &m_buckets[index] : 0;
		return const_iterator(keyNode, index, this);
	}

	void erase(iterator it)
	{
		RDE_ASSERT(!empty());
		RDE_ASSERT(invariant());
		if (it != end())
		{
			size_type i = it.get_index();
			RDE_ASSERT(m_buckets[i].used);
			size_type j(i);
			// Remove "holes" in case of collisions.
			// We cannot have unused nodes in our chains of items resolving to
			// same bucket.
			while (true)
			{
				j = get_bucket(j + 1);
				if (!m_buckets[j].used)
					break;
#if RDE_HASHMAP_CACHE_HASH
				const size_type k = get_bucket(m_buckets[j].hash);
#else
				const size_type k = get_bucket(m_hashFunc(m_buckets[j].data.first));
#endif
				if ((j > i && (k <= i || k > j)) ||
					(j < i && (k <= i && k > j)))
				{
					m_buckets[i] = m_buckets[j];
					i = j;
				}
			}
			RDE_ASSERT(m_buckets[i].used);
			m_buckets[i].used = false;
			--m_numEntries;
		}
		RDE_ASSERT(invariant());
	}
	size_type erase(const key_type& k)
	{
		iterator i(find(k));
        if (i == end()) 
			return 0;
        erase(i);
        return 1;
	}

	// @todo: doesnt make much sense here...
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
			m_buckets[i].used = false;
		m_numEntries = 0;
		m_numCollisions = 0;
	}

	size_type used_memory() const
	{
		return kNodeSize * m_capacity;
	}

private:
	void grow(size_type new_capacity_hint)
	{
		RDE_ASSERT(invariant());
		new_capacity_hint = (new_capacity_hint < 64 ? 64 : new_capacity_hint * 2);
		if (new_capacity_hint > m_capacity)
		{
			node* newBuckets = allocate_buckets(new_capacity_hint);
			rehash(newBuckets, new_capacity_hint);
			delete_buckets();
			m_buckets = newBuckets;
			m_capacity = new_capacity_hint;
		}
		RDE_ASSERT(invariant());
	}

	void rehash(node* new_buckets, size_type new_capacity)
	{
		m_numCollisions = 0;
		for (size_type i = 0; i < m_capacity; ++i)
		{
			node* currentNode = &m_buckets[i];
			if (currentNode->used)
			{
#if RDE_HASHMAP_CACHE_HASH
				const size_type hash(currentNode->hash);
#else
				const size_type hash = m_hashFunc(currentNode->data.first);
#endif
				size_type j = get_bucket(hash, new_capacity);
				size_type hadCollisions(0);
				while (new_buckets[j].used)
				{
					j = get_bucket(j + 1, new_capacity);
					hadCollisions = 1;
				}
				node* newNode = &new_buckets[j];
				RDE_ASSERT(!newNode->used);
				*newNode = *currentNode;
				m_numCollisions += hadCollisions;
			}
		}
	}

	size_type find_bucket(const key_type& key) const
	{
		size_type i = get_bucket(m_hashFunc(key));
		while (m_buckets[i].used && !m_keyEqualFunc(key, m_buckets[i].data.first))
			i = get_bucket(i + 1);
		return i;
	}

	node* allocate_buckets(size_type n)
	{
		node* buckets = static_cast<node*>(m_allocator.allocate(n * sizeof(node)));
		rde::construct_n(buckets, n);
		return buckets;
	}
	void delete_buckets()
	{
		rde::destruct_n(m_buckets, m_capacity);
		m_allocator.deallocate(m_buckets, sizeof(node) * m_capacity);
	}
	RDE_FORCEINLINE size_type get_bucket(size_type hash) const
	{
		RDE_ASSERT(invariant());
		return hash & (m_capacity - 1);
	}
	RDE_FORCEINLINE size_type get_bucket(size_type hash, size_type capacity) const
	{
		RDE_ASSERT(invariant());
		return hash & (capacity - 1);
	}

	bool invariant() const
	{
		return (m_capacity & (m_capacity - 1)) == 0;
	}

	node*			m_buckets;
	size_type		m_capacity;
	size_type		m_numEntries;
	size_type		m_numCollisions;
	THashFunc		m_hashFunc;
	TKeyEqualFunc	m_keyEqualFunc;
	allocator_type	m_allocator;
};

} // namespace rde
