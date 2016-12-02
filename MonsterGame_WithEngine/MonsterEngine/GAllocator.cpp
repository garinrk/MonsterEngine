#include "GAllocator.h"

#include <new>
#include <assert.h>
#include <inttypes.h>
#include <malloc.h>

GAllocator::GAllocator(const size_t total_allocator_size, const unsigned int num_of_descriptors, const uint8_t alignment)
{
	front_of_chunk_ = _aligned_malloc(total_allocator_size, alignment);
	assert(front_of_chunk_ != NULL && "Null Memory Allocation in Allocator Creation");

	back_of_chunk = reinterpret_cast<char*>(front_of_chunk_) + total_allocator_size;
	InitializeFreeList(num_of_descriptors);
	
}

void * GAllocator::GAlloc(const size_t amt)
{
	return GAlloc(amt, 4);
}

void * GAllocator::GAlloc(const size_t amt, const uint8_t alignment) {
	void * return_to_user = NULL;

	assert(IsPowerOfTwo(alignment));

	if (!IsPowerOfTwo(alignment))
		return nullptr;

	//check to see if we need to garbage collect some nodes
	if (tail_of_free_ == NULL) {
		GGCollect();
	}

	_Descriptor * sufficiently_sized_block = FindSuitableUnallocatedBlock(amt, alignment);

	assert(sufficiently_sized_block != NULL && "Ran out of descriptors after garbage collecting");

	//even after garbage collecting we don't have a descriptor
	if (sufficiently_sized_block == NULL)
		return nullptr;

	_Descriptor * new_descriptor;


	return return_to_user;
}

void GAllocator::GGCollect() {

	//looking through the list of unallocated blocks, combining any
	//that are right next to one another, into a single larger
	//free block

	_Descriptor * conductor = unallocated_root_;

	bool finished = false;

	//there are no unallocated blocks to look through
	if (unallocated_root_ == 0)
		return;

	while (conductor != NULL) {
		uintptr_t * addr_to_search_for = static_cast<uintptr_t*>(conductor->base) + conductor->master_size;
		_Descriptor * found_block = SearchForBlock(addr_to_search_for, unallocated_root_);

		if (found_block == NULL) {
			conductor = conductor->next;
		}
		else {
			CombineBlocks(found_block, conductor);
		}
	}

}

GAllocator::~GAllocator()
{
	_aligned_free(front_of_chunk_);
}

void GAllocator::PrintList(_Descriptor * root)
{

	_Descriptor * conductor = root;

	while (conductor != NULL) {
		DEBUGLOG("[addr:0x%04x id:%d]: prev:0x%04x\tblockptr:%04x\tUserptr:%04x\tsize:%zu\tnext:0x%04x", conductor, conductor->debug_id, conductor->prev, conductor->base, conductor->user_ptr, conductor->master_size, conductor->next);
		conductor = conductor->next;
	}
}

////////////////
////PRIVATES
////////////////

