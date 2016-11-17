#include "MonsterAllocator.h"

#include <new>
MonsterAllocator* MonsterAllocator::single = NULL;
void* MonsterAllocator::singleton_addr = NULL;

MonsterAllocator::MonsterAllocator(size_t size_of_chunk, const unsigned int num_of_descriptors, uint8_t initial_alignment)
{
	//TODO: Make sure initial alignment is a power of 2
	front_of_chunk_ = _aligned_malloc(size_of_chunk, initial_alignment);
	assert(front_of_chunk_ != NULL && "NULL Memory Allocation");


	back_of_chunk_ = reinterpret_cast<char*>(front_of_chunk_) + size_of_chunk;

	InitializeFreeList(num_of_descriptors);

	DEBUG_LIST_DISPLAY;
}

MonsterAllocator::~MonsterAllocator()
{
	_aligned_free(front_of_chunk_);
	//_aligned_free(singleton_addr);
}



void * MonsterAllocator::MonsterMalloc(size_t i_amt) {


	//no more free block descriptors, attempt to get some by garbage collecting.
	if (endf_of_free_ == NULL) {
		GarbageCollect();
	}



	BlockDescriptor * sufficientBlock = FindSuitableUnallocBlock(i_amt,4);

	//if even after garbage collecting we don't have a descriptor.
	if (sufficientBlock == NULL)
		return nullptr;

	BlockDescriptor * newBD = StealFromBlock(sufficientBlock,i_amt,4);
	newBD->user_size = i_amt;
	if (newBD == NULL)
		return nullptr;
	AddToAllocated(newBD);

	DEBUGLOG("USER REQUEST %zu BYTES", i_amt);
	DEBUG_LIST_DISPLAY;
	

	return newBD->user_ptr;
}

void * MonsterAllocator::MonsterMalloc(size_t amt, uint8_t align)
{
	//no more free block descriptors, attempt to get some by garbage collecting.
	if (endf_of_free_ == NULL) {
		GarbageCollect();
	}



	BlockDescriptor * sufficientBlock = FindSuitableUnallocBlock(amt,align);

	//if even after garbage collecting we don't have a descriptor.
	if (sufficientBlock == NULL)
		return nullptr;

	BlockDescriptor * newBD = StealFromBlock(sufficientBlock, amt,align);
	newBD->user_size = amt;
	if (newBD == NULL)
		return nullptr;
	AddToAllocated(newBD);

	DEBUGLOG("USER REQUEST %zu BYTES", amt);
	DEBUG_LIST_DISPLAY;


	return newBD->user_ptr;
}

void MonsterAllocator::InitializeFreeList(const int num_of_descriptors)
{
	front_of_bd_ = reinterpret_cast<BlockDescriptor*>(reinterpret_cast<char*>(back_of_chunk_) - (sizeof(BlockDescriptor) * num_of_descriptors));

	assert(front_of_bd_ > front_of_chunk_ && "Pool of block descriptors larger than the entire chunk, too many descriptors");

	front_of_bd_->prev = NULL;
	front_of_bd_->block_base = NULL;
	front_of_bd_->whole_block_size = 0;

	BlockDescriptor * current;
	current = front_of_bd_;


#ifdef _DEBUG
	current->id = 0;
#endif

	for ( int i = 0; i < num_of_descriptors - 1; i++) {
		BlockDescriptor * newBD = current + 1;
		if (newBD >= (BlockDescriptor*)back_of_chunk_)
			break;
		current->next = newBD;
		newBD->prev = current;
		newBD->block_base = NULL;
		newBD->whole_block_size = 0;
		newBD->next = NULL;

#ifdef _DEBUG
		newBD->id = i + 1;
#endif

		current = newBD;
	}

	free_root_ = front_of_bd_;
	endf_of_free_ = current;

	total_bytes_ = reinterpret_cast<char*>(front_of_bd_) - reinterpret_cast<char*>(front_of_chunk_);

	//set up initial unallocated block
	BlockDescriptor * initialUnallocatedBlock;
	initialUnallocatedBlock = endf_of_free_; //use the end of free
	endf_of_free_ = endf_of_free_->prev;
	endf_of_free_->next = NULL; //update end of free

	initialUnallocatedBlock->prev = NULL;
	initialUnallocatedBlock->block_base = front_of_chunk_;
	initialUnallocatedBlock->whole_block_size = total_bytes_;
	initialUnallocatedBlock->next = NULL;

	AddToUnallocated(initialUnallocatedBlock);

}

