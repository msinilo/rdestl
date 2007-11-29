#ifndef RDE_ALLOCATOR_H
#define RDE_ALLOCATOR_H

namespace rdestl
{

// CONCEPT!
class allocator
{
public:
	explicit allocator(const char* name = "DEFAULT"):	m_name(name) {}
	~allocator() {}

	void* allocate(size_t bytes, int flags = 0);
	void deallocate(void* ptr, size_t bytes);

	const char* get_name() const;

private:
	const char*	m_name;
};

} // namespace rdestl

#endif // #ifndef RDE_ALLOCATOR_H
