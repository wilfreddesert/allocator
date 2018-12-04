#include <stdint.h>
#include "linked_list.h"
#include "malloc_freelist.h"
#pragma mark - Definitions -

/**
* Aligning up
* to the nearest power of two
*/
#ifndef align_up
#define align_up(num, align) \
	(((num) + ((align) - 1)) & ~((align) - 1))
#endif

/*
* Container for our free-list.
*/
typedef struct {
	ll_t node;
	size_t size;
	char * block;
} alloc_node_t;

int if_created;

#define ALLOC_HEADER_SZ offsetof(alloc_node_t, block)

// We are enforcing a minimum allocation size of 32B.
#define MIN_ALLOC_SZ ALLOC_HEADER_SZ + 32

#pragma mark - Declarations -

// Declare and initialize linked list
static LIST_INIT(free_list);

/**
* When we free, we can check if any memory blocks
* can be combined into larger blocks.  This will help us fight against
* memory fragmentation in a simple way.
*/
void defrag_free_list(void)
{
	alloc_node_t *b, *lb = NULL, *t;

	list_for_each_entry_safe(b, t, &free_list, node)
	{
		if(lb)
		{
			if((((uintptr_t)&lb->block) + lb->size) == (uintptr_t)b)
			{
				lb->size += sizeof(*b) + b->size;
				list_del(&b->node);
				continue;
			}
		}
		lb = b;
	}
	printf("Defragged!\n");
}

#pragma mark - APIs -

void * fl_malloc(size_t size)
{
	void * ptr = NULL;
	alloc_node_t *blk = NULL;

	if(size > 0)
	{
		//Align the pointer
		size = align_up(size, sizeof(void *));

		// try to find a big enough block to alloc
		list_for_each_entry(blk, &free_list, node)
		{
			if(blk->size >= size)
			{
				ptr = &blk->block;
				break;
			}
		}

		// Something found
		if(ptr)
		{
			// Can we split the block?
			if((blk->size - size) >= MIN_ALLOC_SZ)
			{
				alloc_node_t *new_blk;
				new_blk = (alloc_node_t *)((uintptr_t)(&blk->block) + size);
				new_blk->size = blk->size - size - ALLOC_HEADER_SZ;
				blk->size = size;
				list_add_(&new_blk->node, &blk->node, blk->node.next);
			}

			list_del(&blk->node);
		}
		else{
			printf("Not enough space.\n");
			return;
		}
	} 
	return ptr;
}

void fl_free(void * ptr)
{
	alloc_node_t *blk, *free_blk;

	//Don't free a NULL pointer..
	if(ptr)
	{

		// we take the pointer and use container_of to get the corresponding alloc block
		blk = container_of(ptr, alloc_node_t, block);

		//Let's put it back in the proper spot
		list_for_each_entry(free_blk, &free_list, node)
		{
			if(free_blk > blk)
			{
				list_add_(&blk->node, free_blk->node.prev, &free_blk->node);
				goto blockadded;
			}
		}
		list_add_tail(&blk->node, &free_list);

blockadded:
		// Let's see if we can combine any memory
		defrag_free_list();
	}
}

void malloc_addblock(void *addr, size_t size)
{
	

	if (if_created){
		printf("Block already exists.\n");
		return;
	}
	
	alloc_node_t *blk;

	// let's align the start address of our block to the next pointer aligned number
	blk = (void *) align_up((uintptr_t)addr, sizeof(void*));

	// calculate actual size - remove our alignment and our header space from the availability
	blk->size = (uintptr_t) addr + size - (uintptr_t) blk - ALLOC_HEADER_SZ;

	//Whoa! ... and now our giant block of memory is added to the list!
	list_add(&blk->node, &free_list);
	if_created=1;
}



