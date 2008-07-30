#pragma once

#include "rdestl/algorithm.h"
#include "rdestl/allocator.h"
#include "rdestl/functional.h"
#include "rdestl/pair.h"

namespace rde
{
class hash_map_base2_power2
{
public:
	typedef size_t	size_type;

protected:
	RDE_FORCEINLINE static size_type get_next_capacity(size_type current_capacity)
	{
		return current_capacity < 64 ? 64 : current_capacity * 2;
	}
	inline static bool invariant(size_type capacity)
	{
		return (capacity & (capacity - 1)) == 0;
	}
	RDE_FORCEINLINE static size_type get_bucket(size_type hash, size_type capacity)
	{
		return hash & (capacity - 1);
	}
	void set_capacity(size_type) {}
};

class hash_map_base2_prime
{
public:
	typedef size_t	size_type;

protected:
	static size_type get_next_capacity(size_type current_capacity);
	inline static bool invariant(size_type)
	{
		return true;
	}
	RDE_FORCEINLINE size_type get_bucket(size_type hash, size_type capacity) const
	{
		return hash % capacity;
	}
};


// TLoadFactor4 determines the max load factor of the hash table.
// Basically, it's used like: 
//	if (numEntries * 4 >= capacity * loadFactor4)
//		grow&rehash.
// So, LoadFactor4 == 1 gives 25% load
// LoadFactor4 == 2 - 50% load
// LoadFactor4 == 3 - 75% load and so on.
template<typename TKey, typename TValue, 
	class THashFunc = rde::hash<TKey>, 
	class TKeyEqualFunc = rde::equal_to<TKey>,
	class TBase = hash_map_base2_power2,
	int TLoadFactor4 = 2,
	class TAllocator = rde::allocator>
class hash_map2 : public TBase
{
	typedef TBase	Base;
public:
	typedef typename TBase::size_type		size_type;
	typedef rde::pair<TKey, TValue>		value_type;

private:
	struct node
	{
		static const unsigned int kTombstoneHash = 0xFFFFFFFF;

		node(): hash(kTombstoneHash) {}

		RDE_FORCEINLINE bool is_used() const	{ return hash != kTombstoneHash; }
		RDE_FORCEINLINE void set_unused()		{ hash = kTombstoneHash; }

		value_type		data;
		unsigned int	hash;
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
			return rhs.m_node == m_node && m_map == rhs.get_map();
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
				if (m_map->m_buckets[i].is_used())
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
			if (m_buckets[i].is_used())
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
		//if (m_numEntries * 4 >= m_capacity * 2)
		if (m_numEntries * 4 >= m_capacity * (size_type)TLoadFactor4)
			grow(m_capacity);

		const size_type hash = hash_func(value.first);
		size_t i(hash);
		node* currentNode(0);
		size_type hadCollisions(0);
#if RDE_HASHMAP_TRACK_LONGEST_CLUSTER
		size_type longestCluster(0);
#endif
		while (true)
		{
			i = get_bucket(i);

			currentNode = &m_buckets[i];
			// Empty node with proper hashing?
			if (!currentNode->is_used())
				break;

			// Entry with this key already exists?
			if (m_keyEqualFunc(value.first, currentNode->data.first))
			{
				return pair<iterator, bool>(iterator(currentNode, i, this), false);
			}
			hadCollisions = 1;

			++i;
#if RDE_HASHMAP_TRACK_LONGEST_CLUSTER
			++longestCluster;
			if (longestCluster > m_longestCluster)
				m_longestCluster = longestCluster;
#endif
		}
		m_numCollisions += hadCollisions;
		rde::copy_construct(&currentNode->data, value);
		currentNode->hash = (unsigned int)hash;
		++m_numEntries;
		RDE_ASSERT(currentNode == &m_buckets[i]);
		return pair<iterator, bool>(iterator(currentNode, i, this), true);
	}
	template<class InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		for (InputIterator it = first; it != last; ++it)
			insert(*it);
	}

