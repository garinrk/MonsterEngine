#ifndef MONSTERALLOCATOR_INL_H
#define MONSTERALLOCATOR_INL_H
inline size_t MonsterAllocator::GetAlignmentOffset(const void * addr) {
	return ALIGNMENT - (reinterpret_cast<size_t>(addr) % ALIGNMENT);
}
#endif