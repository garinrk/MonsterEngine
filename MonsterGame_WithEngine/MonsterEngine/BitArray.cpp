#include "BitArray.h"

BitArray * BitArray::Create(const size_t num_of_bits, GAllocator * my_allocator)
{
	//Don't forget!
	const size_t bits_per_byte = 8;
	
	//given the amount of bits needed, how many containers will we be needing? 32 - 64 dependent
	const size_t amt_per_container = bits_per_byte * sizeof(bitContainer);

	//now, how many containers are we gonna need to store the amount of bits that you need?
	size_t number_of_containers = (num_of_bits + (amt_per_container - 1)) / amt_per_container;

	//how much memory we gonna need for all these containers and the bit array structure itself?
	size_t total_bytes_needed = number_of_containers * sizeof(bitContainer) + sizeof(BitArray);

	//let's get dat memory
	void * memory_chunk = my_allocator->GAlloc(total_bytes_needed);

	//but wait, where are the actual bits gonna be? Past the BitArray!
	size_t* bits_memory = reinterpret_cast<size_t*>(reinterpret_cast<uintptr_t>(memory_chunk) + sizeof(BitArray));

	//create dat bit array.
	return new (memory_chunk) BitArray(num_of_bits, total_bytes_needed, number_of_containers, bits_memory);
}

BitArray::~BitArray()
{
	//TODO: Check for outstanding allocations in a more graceful way?
	assert(AreAllClear());


}

void BitArray::ClearAll()
{
	memset(bits_, 0x00, number_of_bytes - sizeof(BitArray));
}

void BitArray::SetAll()
{
	memset(bits_, 0xFF, number_of_bytes - sizeof(BitArray));
}

bool BitArray::AreAllClear() const
{
	//using bitscanforward, look through all the containers
	for (size_t i = 0; i < number_of_containers; i++) {
		unsigned long index = 0;
		//position of container
		if (BITSCAN(&index, bits_[i])) {
			return false;
		}
	}

	return true;
}

bool BitArray::AreAllSet() const
{
	//using bitscanforward, look through all the containers
	for (size_t i = 0; i < number_of_containers; i++) {
		unsigned long index = 0;
		//position of container
		if (!BITSCAN(&index,bits_[i])) {
			return false;
		}


	}
	return true;
}

bool BitArray::IsSet(size_t bit_number) const
{
	//bit position within the container
	size_t bit_pos = bit_number % (sizeof(bitContainer) * bits_per_byte);

	//which container?
	size_t which_container = bit_number / (sizeof(bitContainer) * bits_per_byte);
	
	size_t val = bits_[which_container] & (BIT_CHECK << bit_pos);
	if (val)
		return true;
	else
		return false;
}

bool BitArray::IsClear(size_t bit_number) const
{
	//bit position within the container
	size_t bit_pos = bit_number % (sizeof(bitContainer) * bits_per_byte);
	size_t which_container = bit_number / (sizeof(bitContainer) * bits_per_byte);


	//which container?
	size_t val = (*(bits_ + which_container) >> bit_pos) & 1;
	if (!val)
		return true;
	else
		return false;
}

void BitArray::SetBit(const size_t bit_to_set)
{
	size_t bit_pos = bit_to_set % (sizeof(bitContainer)*bits_per_byte);
	size_t which_container = bit_to_set / (sizeof(bitContainer)*bits_per_byte);
	*(bits_ + which_container) |= BIT_CHECK << bit_pos;
}

void BitArray::ClearBit(const size_t bit_to_clear)
{
	size_t bit_pos = bit_to_clear % (sizeof(bitContainer)*bits_per_byte);
	size_t which_container = bit_to_clear / (sizeof(bitContainer)*bits_per_byte);
	*(bits_ + which_container) &= ~(BIT_CHECK << bit_to_clear);
}

bool BitArray::GetFirstClearBit(size_t & o_index) const
{
	size_t bit_pos = 0;
	size_t bits_left = 0;
	//negate and use the inverse to find the first set
	for (size_t i = 0; i < number_of_containers; i++) {
		unsigned long index = 0;

		if (i == number_of_containers - 1) {
			bits_left = number_of_bits_ % (sizeof(bitContainer) * bits_per_byte);
			if (bits_left == 0) {
				bits_left = sizeof(bitContainer) * bits_per_byte;
			}
			size_t index = 0;
			while (index < bits_left) {
				bit_pos = index + (sizeof(bitContainer) * bits_per_byte) * i;
				if (IsClear(bit_pos)) {
					o_index = bit_pos;
					return true;
				}
				index++;
			}

		}
		else {
			//position of container
			if (BITSCAN(&index, ~bits_[i])) {
				o_index = (index + (sizeof(bitContainer) * bits_per_byte) * i);
				return true;
			}
		}
	}//for
	return false;
}

bool BitArray::GetFirstSetBit(size_t & o_index) const
{
	size_t bit_pos = 0;
	size_t bits_left = 0;
	//using bitscanforward, look through all the containers
	for (size_t i = 0; i < number_of_containers; i++) {
		unsigned long index = 0;

		if (i == number_of_containers - 1) {
			bits_left = number_of_bits_ % (sizeof(bitContainer) * bits_per_byte);

			if (bits_left == 0) {
				bits_left = sizeof(bitContainer) * bits_per_byte;
			}
			
			size_t index = 0;
			while (index < bits_left) {
				bit_pos = index + (sizeof(bitContainer) * bits_per_byte) * i;
				if (IsSet(bit_pos)) {
					o_index = bit_pos;
					return true;
				}
				index++;
			}
		}
		else {
			//position of container
			if (BITSCAN(&index, bits_[i])) {
				o_index = static_cast<size_t>(index + (sizeof(bitContainer) * bits_per_byte) * i);
				return true;
			}
		}
	}

	return false;
}

inline bool BitArray::operator[](const size_t index)
{
	return IsSet(index);
}

//////////
//PRIVATES
//////////
BitArray::BitArray(const size_t amt_of_user_requested_bits, size_t amt_of_bytes, size_t amt_of_containers, size_t* bits_array_addr) :
	number_of_bits_(amt_of_user_requested_bits),
	number_of_bytes(amt_of_bytes),
	number_of_containers(amt_of_containers),
	bits_(bits_array_addr)
	
{
	DEBUGLOG("BitArray created for %zu bits\t%zu containers\t%zu bytes ", amt_of_user_requested_bits,amt_of_containers, amt_of_bytes);

	//lets set all da containers to empty.
	memset(bits_, 0x00, amt_of_bytes - sizeof(BitArray));

}