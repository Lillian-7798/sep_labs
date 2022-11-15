#include "MurmurHash3.h"
#include <map>
#include <vector>
struct  Header
{
    unsigned long long time;
    unsigned long long num;
    unsigned long long keymin;
    unsigned long long keymax;
    // Header(unsigned long long t,unsigned long long n,unsigned long long min,unsigned long long max){
    //     time=t;num=n;keymin=min;keymax=max;
    // }
};
struct BloomFilter{
bool table[10240];
void set(uint64_t key){
    unsigned int hash[4] = {0};
    MurmurHash3_x64_128(&key, sizeof(key), 1, hash);
    for(int i=0;i<4;i++){
        table[hash[i]%10240]=true;
    }
}
bool find(uint64_t key){
    unsigned int hash[4]={0};
    MurmurHash3_x64_128(&key,sizeof(key),1,hash);
    for(int i=0;i<4;i++){
        if(!table[hash[i]%10240]) return false;
    }
    return true;
}
BloomFilter(){
    for(int i=0;i<10240;i++){
        table[i]=false;
    }
}
};
class SSTable{
private:
    
public:
    Header header;
    BloomFilter bloom;
    std::map<uint64_t, unsigned int> index;
    SSTable();
    void setHeader(uint64_t time,uint64_t num,uint64_t min,uint64_t max);
};
struct Level
{
    int ssNum;
    std::vector<SSTable> ssTable;
    Level(){
        ssNum=0;
    }
};