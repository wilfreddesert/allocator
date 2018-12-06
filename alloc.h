#include "doubly-linked list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#define align_up(num, align) \
	(((num) + ((align) - 1)) & ~((align) - 1))

enum Errors { InvalidFree, NoEnoughMemory, NoPersistMemory, TooBigBlock};
class CleverPtr { 
public:
    DoublyLinkedList::Node * block;
    public: 
    void* get(){
        return (void*)block->data;
    }

    CleverPtr(DoublyLinkedList::Node* block){
        block->links++;
        this->block = block;
    }

    ~CleverPtr(){
        this->block->links--;
        this->block->is_free = true;
    }
};  

class Allocator {
public:
	Allocator(size_t size){
        this->mem = malloc(size);
        DoublyLinkedList::Node *blk = new DoublyLinkedList::Node;
        blk->data = (void *)align_up((uintptr_t)mem, sizeof(void*));
        blk->size = (uintptr_t)mem + size - (uintptr_t) blk;
        blk->is_free = true;
		blk->links = 0;
		
    }

	~Allocator()
    {
        free(this->mem);
    }
public:  
	void* mem;
    size_t maxblocksize;
    void show_busy_blocks();
    void show_free_blocks();
    void show_hang_blocks();
	CleverPtr* a_alloc(size_t size); // выделить новый блок
    void a_free(CleverPtr* blk); // не удалять, если есть ещё ссылающиеся CleverPtr 
    void a_realloc(CleverPtr* mem, size_t new_size); // увеличить объём памяти, по возможности на том же месте, при этом сбросить частоту обращений к блоку - Frequency 
    void a_defrag(); // c уточнением перемещённых адресов в уже выделенных объектах CleverPtr – сложность работы, не больше линейной, лучше меньше!! (т.к. - известно положение FreeBlocks)
    // при дефрагментации, более часто использующиеся блоки, размещать в начале, чтобы не нужно было их перемещать по памяти
    void free_hang_blocks(); // удалить блоки, на которых не ссылается ни 1 CleverPtr 

};