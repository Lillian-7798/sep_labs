#include "sstable.h"
SSTable::SSTable(){
};
void SSTable::setHeader(uint64_t time,uint64_t num,uint64_t min, uint64_t max){
    header.time=time;
    header.num=num;
    header.keymin=min;
    header.keymax=max;
}