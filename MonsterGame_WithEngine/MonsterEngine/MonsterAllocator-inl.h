#pragma once
inline size_t MonsterAllocator::GetAlignmentOffset(void * addr) {
	return ALIGNMENT - (reinterpret_cast<size_t>(addr) % ALIGNMENT);
}