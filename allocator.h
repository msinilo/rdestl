#ifndef RDESTL_ALLOCATOR_H
#define RDESTL_ALLOCATOR_H

namespace rde
{

// CONCEPT!
class allocator
{
public:
	explicit allocator(const char* name = "DEFAULT"):	m_name(name) {}
	~allocator() {}

	void* allocate(size_t bytes, int flags = 0);
	void* allocate_aligned(size_t bytes, size_t alignment, int flags = 0);
	void deallocate(void* ptr, size_t bytes);

	const char* get_name() const;

private:
	const char*	m_name;
};

// True if lhs can free memory allocated by rhs and vice-versa.
inline bool operator==(const allocator& /*lhs*/, const allocator& /*rhs*/)
{
	return true;
}

} // namespace rde

//-----------------------------------------------------------------------------
#endif // #ifndef RDESTL_ALLOCATOR_H
