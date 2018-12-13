#include <stdio.h>
#include <stdlib.h>

class DoublyLinkedList {

public:
	 DoublyLinkedList()
    {
        head = NULL;
        tail = NULL;
    };

    struct Node {
        size_t size;
		Node* previous;
		Node* next;
        bool is_free;
        int links;
        void* data;
		Node():
			size(0), 
			previous(nullptr),
			next(nullptr),
			is_free(false),
			links(0),
			data(NULL) {}
    };


    Node *tail, *head;

	Node*  newNode (Node* tmp, size_t size, void *data, Node *previous, Node* next, bool is_free, int links){
//		tmp = (Node*)addr;
		tmp->data = data;  
		tmp->size = size;
		tmp->is_free = is_free;
		tmp->links = links;
		tmp->previous = previous;
		if(tmp->previous!=nullptr) {
			tmp->previous->next = tmp;
		}
		tmp->next = next;
		if(tmp->next!=nullptr){
			tmp->next->previous = tmp;
		}
		return tmp;
	}

   /* void insert(Node* previousNode, Node* newNode){
        if (previousNode == nullptr) {
        // Is the first node
        if (head != nullptr) {
            // The list has more elements
            newNode->next = head;           
            newNode->next->previous = newNode;
        }else {
            newNode->next = nullptr;
        }
        head = newNode;
        head->previous = nullptr;
    } else {
        if (previousNode->next == nullptr){
            // Is the last node
            previousNode->next = newNode;
            newNode->next = nullptr;
        }else {
            // Is a middle node
            newNode->next = previousNode->next;
            if (newNode->next != nullptr){
                newNode->next->previous = newNode;
            }
            previousNode->next = newNode;
            newNode->previous = previousNode;
        }
    }
    };*/
    void remove(Node* deleteNode){
         if (deleteNode->previous == nullptr){
        // Is the first node
            if (deleteNode->next == nullptr){
                // List only has one element
                head = nullptr;            
            }else {
                // List has more elements
                head = deleteNode->next;
                head->previous = nullptr;
            }
        }
        else {
            if (deleteNode->next == nullptr){
                // Is the last node
                deleteNode->previous->next = nullptr;
            }else {
                // Middle node
                deleteNode->previous->next = deleteNode->next;
                deleteNode->next->previous = deleteNode->previous;
            }
        }
    };
	void* find_last(void * mem){
		size_t MALLOC_SIZE = (4 * 1024 * 1024);
		Node* tmp;
		tmp = head;
		void* min_addr = (char*)mem + MALLOC_SIZE;
		while (tmp!=NULL)
		{
			if (tmp<=min_addr)
			{
				min_addr = tmp;
			}
			tmp = tmp->next;
		}
		return min_addr;
	}
private:
    DoublyLinkedList(DoublyLinkedList &doublyLinkedList);
};



/*  void display()


  {
    node *temp=new node;
    temp=head;
    while(temp!=NULL)
    {
      cout<<temp->data<<"\t";
      temp=temp->next;
    }
  } */

/*
void FreeListAllocator::Coalescence(Node* previousNode, Node * freeNode) {   
    if (freeNode->next != nullptr && 
            (std::size_t) freeNode + freeNode->data.blockSize == (std::size_t) freeNode->next) {
        freeNode->data.blockSize += freeNode->next->data.blockSize;
        m_freeList.remove(freeNode, freeNode->next);
#ifdef _DEBUG
    std::cout << "\tMerging(n) " << (void*) freeNode << " & " << (void*) freeNode->next << "\tS " << freeNode->data.blockSize << std::endl;
#endif
    }
    
    if (previousNode != nullptr &&
            (std::size_t) previousNode + previousNode->data.blockSize == (std::size_t) freeNode) {
        previousNode->data.blockSize += freeNode->data.blockSize;
        m_freeList.remove(previousNode, freeNode);
#ifdef _DEBUG
    std::cout << "\tMerging(p) " << (void*) previousNode << " & " << (void*) freeNode << "\tS " << previousNode->data.blockSize << std::endl;
#endif
    }
}
*/