void MonsterAllocator::AddToAllocated(BlockDescriptor * to_insert)
{
	BlockDescriptor* conductor;
	conductor = allocated_root_;

	if (allocated_root_ == 0) {
		to_insert->prev = NULL;
		to_insert->next = NULL;
		allocated_root_ = to_insert;
		return;
	}
	else {
		while (conductor->next != NULL) {
			conductor = conductor->next;
		}
		conductor->next = to_insert;
		to_insert->prev = conductor;
	}

}

void MonsterAllocator::AddToUnallocated(BlockDescriptor * to_insert)
{
	BlockDescriptor* conductor;
	conductor = unallocated_root_;

	if (unallocated_root_ == 0) {
		unallocated_root_ = to_insert;
		unallocated_root_->prev = NULL;
		unallocated_root_->next = NULL;
		return;
	}
	else {
		while (conductor->next != NULL) {
			conductor = conductor->next;
		}

		conductor->next = to_insert;
		to_insert->prev = conductor;
	}
}

void MonsterAllocator::GarbageCollect()
{
	//looks through the list of free memory blocks
	//combing any that can be combined into a single larger free block.
	BlockDescriptor * placeHolder;
	placeHolder = unallocated_root_;

	bool finished = false;

	if (unallocated_root_ == 0)
		return;

	while (!finished) {

		char * addrToSearchFor;
 		addrToSearchFor = static_cast<char*>(placeHolder->block_base) + placeHolder->whole_block_size;
		BlockDescriptor * foundBlock = UnallocBlockSearch(addrToSearchFor,unallocated_root_);

		if (foundBlock == NULL) {
			placeHolder = placeHolder->next;
		}
		else {
			//we found a block right after, combine them.
			ConsolidateBlocks(foundBlock, placeHolder);
		}
		
		if (placeHolder == NULL )
			finished = true;
	}


}


BlockDescriptor * MonsterAllocator::UnallocBlockSearch(const void * base_addr, BlockDescriptor * start) const
{
	BlockDescriptor * conductor;
	conductor = start;

	while (conductor!= NULL) {
		if (conductor->block_base == base_addr)
			return conductor;
		else
			conductor = conductor->next;
	}
	
	return nullptr;
}

BlockDescriptor * MonsterAllocator::FindSuitableUnallocBlock(size_t amt, uint8_t align) const
{
	BlockDescriptor * conductor;
	conductor = unallocated_root_;
	amt += align;
#ifdef _DEBUG
	amt += GUARDBAND_BYTES * 2; //need to account for padding and guardbands
#endif // _DEBUG


	while (conductor != NULL) {
		if (conductor->whole_block_size >= amt) {
			return conductor;
		}
		conductor = conductor->next;
	}

	return nullptr;
}

void MonsterAllocator::ConsolidateBlocks(BlockDescriptor * first, BlockDescriptor * second)
{
	DEBUGLOG("Combining blocks %d and %d", first->id, second->id);
	size_t newSize = first->whole_block_size + second->whole_block_size;
	
	second->whole_block_size = newSize;
	RemoveFromList(first->user_ptr, unallocated_root_);
	AddToFree(first);

	DEBUG_LIST_DISPLAY;


}


void MonsterAllocator::AddToFree(BlockDescriptor * to_add)
{
	to_add->block_base = NULL;
	to_add->whole_block_size = NULL;
	to_add->user_ptr = NULL;
	endf_of_free_->next = to_add;
	to_add->prev = endf_of_free_;

	endf_of_free_ = to_add;
	endf_of_free_->next = NULL;
}

BlockDescriptor * MonsterAllocator::RemoveFromList(const void * addr, BlockDescriptor * root)
{
	BlockDescriptor * conductor;
	conductor = root;

	while (conductor != NULL) {
		if (conductor->user_ptr == addr) //found the block
		{
			//head case with something after it
			if (conductor->prev == NULL && conductor->next != NULL) {
				if (conductor == free_root_) {
					free_root_ = conductor->next;
				}
				else if (conductor == allocated_root_) {
					allocated_root_ = conductor->next;
				}
				else if (conductor == unallocated_root_) {
					unallocated_root_ = conductor->next;
				}

				conductor->next->prev = NULL;
				conductor->next = NULL;


			}
			//root case
			else if (conductor->prev == NULL && conductor->next == NULL) {
				if (conductor == free_root_) {
					free_root_ = NULL;
				}
				else if (conductor == allocated_root_) {
					allocated_root_ = NULL;
				}
				else if (conductor == unallocated_root_) {
					unallocated_root_ = NULL;
				}
			}
			//middle
			else if (conductor->prev != NULL && conductor->next != NULL) {
				conductor->next->prev = conductor->prev;
				conductor->prev->next = conductor->next;
				conductor->next = NULL;
				conductor->prev = NULL;
			} 
			//tail
			else if (conductor->prev != NULL && conductor->next == NULL) {
				conductor->prev->next = NULL;
				conductor->prev = NULL;

				if (conductor == endf_of_free_) {
					endf_of_free_ = NULL;
				}
			}

			return conductor;

		}//if

		else {
			//move on
			conductor = conductor->next;
		}
	}//while

	return nullptr;
}