void GAllocator::InitializeFreeList(const unsigned int num_of_descriptors)
{

	front_of_pool_ = reinterpret_cast<_Descriptor*>(reinterpret_cast<uintptr_t*>(back_of_chunk) - (sizeof(_Descriptor) * num_of_descriptors));

	//make sure we didn't create too many descriptors
	assert(front_of_pool_ > front_of_chunk_ && "Too many descriptors, pool is larger than the total size");

	//set up front of pool
	front_of_pool_->prev = NULL;
	front_of_pool_->base = NULL;
	front_of_pool_->master_size = 0;
	front_of_pool_->user_ptr = NULL;
	front_of_pool_->user_size = 0;

	_Descriptor * conductor = front_of_pool_;

#ifdef _DEBUG
	conductor->debug_id = 0;
#endif
	
	for (size_t i = 0; i < num_of_descriptors - 1; i++) {
		_Descriptor * new_descriptor = conductor + 1;

		assert(new_descriptor < reinterpret_cast<_Descriptor*>(back_of_chunk));
		//TODO: Shouldn't need this...
		////just in case we go too far...
		//if (new_descriptor >= reinterpret_cast<_Descriptor*>(back_of_chunk))
		//	break;

		//set links
		conductor->next = new_descriptor;
		new_descriptor->prev = conductor;
		new_descriptor->next = NULL;

		//set properties
		new_descriptor->base = NULL;
		new_descriptor->master_size = 0;
		new_descriptor->user_ptr = NULL;
		new_descriptor->user_size = 0;

#ifdef _DEBUG
		new_descriptor->debug_id = static_cast<int>(i) + 1;
#endif // _DEBUG


		conductor = new_descriptor;
	}

	//free list properties
	free_root_ = front_of_pool_;
	tail_of_free_ = conductor;

	total_bytes_left = front_of_pool_ - static_cast<_Descriptor*>(front_of_chunk_);

	//initial unallocated block

	_Descriptor * init_unalloc_block;

	//steals the last free block to set itself up
	init_unalloc_block = tail_of_free_;
	tail_of_free_ = tail_of_free_->prev;
	tail_of_free_->next = NULL;

	//set other block properties
	init_unalloc_block->prev = NULL;
	init_unalloc_block->base = front_of_chunk_;
	init_unalloc_block->master_size = total_bytes_left;
	init_unalloc_block->next = NULL;
	init_unalloc_block->user_ptr = NULL;
	init_unalloc_block->user_size = 0;

	AddToUnallocatedList(init_unalloc_block);

	DEBUGLOG("FREE:");
	PRINT_LIST(free_root_);
	DEBUGLOG("UNALLOC:");
	PRINT_LIST(unallocated_root_);

}

void GAllocator::AddToAllocatedList(const _Descriptor * node_to_insert)
{
}


void GAllocator::AddToUnallocatedList(_Descriptor * node_to_insert)
{
	_Descriptor * conductor = unallocated_root_;

	if (unallocated_root_ == 0) {
		unallocated_root_ = node_to_insert;
		unallocated_root_->prev = NULL;
		unallocated_root_->next = NULL;
	}
	else {
		while (conductor->next != NULL) {
			conductor = conductor->next;
		}

		conductor->next = node_to_insert;
		node_to_insert->prev = conductor;

	}
}

void GAllocator::AddToFreeList(const _Descriptor * node_to_insert)
{
}

bool GAllocator::CheckGuardBands(_Descriptor * node_to_check)
{
	return false;
}

bool GAllocator::CombineBlocks(_Descriptor * first, _Descriptor * second)
{
	return false;
}

_Descriptor * GAllocator::FindSuitableUnallocatedBlock(const size_t amt, uint8_t alignment) const
{
	_Descriptor * conductor = unallocated_root_;

	size_t toLookFor = amt;/*+ alignment;*/

#ifdef _DEBUG
	toLookFor += BAND_SIZE * 2;
#endif

	while (conductor != NULL)
	{
		if (conductor->master_size >= toLookFor) {
			return conductor;
		}
		else {
			conductor = conductor->next;
		}
	}

	return NULL;
}

_Descriptor * GAllocator::StealFromBlock(const _Descriptor * victim, const size_t amt_to_take, const uint8_t alignment)
{

	//grab a descriptor from the free list
	_Descriptor * thief = tail_of_free_;
	if (thief->prev != NULL) {
		tail_of_free_ = thief->prev;
	}

	tail_of_free_->next = NULL;
	thief->prev = NULL;
	thief->next = NULL;

	size_t entire_amt_to_take = amt_to_take;

#ifdef _DEBUG
	entire_amt_to_take += BAND_SIZE * 2;
#endif

	//take the front of the victim's space
	thief->base = victim->base;
	thief->user_ptr = thief->base;

	return nullptr;
}

bool GAllocator::IsPowerOfTwo(uint8_t input)
{
	uint8_t val = (input != 0) && !(input & (input - 1));

	if (val == 1)
		return true;
	else
		return false;
}