	iterator find(const key_type& key)
	{
		if (empty())
			return end();
		const size_type index = find_bucket(key);
		node* keyNode = m_buckets[index].is_used() ? &m_buckets[index] : 0;
		return iterator(keyNode, index, this);
	}
	const_iterator find(const key_type& key) const
	{
		if (empty())
			return end();
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
			RDE_ASSERT(m_buckets[i].is_used());
			size_type j(i);
			// Remove "holes" in case of collisions.
			// We cannot have unused nodes in our chains of items resolving to
			// same bucket.
			while (true)
			{
				j = get_bucket(j + 1);
				if (!m_buckets[j].is_used())
					break;
				const size_type k = get_bucket(m_buckets[j].hash);
				if ((j > i && (k <= i || k > j)) ||
					(j < i && (k <= i && k > j)))
				{
					m_buckets[i] = m_buckets[j];
					i = j;
				}
			}
			RDE_ASSERT(m_buckets[i].is_used());
			m_buckets[i].set_unused();
			rde::destruct(&m_buckets[i].data);
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
			m_buckets[i].set_unused();

			//m_buckets[i].used = false;
		m_numEntries = 0;
		m_numCollisions = 0;
	}

	size_type used_memory() const
	{
		return kNodeSize * m_capacity;
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
	void grow(size_type new_capacity_hint)
	{
		RDE_ASSERT(invariant());
		new_capacity_hint = Base::get_next_capacity(new_capacity_hint);
#if RDE_HASHMAP_TRACK_LONGEST_CLUSTER
		m_longestCluster = 0;
#endif
		if (new_capacity_hint > m_capacity)
		{
			node* newBuckets = allocate_buckets(new_capacity_hint);
			rehash(newBuckets, new_capacity_hint);
			//delete_buckets();
			m_allocator.deallocate(m_buckets, sizeof(node) * m_capacity);
			m_buckets = newBuckets;
			m_capacity = new_capacity_hint;
		}
		RDE_ASSERT(invariant());
	}

	void rehash(node* new_buckets, size_type new_capacity)
	{
		m_numCollisions = 0;
		node* bucket = &m_buckets[0];
		node* bucketEnd = bucket + m_capacity;
		for (/**/; bucket != bucketEnd; ++bucket)
		{
			node* currentNode = bucket;
			if (currentNode->is_used())
			{
				const size_type hash(currentNode->hash);
				size_type j = get_bucket(hash, new_capacity);
				size_type hadCollisions(0);
#if RDE_HASHMAP_TRACK_LONGEST_CLUSTER
				size_type longestCluster(0);
#endif
				while (new_buckets[j].is_used())
				{
					j = get_bucket(j + 1, new_capacity);
					hadCollisions = 1;
#if RDE_HASHMAP_TRACK_LONGEST_CLUSTER
					++longestCluster;
					if (longestCluster > m_longestCluster)
						m_longestCluster = longestCluster;
#endif
				}
				node* newNode = &new_buckets[j];
				RDE_ASSERT(!newNode->is_used());
				rde::copy_construct(&newNode->data, currentNode->data);
				//newNode->data = currentNode->data;
				//newNode->used = true;
				newNode->hash = (unsigned long)hash;
				rde::destruct(&currentNode->data);
				m_numCollisions += hadCollisions;
			}
		}
	}

	size_type find_bucket(const key_type& key) const
	{
		size_type i = get_bucket(hash_func(key));
		while (m_buckets[i].is_used() && !m_keyEqualFunc(key, m_buckets[i].data.first))
			i = get_bucket(i + 1);
		return i;
	}

	node* allocate_buckets(size_type n)
	{
		node* buckets = static_cast<node*>(m_allocator.allocate(n * sizeof(node)));
		node* iterBuckets(buckets);
		for (size_type i = 0; i < n; ++i, ++iterBuckets)
			iterBuckets->set_unused();
			
		return buckets;
	}
	void delete_buckets()
	{
		//rde::destruct_n(m_buckets, m_capacity);
		for (size_t i = 0; i < m_capacity; ++i)
		{
			if (m_buckets[i].is_used())
				rde::destruct(&m_buckets[i].data);
		}
		m_allocator.deallocate(m_buckets, sizeof(node) * m_capacity);
	}
	RDE_FORCEINLINE size_type get_bucket(size_type hash) const
	{
		RDE_ASSERT(invariant());
		return Base::get_bucket(hash, m_capacity);
	}
	RDE_FORCEINLINE size_type get_bucket(size_type hash, size_type capacity) const
	{
		RDE_ASSERT(invariant());
		return Base::get_bucket(hash, capacity);
	}
	RDE_FORCEINLINE size_type hash_func(const key_type& key) const
	{
		const size_type h = m_hashFunc(key);
		RDE_ASSERT(h != node::kTombstoneHash);
		return h;
	}
	bool invariant() const
	{
		return Base::invariant(m_capacity);
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
