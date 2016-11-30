#pragma once
struct _Descriptor {
	_Descriptor * prev;
	void * base;
	void * user_ptr;
	size_t master_size;
	size_t user_size;
	size_t offset_pad;

#if _DEBUG
	int id;
#endif
	_Descriptor * next;
};


class GAllocator
{
public:
	GAllocator();
	~GAllocator();
};

