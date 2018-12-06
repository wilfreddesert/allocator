#include "alloc.h"
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
	size_t MALLOC_SIZE = (4 * 1024 * 1024);
	 Allocator my_block = Allocator::Allocator(MALLOC_SIZE);
	//Allocator *my_block = new Allocator(MALLOC_SIZE);


	printf("Allocated a block on the host, pointer: %p, size %d\n", my_block, MALLOC_SIZE);

	CleverPtr t1 = *my_block.a_alloc(100);
	printf("Malloc'd from free list: %p\n", t1);

	CleverPtr t2 = *my_block.a_alloc(100);
	printf("Malloc'd from free list: %p\n", t2);

	printf("Putting first pointer back into the pool\n");
	(my_block).a_free(&t1), t1 = NULL;

	printf("Alloc'ing same size as first pointer\n");
	t1 = *my_block.a_alloc(200);

	printf("Malloc'd from free list: %p\n", t1);

	my_block.a_free(&t2), t2 = NULL;
	my_block.a_free(&t1), t1 = NULL;
	delete[] &my_block;
	getchar();getchar();
}