BlockDescriptor * MonsterAllocator::StealFromBlock(BlockDescriptor * victim, size_t amt,uint8_t align)
{
	
	//get a bd from the free list
	BlockDescriptor * thief = endf_of_free_;
	if (thief->prev != NULL) {
		endf_of_free_ = thief->prev;
	}
	endf_of_free_->next = NULL;
	thief->prev = NULL;
	thief->next = NULL; 

	//check for alignmentcxv
	size_t pad = GetAlignmentOffset(victim->block_base,align);

	size_t wholeAmountStolen = amt + pad;

#ifdef _DEBUG
	wholeAmountStolen = amt + GUARDBAND_BYTES * 2 + pad;
#endif

	thief->block_base = reinterpret_cast<char*>(victim->block_base); //we're going to take the front of the victim space
	thief->offset_padding = pad;
	thief->user_ptr = reinterpret_cast<char*>(thief->block_base) + pad; //assign user's pointer
#ifdef _DEBUG					
	unsigned char * frontguardbandPos = static_cast<unsigned char*>(thief->block_base) + pad;
	for (int i = 0; i < GUARDBAND_BYTES; i++) {
		*(frontguardbandPos + i) = GUARDBAND_VAL;

	}

	thief->user_ptr = reinterpret_cast<char*>(thief->block_base) + GUARDBAND_BYTES + pad;
#endif	

#ifdef _DEBUG
	unsigned char * backguardbandPos = reinterpret_cast<unsigned char*>(thief->user_ptr) + amt; //put back guardband

	for (int i = 0; i < GUARDBAND_BYTES; i++) {
		*(backguardbandPos + i) = GUARDBAND_VAL;

	}

#endif // _DEBUG


	thief->whole_block_size = wholeAmountStolen;
	victim->whole_block_size -= wholeAmountStolen;	

	char * modifiedBlockBase2 = reinterpret_cast<char*>(victim->block_base) + wholeAmountStolen;
	victim->block_base = reinterpret_cast<void*>(modifiedBlockBase2);
	return thief;
}


void MonsterAllocator::PrintLists() const
{
	//print free lists
	DEBUGLOG("=========PRINT START");

	BlockDescriptor * conductor;
	conductor = free_root_;


	while (conductor != NULL) {
		DEBUGLOG("FREE NODE [addr:0x%04x id:%d]: prev:0x%04x\tblockptr:%04x\tUserptr:%04x\tsize:%zu\tnext:0x%04x", conductor, conductor->id, conductor->prev, conductor->block_base, conductor->user_ptr, conductor->whole_block_size, conductor->next);
		conductor = conductor->next;
	}

	conductor = allocated_root_;

	while (conductor != NULL) {
DEBUGLOG("ALLOC NODE [addr:0x%04x id:%d]: prev:0x%04x\tblockptr:%04x\tUserptr:%04x\tsize:%zu\tnext:0x%04x", conductor, conductor->id, conductor->prev, conductor->block_base, conductor->user_ptr, conductor->whole_block_size, conductor->next);
conductor = conductor->next;
	}

	conductor = unallocated_root_;

	while (conductor != NULL) {
		DEBUGLOG("UNALLOC NODE [addr:0x%04x id:%d]: prev:0x%04x\tblockptr:%04x\tUserptr:%04x\tsize:%zu\tnext:0x%04x", conductor, conductor->id, conductor->prev, conductor->block_base,conductor->user_ptr, conductor->whole_block_size, conductor->next);
		conductor = conductor->next;
	}

	DEBUGLOG("=========PRINT END");

}

void MonsterAllocator::PrintAllocatedList()  const {
	//print allocated lists
	DEBUGLOG("=========PRINT ALLOCATED START");

	BlockDescriptor * conductor;
	conductor = allocated_root_;

	while (conductor != NULL) {
		DEBUGLOG("ALLOC NODE [addr:0x%04x id:%d]: prev:0x%04x\tblockptr:%04x\tUserptr:%04x\tsize:%zu\tnext:0x%04x", conductor, conductor->id, conductor->prev, conductor->block_base, conductor->user_ptr, conductor->whole_block_size, conductor->next);
		conductor = conductor->next;
	}

}

