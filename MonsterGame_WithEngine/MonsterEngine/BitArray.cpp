#include "BitArray.h"



BitArray::BitArray(const size_t num_of_bits) :
	number_of_bits_(num_of_bits)
{
	DEBUGLOG("Creating Bitarray");
	DEBUGLOG("Size of size_t: %d", sizeof(size_t));
}


BitArray::~BitArray()
{
	//TODO: Check for outstanding allocations
}
