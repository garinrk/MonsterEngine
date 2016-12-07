#include "GAllocator.h"

#include <new>
#include <assert.h>
#include <inttypes.h>
#include <malloc.h>

GAllocator::GAllocator(const size_t total_allocator_size, const unsigned int num_of_descriptors, const uint8_t alignment)
{
	front_of_chunk_ = _aligned_malloc(total_allocator_size, alignment);
	assert(front_of_chunk_ != NULL && "Null Memory Allocation in Allocator Creation");

	total_bytes_left = total_allocator_size;

	back_of_chunk_ = reinterpret_cast<uint8_t*>(front_of_chunk_) + total_allocator_size;
	InitializeFreeList(num_of_descriptors);
	
}

void * GAllocator::GAlloc(const size_t amt)
{
	return GAlloc(amt, 4);
}

void * GAllocator::GAlloc(const size_t amt, const uint8_t alignment) {
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

	_Descriptor * new_descriptor = StealFromBlock(sufficiently_sized_block,amt,alignment);

	assert(new_descriptor != NULL);
	//TODO: if null, should reverse what happened in steal from block
	//if (new_descriptor == NULL)
	//	return nullptr;

	new_descriptor->user_size = amt;
	AddToAllocatedList(new_descriptor);

	DEBUGLOG("User requested %zu BYTES", amt);

#ifdef _DEBUG
	DEBUGLOG("Allocated:");
	PrintList(allocated_root_);
	DEBUGLOG("Unallocated:");
	PrintList(unallocated_root_);
	DEBUGLOG("Free:");
	PrintList(free_root_);
#endif



	return new_descriptor->user_ptr;
}

bool GAllocator::GFree(const void * addr_to_free)
{
	_Descriptor * to_move_to_unallocated = RemoveBlockFromList(addr_to_free, allocated_root_);
	assert(to_move_to_unallocated != NULL && "attempted to free a non valid address");

	if (to_move_to_unallocated == NULL) {
		return false;
	}

	bool band_integrity_check = CheckGuardBands(to_move_to_unallocated);
	assert(band_integrity_check && "Guardband corruption");

	AddToUnallocatedList(to_move_to_unallocated);

	DEBUGLOG("User freed %04X addr", addr_to_free);

#ifdef _DEBUG
	DEBUGLOG("Allocated:");
	PrintList(allocated_root_);
	DEBUGLOG("Unallocated:");
	PrintList(unallocated_root_);
	DEBUGLOG("Free:");
	PrintList(free_root_);
#endif
	return true;
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
		uint8_t* addr_to_search_for = static_cast<uint8_t*>(conductor->base) + conductor->master_size;
		_Descriptor * found_block = SearchForBlock(addr_to_search_for, unallocated_root_);

		if (found_block == NULL) {
			conductor = conductor->next;
		}
		else {
			CombineBlocks(found_block, conductor);
		}
	}


	DEBUGLOG("GARBAGE COLLECTED");

#ifdef _DEBUG
	DEBUGLOG("Allocated:");
	PrintList(allocated_root_);
	DEBUGLOG("Unallocated:");
	PrintList(unallocated_root_);
	DEBUGLOG("Free:");
	PrintList(free_root_);
#endif
}

GAllocator * GAllocator::GetInstance()
{
	return nullptr;
}

void GAllocator::DestroyInstance()
{
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

	front_of_pool_ = reinterpret_cast<_Descriptor*>(reinterpret_cast<uint8_t*>(back_of_chunk_) - (sizeof(_Descriptor) * num_of_descriptors));

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

		assert(new_descriptor < reinterpret_cast<_Descriptor*>(back_of_chunk_));

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

	total_bytes_left -= sizeof(_Descriptor) * num_of_descriptors;

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

#ifdef _DEBUG
	DEBUGLOG("Allocated:");
	PrintList(allocated_root_);
	DEBUGLOG("Unallocated:");
	PrintList(unallocated_root_);
	DEBUGLOG("Free:");
	PrintList(free_root_);
#endif // _DEBUG
	
}