void MonsterAllocator::PrintFreeList()  const
{
	//print free lists
	DEBUGLOG("=========PRINT FREE START");

	BlockDescriptor * conductor;
	conductor = free_root_;

	while (conductor != NULL) {
		DEBUGLOG("FREE NODE [addr:0x%04x id:%d]: prev:0x%04x\tblockptr:%04x\tUserptr:%04x\tsize:%zu\tnext:0x%04x", conductor, conductor->id, conductor->prev, conductor->block_base, conductor->user_ptr, conductor->whole_block_size, conductor->next);
		conductor = conductor->next;
	}

}


void MonsterAllocator::PrintUnallocatedList() const
{
	//print free lists
	DEBUGLOG("=========PRINT UNALLOCATED START");

	BlockDescriptor * conductor;
	conductor = unallocated_root_;

	while (conductor != NULL) {
		DEBUGLOG("UNALLOCATED NODE [addr:0x%04x id:%d]: prev:0x%04x\tblockptr:%04x\tUserptr:%04x\tsize:%zu\tnext:0x%04x", conductor, conductor->id, conductor->prev, conductor->block_base, conductor->user_ptr, conductor->whole_block_size, conductor->next);
		conductor = conductor->next;
	}

}


bool MonsterAllocator::MonsterFree(void * addr)
{

	BlockDescriptor * toMoveToUnallocated = RemoveFromList(addr, allocated_root_);

	assert(toMoveToUnallocated != NULL && "Attempted to free a non valid addr");

	bool guardBandIntegrity = GuardBandChecks(toMoveToUnallocated);

	assert(guardBandIntegrity && "GUARDBANDS CORRUPTED");

	
	//if they free things that don't exist in the allocated list
	if (toMoveToUnallocated == NULL) {
		return false;
	}
	AddToUnallocated(toMoveToUnallocated);
	DEBUGLOG("==USER FREE: %04X BEFORE GARBAGE COLLECT==", addr);

	DEBUG_LIST_DISPLAY;

	GarbageCollect();
	DEBUGLOG("==AFTER GARBAGE COLLECT==", addr);

	DEBUG_LIST_DISPLAY;

	return true;
}

bool MonsterAllocator::GuardBandChecks(BlockDescriptor * to_check)
{
	uint8_t* frontGuardBandAddr = reinterpret_cast<uint8_t*>(to_check->block_base) + to_check->offset_padding;
	for (int i = 0; i < GUARDBAND_BYTES; i++) {
		if (*(frontGuardBandAddr + i) != GUARDBAND_VAL){
			return false;
		}
	}

	
	uint8_t * backGuardBandAddr = static_cast<uint8_t*>(to_check->user_ptr) + to_check->user_size;
	for (int i = 0; i < GUARDBAND_BYTES; i++) {
		if (*(backGuardBandAddr + i) != GUARDBAND_VAL) {
			return false;
		}
	}

	return true;
}







bool MonsterAllocator::Contains(const void * addr) const
{

	//look through allocated first

	BlockDescriptor * conductor;
	conductor = allocated_root_;

	while (conductor != NULL) {
		if (conductor->user_ptr == addr)
			return true;
		else
			conductor = conductor->next;
	}

	conductor = unallocated_root_;

	while (conductor != NULL) {
		if (conductor->user_ptr == addr)
			return true;
		else
			conductor = conductor->next;
	}
	return false;
}

bool MonsterAllocator::isAllocated(const void * addr) const
{
	BlockDescriptor * conductor;
	conductor = allocated_root_;

	while (conductor != NULL) {
		if (conductor->user_ptr == addr)
			return true;
		else
			conductor = conductor->next;
	}
	return false;
}


size_t MonsterAllocator::GetLargestFreeBlock() const
{
	size_t largest = 0;

	BlockDescriptor * conductor;
	conductor = unallocated_root_;

	while (conductor != NULL) {
		if (conductor->whole_block_size > largest)
			largest = conductor->whole_block_size;
		else
			conductor = conductor->next;
	}
	if (largest == 0)
		return 0;
#ifdef _DEBUG
	largest -= GUARDBAND_BYTES * 2;
#endif
	return largest;
}

void MonsterAllocator::CreateInstance(size_t total_size_of_heap, const unsigned int num_of_descriptors, uint8_t align) {
	MonsterAllocator::singleton_addr = _aligned_malloc(sizeof(MonsterAllocator), 4);
	single = new (singleton_addr) MonsterAllocator(total_size_of_heap, num_of_descriptors,align);
}

MonsterAllocator * MonsterAllocator::getInstance()
{
	if(!single){
		CreateInstance(TOTAL_SIZE,NUM_DESCRIPTORS,ALIGNMENT);
		return single;
	}
	else
		return single;
}

//create destroy and a gate

void MonsterAllocator::DestroyInstance() {
	single->~MonsterAllocator();

	_aligned_free(single);
	single = NULL;
}