#ifndef RDESTL_ALLOCATOR_H
#define RDESTL_ALLOCATOR_H

namespace rde
{
// CONCEPT!
class allocator
{
public:
	explicit allocator(const char* name = "DEFAULT"): m_name(name) {}
	allocator(const allocator&) = default;
	allocator(allocator&&) = default;
	~allocator() {}

	allocator& operator=(const allocator&) = default;

	void* allocate(unsigned int bytes, int flags = 0);
	// Not supported for standard allocator for the time being.
	void* allocate_aligned(unsigned int bytes, unsigned int alignment, int flags = 0);
	void deallocate(void* ptr, unsigned int bytes);

	const char* get_name() const;

private:
	const char*	m_name;
};

// True if lhs can free memory allocated by rhs and vice-versa.
inline bool operator==(const allocator& /*lhs*/, const allocator& /*rhs*/)
{
	return true;
}
inline bool operator!=(const allocator& lhs, const allocator& rhs)
{
	return !(lhs == rhs);
}

inline void* allocator::allocate(unsigned int bytes, int)
{
	return operator new(bytes);
}

inline void allocator::deallocate(void* ptr, unsigned int)
{
	operator delete(ptr);
}

} // namespace rde

//-----------------------------------------------------------------------------
#endif // #ifndef RDESTL_ALLOCATOR_H
