#include <iostream>

#include "HashTable.h"

using namespace std;

int HashTable::hash(int key)
{
    return key % BUCKET_SIZE;
}

void HashTable::Insert(int key, int value)
{
    // TODO
    int buketNum = hash(key);
    if(bucket[buketNum]==NULL){
	    bucket[buketNum] = new HashNode(key,value);
    }
    else{
	    HashNode *p = bucket[buketNum];
	    while(p->next!=NULL) {
		    if(p->key==key){
			    p->val=value;
			    return;
		    }
		    p=p->next;
	    }
	    if(p->key==key){
		    p->val = value;
		    return;
	    }
	    p->next = new HashNode(key,value);
    }
}

void HashTable::Search(int key)
{
    // TODO
    int buketNum = hash(key);
    HashNode *p = bucket[buketNum];
    if(p==NULL) cout<<"Not Found"<<endl;
    else{
	    int index = 0;
	    while(p != NULL){
		    if(p->key==key){
			    cout<<"bucket "<<buketNum<<" index "<<index<<" key "<<key<<" value "<<p->val<<endl;
			    return;
		    }
		   p=p->next;
		   index++;
	    }
	    cout<<"Not Found"<<endl;
    }
    return;
}

void HashTable::Delete(int key)
{
    // TODO
    int buketNum = hash(key);
    HashNode *p = bucket[buketNum];
    HashNode *q;
    if(p==NULL) return;
    else{
	    if(p->key==key){
		    bucket[buketNum]=p->next;
		    delete p;
		    return;
	    }
	    else{
	    	while(p->next != NULL){
		    if(p->next->key==key){
			q=p->next;
		        p->next=q->next;
		        delete q;
		        return;	
		    }
		    p=p->next;
	    	}
		return;
	    }
    }
}

void HashTable::Display()
{
    for (int i = 0; i < BUCKET_SIZE; ++i)
    {
        std::cout << "|Bucket " << i << "|";
        HashNode *node = bucket[i];
        while (node)
        {
            std::cout << "-->(" << node->key << "," << node->val << ")";
            node = node->next;
        }
        std::cout << "-->" << std::endl;
    }
}
