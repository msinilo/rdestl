#include "rdestl/hash_map.h"
#include "rdestl/hash_map2.h"

namespace rde
{
//-----------------------------------------------------------------------------
hash_map_base::size_type hash_map_base::get_next_capacity(size_type n)
{
	static const size_type ls_primes[] = 
	{
		53ul,         97ul,         193ul,       389ul,       769ul,      
		1543ul,       3079ul,       6151ul,      12289ul,     24593ul,    
		49157ul,      98317ul,      196613ul,    393241ul,    786433ul,   
		1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul, 
		50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul
	};
	static const size_type N = sizeof(ls_primes) / sizeof(ls_primes[0]);
	for (size_type i = 0; i < N; ++i)
	{
		if (ls_primes[i] > n)
			return ls_primes[i];
	}
	n += ls_primes[N - 1];
	n |= 1;
	return n;
}

//-----------------------------------------------------------------------------
hash_map_base::size_type hash_map_base2_prime::get_next_capacity(size_type n)
{
	static const size_type ls_primes[] = 
	{
		53ul,         97ul,         193ul,       389ul,       769ul,      
		1543ul,       3079ul,       6151ul,      12289ul,     24593ul,    
		49157ul,      98317ul,      196613ul,    393241ul,    786433ul,   
		1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul, 
		50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul
	};
	static const size_type N = sizeof(ls_primes) / sizeof(ls_primes[0]);
	for (size_type i = 0; i < N; ++i)
	{
		if (ls_primes[i] > n)
			return ls_primes[i];
	}
	n += ls_primes[N - 1];
	n |= 1;
	return n;
}

} // namespace rde
