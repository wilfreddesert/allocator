#include "doubly-linked list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

/*#define align_up(num, align) \
	(((num) + ((align) - 1)) & ~((align) - 1))*/

enum Errors { InvalidFree, NoEnoughMemory, NoPersistMemory, TooBigBlock};
class CleverPtr { 
public:
    DoublyLinkedList::Node * blocks;
    public: 
    void* get(){
        return (void*)blocks->data;
    }

    CleverPtr(DoublyLinkedList::Node* block){
        block->links++;
        blocks = block;
    }

    ~CleverPtr(){
       // this->block->links--;
        //this->block->is_free = true;
    }
};  

class Allocator {
	DoublyLinkedList l;
public:
	Allocator(size_t size){
		
        mem = (void*) malloc(size);
		l.head = (DoublyLinkedList::Node*)((char*)mem + size - sizeof(DoublyLinkedList::Node));
		l.tail = l.head;
		l.head = l.newNode(l.head, size, mem, nullptr, nullptr, true, 1);
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
	void a_free(DoublyLinkedList::Node* blk); // не удалять, если есть ещё ссылающиеся CleverPtr 
    void a_realloc(size_t size, size_t new_size); // увеличить объём памяти, по возможности на том же месте, при этом сбросить частоту обращений к блоку - Frequency 
    void a_defrag(); // c уточнением перемещённых адресов в уже выделенных объектах CleverPtr – сложность работы, не больше линейной, лучше меньше!! (т.к. - известно положение FreeBlocks)
    // при дефрагментации, более часто использующиеся блоки, размещать в начале, чтобы не нужно было их перемещать по памяти
    void free_hang_blocks(); // удалить блоки, на которых не ссылается ни 1 CleverPtr 
	CleverPtr *best_fit_block(size_t size);

};