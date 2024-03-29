#pragma once

#include <intrin.h> //bitscanforward
#include <new> //placement new
#include <string.h> //memset

#include "BlockAllocator.h"
#include "MonsterDebug.h" //logging

#pragma intrinsic(_BitScanForward) 

#if _WIN64
typedef uint64_t	bitContainer;

#define BITSCAN(index_found,value_to_search) _BitScanForward64(index_found,value_to_search)		
#else 
typedef uint32_t	bitContainer;
#define BITSCAN(index_found,value_to_search) _BitScanForward(index_found,value_to_search)		
#endif
namespace MMEngine {
	class BitArray
	{
	public:
		static BitArray * Create(const size_t num_of_bits, MMEngine::BlockAllocator * my_allocator);
		~BitArray();

		void ClearAll();
		void SetAll();

		bool AreAllClear() const;
		bool AreAllSet() const;

		bool IsSet(size_t bit_number) const;
		bool IsClear(size_t bit_number) const;

		void SetBit(const size_t bit_to_set);
		void ClearBit(const size_t bit_to_clear);

		bool GetFirstClearBit(size_t & o_index) const;
		bool GetFirstSetBit(size_t & o_index) const;

		inline bool operator[](const size_t index);
	private:
		BitArray(const size_t amt_of_user_requested_bits, size_t amt_of_bytes, size_t amt_of_containers, size_t* bits_array_addr);
		size_t number_of_bits_;
		size_t number_of_bytes;
		size_t number_of_containers;
		size_t* bits_;
		const size_t bits_per_byte = 8;

#ifdef _WIN64
		uint64_t BIT_CHECK = 1;
#else
		uint32_t BIT_CHECK = 1;
#endif


	};

}