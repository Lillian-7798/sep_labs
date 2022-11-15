#include <iostream>
#include <stdlib.h>

#include "SkipList.h"
using namespace std;

int SkipList::randomLevel()
{
    int result = 1;
    while (result < MAX_LEVEL && (double)rand() / RAND_MAX < 0.5)
    {
        ++result;
    }
    return result;
}

void SkipList::Insert(int key, int value)
{
    // TODO
    //cout<<1;
    std::vector<SKNode*> update;
    SKNode *x= head;
    //cout<<"test"<<endl;
    for(int i=MAX_LEVEL-1;i>=0;i--){
	    while(x!=NIL&&x->forwards[i]->key<key)
	    	x=x->forwards[i];
	    update[i]=x;
    }
    if(x!=NIL)x=x->forwards[1];
    if(x->key==key) x->val=value;
    else{
	    int v=randomLevel();
	    /*if(v>MAX_LEVEL){
		    for(int i=MAX_LEVEL;i<v;i++)
			    update[i]=head;
		    MAX_LEVEL = v;
	    }*/
	    x = new SKNode(key,value,NORMAL);
		    for(int i=0;i<v;i++){
			    x->forwards[i]=update[i]->forwards[i];
			    update[i]->forwards[i]=x;
		    
	    }
    }
}

void SkipList::Search(int key)
{
    // TODO
    SKNode *x = head;
    for(int i = MAX_LEVEL-1;i>=0;i--){
	    while(x!=NIL&&x->forwards[i]->key<key){
		    cout<<i+1<<',';
		    if(x->type==HEAD) cout<<"h ";
		    else {
			    if(x->type==3) cout<<"N ";
			    else cout<<x->key<<' ';
		    }
		    x=x->forwards[i];
	    }
    }
    if(x!=NIL)x=x->forwards[1];
    if(x->key==key) cout<<x->val<<endl;
    else cout<<"Not Found"<<endl;
}

void SkipList::Delete(int key)
{
    // TODO
	std::vector<SKNode*> update;
	SKNode *x=head;
	for(int i=MAX_LEVEL-1;i>=0;i--){
		while(x!=NIL&&x->forwards[i]->key<key)
			x=x->forwards[i];
		update[i]=x;
	}
	if(x!=NIL)x=x->forwards[1];
	if(x->key==key){
		for(int i=0;i<MAX_LEVEL;i++){
			if(update[i]->forwards[i]!=x) break;
			update[i]->forwards[i]=x->forwards[i];
		}
		delete x;
		/*while(MAX_LEVEL>1&&head->forwards[MAX_LEVEL-1]->type==3)
			MAX_LEVEL=MAX_LEVEL-1;*/
	}
}

void SkipList::Display()
{
    for (int i = MAX_LEVEL - 1; i >= 0; --i)
    {
        std::cout << "Level " << i + 1 << ":h";
        SKNode *node = head->forwards[i];
        while (node->type != SKNodeType::NIL)
        {
            std::cout << "-->(" << node->key << "," << node->val << ")";
            node = node->forwards[i];
        }

        std::cout << "-->N" << std::endl;
    }
}
