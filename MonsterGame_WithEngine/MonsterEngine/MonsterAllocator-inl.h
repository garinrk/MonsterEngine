#include "MonsterAllocator.h"
#ifndef MONSTERALLOCATOR_INL_H
#define MONSTERALLOCATOR_INL_H
inline size_t MonsterAllocator::GetAlignmentOffset(const void * addr,uint8_t align) {
	return align - (reinterpret_cast<size_t>(addr) % align);
}

#endif