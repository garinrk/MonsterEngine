#include "GAllocator.h"


GAllocator* GAllocator::singleton_instance_ = NULL;
void* GAllocator::singleton_instance_addr_ = NULL;

GAllocator::GAllocator(const size_t i_totalSizeOfAllocator, const unsigned int i_amtOfDescriptors, const uint8_t i_align)
{
	front_of_chunk_ = _aligned_malloc(i_totalSizeOfAllocator, i_align);
	assert(front_of_chunk_ != NULL && "Null Memory Allocation in Allocator Creation");

	total_bytes_left = i_totalSizeOfAllocator;

	back_of_chunk_ = reinterpret_cast<uint8_t*>(front_of_chunk_) + i_totalSizeOfAllocator;
	InitializeFreeList(i_amtOfDescriptors);
	DEBUGLOG("GALLOCATOR INIT");
	PRINT_GALLOC_STATE;
	
}

void * GAllocator::GAlloc(const size_t i_amt)
{
	return GAlloc(i_amt, 4);
}

void * GAllocator::GAlloc(const size_t i_amt, const uint8_t i_align) {
	assert(IsPowerOfTwo(i_align));

	if (!IsPowerOfTwo(i_align))
		return nullptr;

	//TODO: What if there is a sufficiently sized unallocated node?
	//check to see if we need to garbage collect some nodes
	if (free_root_ == NULL) {
		GGCollect();
	}

	if (free_root_ == NULL)
		return nullptr;
	
	_Descriptor * sufficiently_sized_block = FindSuitableUnallocatedBlock(i_amt);

	//even after garbage collecting we don't have a descriptor
	if (sufficiently_sized_block == NULL)
		return nullptr;
	
	_Descriptor * new_descriptor = StealFromBlock(sufficiently_sized_block,i_amt,i_align);

	//TODO: If you don't fragment and just hand back a suitable block, this user_size may be more
	new_descriptor->user_size = i_amt;
	AddToAllocatedList(new_descriptor);
	DEBUGLOG("User requested %zu BYTES", i_amt);
	PRINT_GALLOC_STATE;



	return new_descriptor->user_ptr;
}

bool GAllocator::GFree(const void * i_addr)
{
	_Descriptor * to_move_to_unallocated = RemoveBlockFromList(i_addr, allocated_root_);
	assert(to_move_to_unallocated != NULL && "attempted to free a non valid address");

	if (to_move_to_unallocated == NULL) {
		return false;
	}

	bool band_integrity_check = CheckGuardBands(to_move_to_unallocated);
	assert(band_integrity_check && "Guardband corruption");

	AddToUnallocatedList(to_move_to_unallocated);

	DEBUGLOG("User freed %04X addr", i_addr);
	PRINT_GALLOC_STATE;
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
			DEBUGLOG("Combining blocks %d and %d\n", conductor->debug_id, found_block->debug_id);
			CombineBlocks(found_block, conductor);
		}
	}
	DEBUGLOG("AFTER GARBAGE COLLECTION");
	PRINT_GALLOC_STATE;
}

void GAllocator::CreateInstance(const size_t i_totalSizeOfAllocator, const unsigned int i_amtOfDescriptors, const uint8_t i_align)
{
	GAllocator::singleton_instance_addr_ = _aligned_malloc(sizeof(GAllocator), 4);
	singleton_instance_ = new (singleton_instance_addr_) GAllocator(i_totalSizeOfAllocator, i_amtOfDescriptors, i_align);
}

void GAllocator::CreateInstance()
{
	GAllocator::singleton_instance_addr_ = _aligned_malloc(sizeof(GAllocator), 4);
	singleton_instance_ = new (singleton_instance_addr_) GAllocator(DEFAULT_TOTAL_SIZE, DEFAULT_NUM_DESCRIPTORS, DEFAULT_ALIGNMENT);
}

