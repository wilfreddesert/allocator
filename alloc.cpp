#include "alloc.h"
#include <stdio.h>
#include <utility> 


CleverPtr* Allocator::a_alloc(size_t size){
	printf("Allocating a block of size :%d\n",size);
	if(size<=Allocator::maxblocksize){
		DoublyLinkedList::Node *tmp = l.head;
		CleverPtr *cur_d = (CleverPtr*)(mem);
		cur_d->blocks = nullptr;
		while(tmp!=NULL){
			if (tmp->is_free){
				if(tmp->size >= size){
					
					void * addr = l.find_last(mem);
					DoublyLinkedList::Node *blk = (DoublyLinkedList::Node*)((char *)addr - sizeof(DoublyLinkedList::Node));
					cur_d = Allocator::best_fit_block(size);
					tmp->size -= size;
					blk = l.newNode(blk, size, (char*)cur_d->blocks->data+size, tmp->previous, tmp, false, 1);
					if (blk->previous==nullptr){
						l.head = blk;
					}
					if (blk->next==nullptr){
						l.tail = blk;
					}
					CleverPtr ret = CleverPtr::CleverPtr(blk);
					return &ret;
				}
			}
			tmp = tmp->next;
		}
			printf("\nERROR! Not enough space\n\n");
			return cur_d;
	}
}

void Allocator::a_free(DoublyLinkedList::Node* blk){
	printf("Freeing a block...\n");
	if (!(blk->is_free)){
       if(blk->previous!=NULL && blk->previous->is_free)
       {
		   if(blk->next!=NULL && blk->next->is_free){
               blk->previous->size += (blk->size + blk->next->size);
               l.remove(blk->next);
               l.remove(blk);
               return;
           }
           blk->previous->size +=blk->size;
           l.remove(blk);

           return;
       }
       if(blk->next!=NULL && blk->next->is_free)
       {
           blk->next->size += blk->size;
           l.remove(blk);
           return;
       }
       blk->is_free=true;
       return;
   }
	printf("\nERROR! Block is already free\n\n");
}

void Allocator::a_realloc(size_t size, size_t new_size){
	printf("Reallocating a block of size :%d\n",new_size);
	void *tmp = mem;
    //memmove(tmp, mem, sizeof(mem));
    //free(mem);
	Allocator::Allocator(new_size);
	memmove(mem, tmp, sizeof(tmp));
	void * addr = l.find_last(mem);
	DoublyLinkedList::Node *block = (DoublyLinkedList::Node*)((char *)addr - sizeof(DoublyLinkedList::Node));
	block = l.newNode(block, new_size-size, mem, l.tail, nullptr, true, 1);
	l.tail = block;
	return;
}

void Allocator::a_defrag(){
	printf("Defragmentation in progress...\n");
	DoublyLinkedList::Node *tmp = l.head;
	while(tmp!=NULL)
	{
		if(tmp->next == NULL){
			l.tail=tmp;
			break;
		}
		tmp = tmp->next;
		
	}
	while(tmp!=NULL){
		if(tmp->is_free && tmp->next!=NULL){
			tmp = tmp ->next;
			if (l.tail->is_free){
				l.tail->size += tmp->previous->size;
			}
			else {
//				l.insert(l.tail, tmp->previous);
			}
			memmove(tmp->previous, tmp, sizeof(tmp));
			tmp = l.head;
		}
		tmp = tmp->next;
	}
}

void Allocator::show_busy_blocks(){
	printf("\n Busy blocks:\n");
    DoublyLinkedList::Node *tmp = l.head;
    while(tmp!=NULL){
        if (!tmp->is_free && tmp->links!=0){
			printf("Address:%p, Size:%d\n", tmp->data, tmp->size);
        }
        tmp = tmp->next;
   }   
}

void Allocator::show_free_blocks(){
	printf("\n Free blocks:\n");
    DoublyLinkedList::Node *tmp = l.head;
    while(tmp!=NULL){
        if (tmp->is_free && tmp->links!=0){
			printf("Address:%p, Size:%d\n", tmp->data, tmp->size);
        }
        tmp = tmp->next;
   }    
}

void Allocator::show_hang_blocks(){
	printf("\n Hang blocks:\n");
    DoublyLinkedList::Node *tmp = l.head;
    while(tmp!=NULL){
        if (!tmp->links){
			printf("Address:%p, Size:%d\n", tmp->data, tmp->size);
        }
        tmp = tmp->next;    
   }
}

void Allocator::free_hang_blocks(){
    DoublyLinkedList::Node *tmp = l.head;
    while(tmp!=NULL){
        if (!tmp->links){
			if (!(tmp->is_free)){
				if(tmp->previous->is_free)
				{
					if(tmp->next->is_free){
					   tmp->previous->size += (tmp->size + tmp->next->size);
					   l.remove(tmp->next);
					   l.remove(tmp);
					}
					else{
						tmp->previous->size +=tmp->size;
						l.remove(tmp);
					}
				}
				else{
					if(tmp->next->is_free)
					{
						tmp->next->size += tmp->size;
						l.remove(tmp);
					}
					else{
						tmp->is_free=true;
					}
				}
			}
		}
        tmp = tmp->next;
	}
	return;
}

CleverPtr* Allocator::best_fit_block(size_t size)
{
	size_t minsize = maxblocksize;
	DoublyLinkedList::Node *tmp = l.head;
	DoublyLinkedList::Node *fit_block;
	while(tmp!=NULL)
	{
		if(tmp->is_free)
			if((minsize >= tmp->size) && (size <= tmp->size)) {
				minsize = tmp->size;
				fit_block = tmp;
			}
			tmp = tmp->next;
	}
	CleverPtr ret = CleverPtr::CleverPtr(fit_block);
	return &ret;
}