#include "alloc.h"
#include <stdio.h>
#include <utility> 
using namespace std;

CleverPtr* Allocator::a_alloc(size_t size){
   if(size<=Allocator::maxblocksize){
        DoublyLinkedList::Node* tmp = new DoublyLinkedList::Node;
		tmp = DoublyLinkedList::head;
        size = align_up(size, sizeof(void *));
        while(tmp!=NULL){
            if (tmp->is_free){
                if(tmp->size >= size){
                   tmp->size -= size;
                   DoublyLinkedList::Node *blk = new DoublyLinkedList::Node;
                   DoublyLinkedList::insert(tmp->previous, blk);
				   tmp->previous->size = size;
                   tmp->previous->is_free = false;
                   return new CleverPtr(blk);
               }
           }
           tmp = tmp->next;
       }
   }
}

void Allocator::a_free(CleverPtr* blk){
	if (!(blk->block->is_free)){
       if(blk->block->previous->is_free)
       {
            if(blk->block->next->is_free){
               blk->block->previous->size += (blk->block->size + blk->block->next->size);
               DoublyLinkedList::remove(blk->block->next);
               DoublyLinkedList::remove(blk->block);
               return;
           }
           blk->block->previous->size +=blk->block->size;
           DoublyLinkedList::remove(blk->block);

           return;
       }
       if(blk->block->next->is_free)
       {
           blk->block->next->size += blk->block->size;
           DoublyLinkedList::remove(blk->block);
           return;
       }
       blk->block->is_free=true;
       return;
   }
}

void Allocator::a_realloc(CleverPtr* mem, size_t new_size){
    DoublyLinkedList::Node *tmp;
    memmove(tmp, mem, sizeof(mem));
    free(mem);
	mem->block->data =  malloc(new_size);
	mem->block->data = (void *)align_up((uintptr_t)mem, sizeof(void*));
    mem->block->size = (uintptr_t)mem + new_size;
	DoublyLinkedList::head = mem->block;
    memmove(mem, tmp, sizeof(tmp));
}

void Allocator::a_defrag(){
    
}

void Allocator::show_busy_blocks(){
   DoublyLinkedList::Node *tmp = new DoublyLinkedList::Node;
   tmp = DoublyLinkedList::head;
   while(tmp!=NULL){
        if (!tmp->is_free && tmp->links!=0){
            printf("Address:%p, Size:%d\n", &tmp, tmp->size);
        }
        tmp = tmp->next;
   }   
}

void Allocator::show_free_blocks(){
    DoublyLinkedList::Node *tmp = new DoublyLinkedList::Node;
    tmp = DoublyLinkedList::head;
    while(tmp!=NULL){
        if (tmp->is_free && tmp->links!=0){
            printf("Address:%p, Size:%d\n", &tmp, tmp->size);
        }
        tmp = tmp->next;
   }    
}

void Allocator::show_hang_blocks(){
    DoublyLinkedList::Node *tmp = new DoublyLinkedList::Node;
    tmp = DoublyLinkedList::head;
    while(tmp!=NULL){
        if (!tmp->links){
            printf("Address:%p, Size:%d\n", &tmp, tmp->size);
        }
        tmp = tmp->next;    
   }
}

void Allocator::free_hang_blocks(){
    DoublyLinkedList::Node *tmp = new DoublyLinkedList::Node;
    tmp = DoublyLinkedList::head;
    while(tmp!=NULL){
        if (!tmp->links){
			if (!(tmp->is_free)){
				if(tmp->previous->is_free)
				{
					if(tmp->next->is_free){
					   tmp->previous->size += (tmp->size + tmp->next->size);
					   DoublyLinkedList::remove(tmp->next);
					   DoublyLinkedList::remove(tmp);
					}
					else{
						tmp->previous->size +=tmp->size;
						DoublyLinkedList::remove(tmp);
					}
				}
				else{
					if(tmp->next->is_free)
					{
						tmp->next->size += tmp->size;
						DoublyLinkedList::remove(tmp);
					}
					else{
						tmp->is_free=true;
					}
				}
			}
		}
        tmp = tmp->next;
	}
}