GAllocator * GAllocator::GetInstance()
{
	if (!singleton_instance_) {
		CreateInstance(DEFAULT_TOTAL_SIZE, DEFAULT_NUM_DESCRIPTORS, DEFAULT_ALIGNMENT);
		return singleton_instance_;
	}
	else {
		return singleton_instance_;
	}
}

void GAllocator::DestroyInstance()
{
	singleton_instance_->~GAllocator();

	_aligned_free(singleton_instance_);
	singleton_instance_ = NULL;
}

GAllocator::~GAllocator()
{
	if (allocated_root_ != NULL) {
		DEBUGLOG("OUTSANDING ALLOCATIONS DETECTED IN BLOCK ALLOCATOR\n");
	}
	PRINT_GALLOC_STATE;
	_aligned_free(front_of_chunk_);
}

void GAllocator::PrintList(_Descriptor * i_root)
{

	_Descriptor * conductor = i_root;

	while (conductor != NULL) {
		DEBUGLOG("[addr:0x%04x id:%d]: prev:0x%04x\tblockptr:%04x\tUserptr:%04x\tsize:%zu\tnext:0x%04x", conductor, conductor->debug_id, conductor->prev, conductor->base, conductor->user_ptr, conductor->master_size, conductor->next);
		conductor = conductor->next;
	}
}

////////////////
////PRIVATES
////////////////

