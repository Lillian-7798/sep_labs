#include "memtable.h"
/******************MemTable****************/
int MemTable::randomLevel()
{
    int result = 1;
    while (result < MAX_LEVEL && (int)rand()%2 < 1)
    {
        ++result;
    }
    return result;
}
void MemTable::insert(uint64_t key, const std::string &s)
{
	SKNode* update[MAX_LEVEL];
    SKNode *x= head;
    for(int i=level-1;i>=0&&x!=NULL;i--){
	    while(x!=NULL&&x->forwards[i]!=NULL&&x->forwards[i]->key<key)
	    	x=x->forwards[i];
	    update[i]=x;
    }
    if(x!=NULL) x=x->forwards[0];
    if(x!=NULL&&x->key==key) {
        space=space-x->val.length();
        x->val=s; 
        space+=s.length();
        return;
        }
    else{
	    int v=randomLevel();
        if(v>level){
            for(int i =level;i<v;i++){
                update[i]=head;
            }
            level=v;
        }
	    x = new SKNode(key,s,NORMAL);
		    for(int i=0;i<v&&x!=NULL&&update[i]!=NULL;i++){
			    x->forwards[i]=update[i]->forwards[i];
			    update[i]->forwards[i]=x;
		  } 
          space=space+sizeof(key)+s.length()+4;  
          kvnum++;
    }
}
std::string MemTable::search(uint64_t key){
    SKNode *x = head;
    for(int i = level-1;i>=0&&x!=NULL;i--){
	    while(x!=NULL&&x->forwards[i]!=NULL&&x->forwards[i]->key<key){
		    x=x->forwards[i];
	    }
    }
    if(x!=NULL){
	    x=x->forwards[0];
    }
    if(x!=NULL&&x->key==key) return x->val;
    else return "";
}
/*bool MemTable::del(uint64_t key){
    SKNode* update[MAX_LEVEL];
	SKNode *x=head;
    for(int i=level-1;i>=0&&x!=NULL;i--){
		while(x!=NULL&&x->forwards[i]!=NULL&&x->forwards[i]->key<key)
			x=x->forwards[i];
		update[i]=x;
	}
	if(x!=NULL)x=x->forwards[0];
	if(x!=NULL&&x->key==key){
        if(x->val=="~DELETED~") return false;
        else {
            x->val="~DELETED~";
            return true;
            }
        // for(int i=0;i<level&&update[i]!=NULL&&x!=NULL;i++){
		// 	if(update[i]->forwards[i]!=x) break;
		// 	update[i]->forwards[i]=x->forwards[i];
		// }
		// delete x;
        // while(level>1&&head->forwards[level-1]->type==3)
        //     level-=1;
        // return true;
	}
    else return false;
}*/
void MemTable::clear(){
    SKNode *n1 = head->forwards[0];
    SKNode *n2;
    for (int i = 0; i < MAX_LEVEL; ++i)
    {
        head->forwards[i] = NIL;
    }
    while (n1!=NIL)
    {
        n2=n1->forwards[0];
        delete n1;
        n1=n2;
    }
}
void MemTable::scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string> > &list)
{
    SKNode *x = head;
    for(int i = level-1;i>=0&&x!=NULL;i--){
	    while(x!=NULL&&x->forwards[i]!=NULL&&x->forwards[i]->key<key1){
		    x=x->forwards[i];
	    }
    }
    if(x!=NULL){
	    x=x->forwards[0];
    }
    if(x!=NULL&&x->key>=key1) {
        while(x!=NULL&&x->key<=key2){
            list.push_back(make_pair(x->key,x->val));
            x=x->forwards[0];
        }
    }
}
bool MemTable::isOver(uint64_t key, const std::string &s){
    int _space;
    SKNode* update[MAX_LEVEL];
    SKNode *x= head;
    for(int i=level-1;i>=0&&x!=NULL;i--){
	    while(x!=NULL&&x->forwards[i]!=NULL&&x->forwards[i]->key<key)
	    	x=x->forwards[i];
	    update[i]=x;
    }
    if(x!=NULL) x=x->forwards[0];
    if(x!=NULL&&x->key==key) {
        _space=space-x->val.length()+s.length();
        }
    else{
        _space=space+sizeof(key)+s.length()+4;  
    }
    if(_space>2086880) return true;
    else return false;
}