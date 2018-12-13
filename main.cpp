#include "alloc.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	size_t MALLOC_SIZE = (4 * 1024 * 1024);
	Allocator * my_block = new Allocator(MALLOC_SIZE);
   

	printf("Allocated a block on the host, pointer: %p, size %d\n", my_block, MALLOC_SIZE);
	
	DoublyLinkedList::Node* t1 = my_block->a_alloc(1000*1000)->blocks;
	DoublyLinkedList::Node* t2 = my_block->a_alloc(3*1000*1000)->blocks;
	DoublyLinkedList::Node* t3 = my_block->a_alloc(100)->blocks;
	DoublyLinkedList::Node* t4 = my_block->a_alloc(200)->blocks;
	DoublyLinkedList::Node* t5 = my_block->a_alloc(300)->blocks;
	DoublyLinkedList::Node* t6 = my_block->a_alloc(400)->blocks;
	DoublyLinkedList::Node* t7 = my_block->a_alloc(400*1000)->blocks;

	//my_block->show_busy_blocks();
	//my_block->show_free_blocks();
	my_block->a_free(t3);
	my_block->a_free(t1);
	my_block->a_free(t1);
	t7 = my_block->a_alloc(400*1000)->blocks;

	my_block->a_realloc(MALLOC_SIZE, 2*MALLOC_SIZE);
	my_block->show_free_blocks();
	my_block->a_defrag();
	my_block->show_busy_blocks();
	getchar();getchar();


}