void GAllocator::InitializeFreeList(const unsigned int i_amtOfDescriptors)
{

	front_of_pool_ = reinterpret_cast<_Descriptor*>(reinterpret_cast<uint8_t*>(back_of_chunk_) - (sizeof(_Descriptor) * i_amtOfDescriptors));

	//make sure we didn't create too many descriptors
	assert(front_of_pool_ > front_of_chunk_ && "Too many descriptors, pool is larger than the total size");

	//set up front of pool
	front_of_pool_->prev = NULL;
	front_of_pool_->base = NULL;
	front_of_pool_->user_ptr = NULL;
	front_of_pool_->master_size = 0;
	front_of_pool_->user_size = 0;

	_Descriptor * conductor = front_of_pool_;

#ifdef _DEBUG
	conductor->debug_id = 0;
#endif
	
	for (size_t i = 0; i < i_amtOfDescriptors - 1; i++) {
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

	total_bytes_left -= sizeof(_Descriptor) * i_amtOfDescriptors;

	//initial unallocated block

	_Descriptor * init_unalloc_block;

	//steals the last free block to set itself up
	init_unalloc_block = free_root_;
	free_root_ = free_root_->next;

	//set other block properties
	init_unalloc_block->prev = NULL;
	init_unalloc_block->base = front_of_chunk_;
	init_unalloc_block->master_size = total_bytes_left;
	init_unalloc_block->next = NULL;
	init_unalloc_block->user_ptr = NULL;
	init_unalloc_block->user_size = 0;

	AddToUnallocatedList(init_unalloc_block);
}

void GAllocator::AddToAllocatedList(_Descriptor * i_node)
{
	_Descriptor * conductor = allocated_root_;

	//we are the first
	if (allocated_root_ == 0) {
		i_node->prev = NULL;
		i_node->next = NULL;
		allocated_root_ = i_node;
	}
	else {
		while (conductor->next != NULL) {
			conductor = conductor->next;
		}
		conductor->next = i_node;
		i_node->prev = conductor;
	}

}


void GAllocator::AddToUnallocatedList(_Descriptor * i_node)
{
	_Descriptor * conductor = unallocated_root_;

	if (unallocated_root_ == 0) {
		unallocated_root_ = i_node;
		unallocated_root_->prev = NULL;
		unallocated_root_->next = NULL;
	}
	else {
		while (conductor->next != NULL) {
			conductor = conductor->next;
		}

		conductor->next = i_node;
		i_node->prev = conductor;
	}
}

void GAllocator::AddToFreeList(_Descriptor * i_node)
{
	//nullify descriptor
	i_node->base = NULL;
	i_node->master_size = NULL;
	i_node->user_size = NULL;
	i_node->user_ptr = NULL;
	i_node->next = NULL;
	
	if (free_root_ == NULL) {
		free_root_ = i_node;
		free_root_->prev = NULL;
	}
	else {
		i_node->next = free_root_;
		free_root_->prev = i_node;
		free_root_ = i_node;
	}
}

bool GAllocator::CheckGuardBands(_Descriptor * i_node)
{
	uint8_t* front_band_addr = static_cast<uint8_t*>(i_node->user_ptr) - BAND_SIZE;
	for (size_t i = 0; i < BAND_SIZE; i++) {
		if (*(front_band_addr + i) != BAND_VAL) {
			return false;
		}
	}

	uint8_t * back_band_addr = static_cast<uint8_t*>(i_node->user_ptr) + i_node->user_size;
	for (size_t i = 0; i < BAND_SIZE; i++) {
		if (*(front_band_addr + i) != BAND_VAL) {
			return false;
		}
	}
	return true;
}

void GAllocator::CombineBlocks(_Descriptor * i_firstBlock, _Descriptor * i_secondBlock)
{
	size_t size_of_combined_blocks = i_firstBlock->master_size + i_secondBlock->master_size;

	i_secondBlock->master_size = size_of_combined_blocks;

	RemoveBlockFromList(i_firstBlock->user_ptr, unallocated_root_);
	AddToFreeList(i_firstBlock);
}

void GAllocator::NullRootReference(_Descriptor * i_node)
{
	//what type of root are we? Set the appropriate reference
	if (i_node == free_root_) {
		free_root_ = NULL;
	}
	else if (i_node == allocated_root_) {
		allocated_root_ = NULL;
	}
	else if (i_node == unallocated_root_) {
		unallocated_root_ = NULL;
	}
}

void GAllocator::MoveRootReferencesForward(_Descriptor * i_node)
{
	//what type of root are we? Set the appropriate reference
	if (i_node == free_root_) {
		free_root_ = i_node->next;
	}
	else if (i_node == allocated_root_) {
		allocated_root_ = i_node->next;
	}
	else if (i_node == unallocated_root_) {
		unallocated_root_ = i_node->next;
	}
}

_Descriptor * GAllocator::SearchForBlock(const void * i_addr, _Descriptor * i_root) const
{
	_Descriptor * conductor = i_root;

	while (conductor != NULL) {
		if (conductor->base == i_addr) {
			return conductor;
		}
		else {
			conductor = conductor->next;
		}
	}

	return nullptr;
}

_Descriptor * GAllocator::FindSuitableUnallocatedBlock(const size_t i_amt) const
{
	_Descriptor * conductor = unallocated_root_;

	size_t toLookFor = i_amt;

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

_Descriptor * GAllocator::RemoveBlockFromList(const void * i_addr, _Descriptor * i_root)
{
	_Descriptor * conductor = i_root;

	while (conductor != NULL) {

		//found it
		if (conductor->user_ptr == i_addr) {

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

_Descriptor * GAllocator::StealFromBlock(_Descriptor * i_victim, const size_t i_amt, const uint8_t i_align)
{

	
	//grab a descriptor from the free list
	_Descriptor * thief = free_root_;
	free_root_ = free_root_->next;



	//thief doesn't point anywhere, zero out references
	thief->prev = NULL;
	thief->next = NULL;
	

	size_t entire_amt_to_take = i_amt;

#ifdef _DEBUG
	entire_amt_to_take += BAND_SIZE * 2;
#endif

	//take the front of the victim's space
	thief->base = i_victim->base;


	//guardbands
#ifdef _DEBUG

	//check to see if "user ptr" address is aligned
	void * addr_to_check = static_cast<uint8_t*>(thief->base) + BAND_SIZE;

	uint8_t * front_guardband_pos;


	if (reinterpret_cast<size_t>(addr_to_check) % i_align == 0) {
		//aligned
		front_guardband_pos = static_cast<uint8_t*>(thief->base);
	}
	else {
		//get padding
		size_t alignment_padding = i_align - (reinterpret_cast<size_t>(addr_to_check) % i_align);
		front_guardband_pos = static_cast<uint8_t*>(addr_to_check) + alignment_padding - BAND_SIZE;
	}

	for (size_t i = 0; i < BAND_SIZE; i++) {
		*(front_guardband_pos + i) = BAND_VAL;
	}

	//reassign user ptr to be in front of the front guardbands
	thief->user_ptr = front_guardband_pos + BAND_SIZE;	

	uint8_t* back_guardband_pos = static_cast<uint8_t*>(thief->user_ptr) + i_amt;


	for (size_t i = 0; i < BAND_SIZE; i++) {
		*(back_guardband_pos + i) = BAND_VAL;
	}
	
	
#endif
	
	entire_amt_to_take = (static_cast<uint8_t*>(thief->user_ptr) + i_amt) - static_cast<uint8_t*>(thief->base);

	//add guardbands for size to modify if in debug
#ifdef _DEBUG
	entire_amt_to_take = (back_guardband_pos + BAND_SIZE) - static_cast<uint8_t*>(thief->base);
#endif

	thief->master_size = entire_amt_to_take;
	i_victim->master_size -= entire_amt_to_take;
	
	i_victim->base = static_cast<uint8_t*>(i_victim->base) + thief->master_size;
	
	//TODO: Fragmentation Minimum. Fall over to handing back
	//a sufficient unallocated block
	if (i_victim->master_size == 0) {
		RemoveBlockFromList(i_victim->user_ptr, unallocated_root_);
		AddToFreeList(i_victim);
	}

	return thief;
}

bool GAllocator::IsPowerOfTwo(const uint8_t i_toCheck)
{
	uint8_t val = (i_toCheck != 0) && !(i_toCheck & (i_toCheck - 1));

	if (val == 1)
		return true;
	else
		return false;
}

bool GAllocator::ContainsAddressInBlock(const void* i_addr) {


	if (i_addr <= back_of_chunk_ && i_addr >= front_of_chunk_){
		return true;
	}
	else {
		return false;
	}

	
}

bool GAllocator::IsAllocatedAddress(const void* i_addr) {
	_Descriptor * conductor = allocated_root_;

	while (conductor != NULL) {
		if (conductor->user_ptr == i_addr) {
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

#ifdef _DEBUG
	if (largest > 0) {
		largest -= BAND_SIZE * 2;
	}
#endif
	return largest;
}

void GAllocator::PrintAllocatorState() {
	DEBUGLOG("===ALLOCATOR STATE===");

	_Descriptor * conductor = free_root_;

	while (conductor != NULL) {
		DEBUGLOG("FREE NODE [addr:0x%04x id:%d]: prev:0x%04x\tblockptr:%04x\tUserptr:%04x\tsize:%zu\tnext:0x%04x", conductor, conductor->debug_id, conductor->prev, conductor->base, conductor->user_ptr, conductor->master_size, conductor->next);
		conductor = conductor->next;
	}

	conductor = allocated_root_;

	while (conductor != NULL) {
		DEBUGLOG("ALLOC NODE [addr:0x%04x id:%d]: prev:0x%04x\tblockptr:%04x\tUserptr:%04x\tsize:%zu\tnext:0x%04x", conductor, conductor->debug_id, conductor->prev, conductor->base, conductor->user_ptr, conductor->master_size, conductor->next);
		conductor = conductor->next;
	}
	
	conductor = unallocated_root_;

	while (conductor != NULL) {
		DEBUGLOG("UNALLOC NODE [addr:0x%04x id:%d]: prev:0x%04x\tblockptr:%04x\tUserptr:%04x\tsize:%zu\tnext:0x%04x", conductor, conductor->debug_id, conductor->prev, conductor->base, conductor->user_ptr, conductor->master_size, conductor->next);
		conductor = conductor->next;
	}
}