void GAllocator::AddToAllocatedList(_Descriptor * node_to_insert)
{
	_Descriptor * conductor = allocated_root_;

	//we are the first
	if (allocated_root_ == 0) {
		node_to_insert->prev = NULL;
		node_to_insert->next = NULL;
		allocated_root_ = node_to_insert;
	}
	else {
		while (conductor->next != NULL) {
			conductor = conductor->next;
		}
		conductor->next = node_to_insert;
		node_to_insert->prev = conductor;
	}

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

void GAllocator::AddToFreeList(_Descriptor * node_to_insert)
{
	//nullify descriptor
	node_to_insert->base = NULL;
	node_to_insert->master_size = NULL;
	node_to_insert->user_size = NULL;
	node_to_insert->user_ptr = NULL;
	node_to_insert->next = NULL;

	//set at end
	tail_of_free_->next = node_to_insert;
	node_to_insert->prev = tail_of_free_->prev;
	tail_of_free_ = node_to_insert;
	

}

bool GAllocator::CheckGuardBands(_Descriptor * node_to_check)
{
	uint8_t* front_band_addr = static_cast<uint8_t*>(node_to_check->base);
	for (size_t i = 0; i < BAND_SIZE; i++) {
		if (*(front_band_addr + i) != BAND_VAL) {
			return false;
		}
	}

	uint8_t * back_band_addr = static_cast<uint8_t*>(node_to_check->user_ptr) + node_to_check->user_size;
	for (size_t i = 0; i < BAND_SIZE; i++) {
		if (*(front_band_addr + i) != BAND_VAL) {
			return false;
		}
	}
	return true;
}

void GAllocator::CombineBlocks(_Descriptor * first, _Descriptor * second)
{
	size_t size_of_combined_blocks = first->master_size + second->master_size;

	second->master_size = size_of_combined_blocks;

	RemoveBlockFromList(first->user_ptr, unallocated_root_);
	AddToFreeList(first);
}

void GAllocator::NullRootReference(_Descriptor * node_to_null)
{
	//what type of root are we? Set the appropriate reference
	if (node_to_null == free_root_) {
		free_root_ = NULL;
	}
	else if (node_to_null == allocated_root_) {
		allocated_root_ = NULL;
	}
	else if (node_to_null == unallocated_root_) {
		unallocated_root_ = NULL;
	}
}

void GAllocator::MoveRootReferencesForward(_Descriptor * node_to_move_forward)
{
	//what type of root are we? Set the appropriate reference
	if (node_to_move_forward == free_root_) {
		free_root_ = node_to_move_forward->next;
	}
	else if (node_to_move_forward == allocated_root_) {
		allocated_root_ = node_to_move_forward->next;
	}
	else if (node_to_move_forward == unallocated_root_) {
		unallocated_root_ = node_to_move_forward->next;
	}
}

_Descriptor * GAllocator::SearchForBlock(const void * addr_to_search_for, _Descriptor * root_node) const
{
	_Descriptor * conductor = root_node;

	while (conductor != NULL) {
		if (conductor->base == addr_to_search_for) {
			return conductor;
		}
		else {
			conductor = conductor->next;
		}
	}

	return nullptr;
}

_Descriptor * GAllocator::FindSuitableUnallocatedBlock(const size_t amt, uint8_t alignment) const
{
	_Descriptor * conductor = unallocated_root_;

	size_t toLookFor = amt;/*+ alignment;*/
//
//#ifdef _DEBUG
//	toLookFor += BAND_SIZE * 2;
//#endif
//
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

_Descriptor * GAllocator::RemoveBlockFromList(const void * addr_to_search_for, _Descriptor * root_node)
{
	_Descriptor * conductor = root_node;

	while (conductor != NULL) {

		//found it
		if (conductor->user_ptr == addr_to_search_for) {

			//head case
			if (conductor->prev == NULL && conductor->next != NULL) {

				//move root references forward to next
				MoveRootReferencesForward(conductor);  
				//modify pointers
				conductor->next->prev = NULL;
				conductor->next = NULL;
			}
			//middle case
			else if (conductor->prev != NULL && conductor->next != NULL) {
				conductor->next->prev = conductor->prev;
				conductor->prev->next = conductor->next;

				conductor->prev = NULL;
				conductor->next = NULL;
			}

			//tail case
			else if (conductor->prev != NULL && conductor->next == NULL) {
			
				conductor->prev->next = NULL;
				conductor->prev = NULL;

				//set tail reference if appropriate
				if (conductor == tail_of_free_) {
					tail_of_free_ = NULL;
				}
			}

			//root case
			else if (conductor->prev == NULL && conductor->next == NULL) {
				//nullify the appropriate root
				NullRootReference(conductor);
			}

			return conductor;
		}

		else {
			//move on
			conductor = conductor->next;
		}
	}
	return nullptr;
}

_Descriptor * GAllocator::StealFromBlock(_Descriptor * victim, const size_t amt_to_take, const uint8_t alignment)
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

	//guardbands
#ifdef _DEBUG
	uint8_t * front_guardband_pos = static_cast<uint8_t*>(thief->base);
	for (size_t i = 0; i < BAND_SIZE; i++) {
		*(front_guardband_pos + i) = BAND_VAL;
	}
	//reassign user ptr to be in front of the front guardbands
	thief->user_ptr = static_cast<uint8_t*>(thief->base) + BAND_SIZE;	

	uint8_t* back_guardband_pos = static_cast<uint8_t*>(thief->user_ptr) + amt_to_take;
	for (size_t i = 0; i < BAND_SIZE; i++) {
		*(back_guardband_pos + i) = BAND_VAL;
	}

#endif
	
	thief->master_size = entire_amt_to_take;
	victim->master_size -= entire_amt_to_take;
	
	victim->base = static_cast<uint8_t*>(victim->base) + entire_amt_to_take;

	return thief;
}

size_t GAllocator::GetAlignmentOffset(const void * addr, uint8_t alignment)
{
	return size_t();
}

bool GAllocator::IsPowerOfTwo(uint8_t input)
{
	uint8_t val = (input != 0) && !(input & (input - 1));

	if (val == 1)
		return true;
	else
		return false;
}

void GAllocator::CreateInstance(const size_t total_allocator_size, const unsigned int num_of_descriptors, const uint8_t alignment)
{
}

bool GAllocator::ContainsAddress(const void* addr_to_find) {

	_Descriptor * conductor = allocated_root_;

	while (conductor != NULL) {
		if (conductor->user_ptr == addr_to_find) {
			return true;
		}
		else {
			conductor = conductor->next;
		}
	}

	while (conductor != NULL) {
		if (conductor->user_ptr == addr_to_find) {
			return true;
		}
		else {
			conductor = conductor->next;
		}
	}

	return false;
}

bool GAllocator::IsAllocatedAddress(const void* addr_to_find) {
	_Descriptor * conductor = allocated_root_;

	while (conductor != NULL) {
		if (conductor->user_ptr == addr_to_find) {
			return true;
		}
		else {
			conductor = conductor->next;
		}
	}

	return false;
}

size_t GAllocator::GetLargestFreeBlockSize() {
	size_t largest = 0;
	
	_Descriptor * conductor = unallocated_root_;


	while (conductor != NULL) {
		if (conductor->master_size > largest) {
			largest = conductor->master_size;
		}
		else {
			conductor = conductor->next;
		}
	}


	return largest;
}