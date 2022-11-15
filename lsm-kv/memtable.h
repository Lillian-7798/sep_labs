#include <string>
#include <vector>
#include <list>
#include <time.h>
#include <limits.h>

#define MAX_LEVEL 10

enum SKNodeType
{
    HEAD = 1,
    NORMAL,
    NIL
};

struct SKNode
{
    uint64_t key;
    std::string val;
    SKNodeType type;
    std::vector<SKNode *> forwards;
    SKNode(int _key, std::string _val, SKNodeType _type)
        : key(_key), val(_val), type(_type)
    {
        for (int i = 0; i < MAX_LEVEL; ++i)
        {
            forwards.push_back(nullptr);
        }
    }
};
class MemTable{
private:
	SKNode *head;
    SKNode *NIL;
    int level = 1;
    unsigned long long s = 1;
    int space=0;
    //int  my_rand();
    int randomLevel();
public:
    int kvnum=0;
    MemTable()
    {
        head = new SKNode(0, 0, SKNodeType::HEAD);
        NIL = new SKNode(INT_MAX, 0, SKNodeType::NIL);
        for (int i = 0; i < MAX_LEVEL; ++i)
        {
            head->forwards[i] = NIL;
        }
        srand(time(NULL));
    }
	void insert(uint64_t key, const std::string &s);
	std::string search(uint64_t key);
	//bool del(uint64_t key);
	void clear();
    bool isOver(uint64_t key, const std::string &s);
    void scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string> > &list);
    SKNode* getFirst(){ return head->forwards[0];}
    SKNode* getLast(){
        SKNode *p=head;
        while(p->forwards[0]!=NIL){
            p=p->forwards[0];
        }
        return p;
        }
	 ~MemTable()
    {
        SKNode *n1 = head;
        SKNode *n2;
        while (n1)
        {
            n2 = n1->forwards[0];
            delete n1;
            n1 = n2;
        }
    }
};