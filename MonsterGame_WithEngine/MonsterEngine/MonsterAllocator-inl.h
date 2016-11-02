#pragma once
inline size_t MonsterAllocator::GetAlignmentOffset(const void * addr) {
	return ALIGNMENT - (reinterpret_cast<size_t>(addr) % ALIGNMENT);
}