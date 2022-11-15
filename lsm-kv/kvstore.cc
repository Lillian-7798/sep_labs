#include "kvstore.h"
#include <string>
#include <fstream>
#include "utils.h"

#define MB 1024*1024

using namespace std;

/******************KVStore****************/
KVStore::KVStore(const std::string &dir) : KVStoreAPI(dir)
{
    Level tmp;
    level.push_back(tmp);
}

KVStore::~KVStore()
{
}
//利用静态局部变量获取时间戳
uint64_t KVStore::gettime()
{
    static uint64_t time = 0;
    time++;
    return time;
}
/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
    if (!memTable.isOver(key, s))
    {
        memTable.insert(key, s);
    }
    else
    {
        if (!utils::dirExists("sstables/level0"))
        {
            utils::_mkdir("sstables/level0");
        }
        std::ofstream outFile("newsstable.sst", std::ios::app | std::ios::binary); //创建一个新的文件，先叫newsstable
        //先构造sstable header
        SSTable tmp;
        int time = gettime();
        int num = memTable.kvnum;
        int min = memTable.getFirst()->key;
        int max = memTable.getLast()->key;
        tmp.setHeader(time, num, min, max);
        outFile.write((char *)(&time), sizeof(uint64_t));
        outFile.write((char *)(&num), sizeof(uint64_t));
        outFile.write((char *)(&min), sizeof(uint64_t));
        outFile.write((char *)(&max), sizeof(uint64_t));
        //先遍历一边写bloom filter和index
        SKNode *p = memTable.getFirst();
        unsigned int offset = 4 * sizeof(uint64_t) + 10240 + num * (sizeof(uint64_t) + sizeof(unsigned int));
        while (p->forwards[0] != NULL)
        {
            uint64_t tmpkey = p->key;
            std::string tmpv = p->val;
            tmp.bloom.set(tmpkey);
            tmp.index[tmpkey] = offset;
            offset += tmpv.length();
            p = p->forwards[0];
        };
        level[0].ssNum++;
        level[0].ssTable.push_back(tmp);
        //再写入bloom filter和index之后，再遍历一边写入数据
        //写入bloom filter
        for (int i = 0; i < 10240; i++)
        {
            outFile.write((char *)tmp.bloom.table[i], sizeof(bool));
        }
        //写入index
        for (map<uint64_t, unsigned int>::iterator iter = tmp.index.begin(); iter != tmp.index.end(); ++iter)
        {
            outFile.write((char *)iter->first, sizeof(uint64_t));
            outFile.write((char *)iter->second, sizeof(unsigned int));
        }
        //写入val
        SKNode *q = memTable.getFirst();
        while (q->forwards[0] != NULL)
        {
            outFile.write(q->val.data(), q->val.length());
            q = q->forwards[0];
        }
        //修改文件名称
        int fileNum = level[0].ssNum - 1; //注意，文件标号是文件数目-1
        std::string sstableName = "./sstables/level0/sstable" + std::to_string(fileNum) + ".sst";
        char *charsstableName = (char *)sstableName.data();               //转成char*才能写，我也不知道为啥
        std::rename("./sstables/level0/newsstable.sst", charsstableName); //文件从newsstable改成文件标号
        //清空memtabel,重新插入key
        memTable.clear();
        memTable.insert(key, s);
        //合并算法
        if(level[0].ssNum>2) compaction(0);
    }
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
    std::string tmp;
    tmp = memTable.search(key);
    if (tmp == "~DELETED~")
        tmp = "";
    if (tmp != "")
        return tmp;
    else{
        // search sstable
        //先从level0开始逐层寻找，先比较keymin-keymax
        for (int i=0;i<level.size();i++){
            for(int j=0;j<level[i].ssTable.size();j++){
                SSTable tmp=level[i].ssTable[j];
                if(key<tmp.header.keymin||key>tmp.header.keymax) continue;
                else{
                    //通过bloom粗略key是否存在
                    if(!tmp.bloom.find(key)) continue;
                    else{
                        //通过bloom判断存在后用index取出位置
                        map<uint64_t, unsigned int >::iterator iter;
		                iter = tmp.index.find(key);
                        if(iter ==tmp.index.end()) continue;
                        else{
                            unsigned int off = tmp.index[key]; //获取offset
                            iter++;
                            //在对应的sstable文件中获取val值
                            std::string fileName = "./sstables/level" + std::to_string(i) +
                                                   "/sstable" + std::to_string(j) + ".sst";
                            std::ifstream inFile(fileName, std::ios::in | std::ios::binary);
                            inFile.seekg(off, ios::beg);
                            char *buf;
                            inFile.read((char *)&buf, iter->second-off);
                            std::string ans = buf;
                            return ans;
                        }
                    }
                }
            }
        }
        return "";
    }
    // return "";
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
    if (get(key).empty())
        return false;
    else
    {
        memTable.insert(key, "~DELETED~");
        return true;
    }
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
    memTable.clear();
    int L=level.size();
    for(int i=0;i<L;i++){
        int S=level[i].ssNum;
        for(int j=0;j<S;j++){
            string filePath="./sstables/level" + std::to_string(i) +"/sstable"+std::to_string(j)+ ".sst";
            utils::rmfile((char *)filePath.data());
        }
        string dirPath="./sstables/level"+std::to_string(i);
        utils::rmdir((char *)dirPath.data());
    }
}
//list自排序
bool Compare(std::pair<uint64_t, std::string>& p1,std::pair<uint64_t, std::string>& p2){
    return p1.first<p2.first;
}

/**
 * Return a list including all the key-value pair between key1 and key2.
 * keys in the list should be in an ascending order.
 * An empty string indicates not found.
 */
void KVStore::scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string>> &list)
{
    // search the memtable
    memTable.scan(key1, key2, list);
    // insert the key in sstable
    for(int i=0;i<level.size();i++){
        int S=level[i].ssNum;
        for(int j=0;j<S;j++){
            if(level[i].ssTable[j].header.keymin>key2||level[i].ssTable[j].header.keymax<key1)
            continue;
            else
            {
                map<uint64_t, unsigned int>::iterator it;
                for (it = level[i].ssTable[j].index.begin(); it != level[i].ssTable[j].index.end(); it++)
                {
                    if(it->first<key1) continue;
                    if(it->first>=key1&&it->first<=key2){
                        unsigned int off = it->second; //获取offset
                        map<uint64_t, unsigned int>::iterator iter=it;
                        iter++;
                        //在对应的sstable文件中获取val值
                        std::string fileName = "./sstables/level" + std::to_string(i) +
                                               "/sstable" + std::to_string(j) + ".sst";
                        std::ifstream inFile(fileName, std::ios::in | std::ios::binary);
                        inFile.seekg(off, ios::beg);
                        char *buf;
                        inFile.read((char *)&buf, iter->second - off);
                        std::string ans = buf;
                        //写入list
                        list.push_back(make_pair(it->first,ans));
                    }
                    if(it->first>key2) break;
                }
            }
        }
    }
    //list.sort排序
    list.sort(Compare);
}
/*合并操作*/
void KVStore::compaction(int leveln){
    int curLevel=leveln+1;
    if(level[leveln].ssNum<= pow(2, leveln + 1)) return;
    string curPath ="sstables/level" + to_string(curLevel);
    //当合并为第一层时
    if(leveln==0){
        if (!utils::dirExists(curPath))
        {
            utils::_mkdir(curPath.c_str());
            Level tmp;
            level.push_back(tmp);
        }
        if(level[1].ssNum==0){
            //当第一层没有文件时
            unsigned long long time=0;
            unsigned long long num=0;
            uint64_t key;
            std::string value;
            std::map<uint64_t,std::string> k_v;
            std::map<uint64_t,unsigned long long> k_t;
            //读取第一层文件
            for(int i=0;i<level[0].ssNum;i++){
                unsigned long long nt=level[0].ssTable[i].header.time;
                if(time<nt) time=nt;
                num+=level[0].ssTable[i].header.num;
                map<uint64_t, unsigned int>::iterator it;
                std::string fileName = "./sstables/level0/sstable" + std::to_string(i) + ".sst";
                std::ifstream inFile(fileName, std::ios::in | std::ios::binary);
                for (it = level[0].ssTable[i].index.begin(); it != level[0].ssTable[i].index.end(); it++)
                {
                        unsigned int off = it->second; //获取offset
                        map<uint64_t, unsigned int>::iterator iter=it;
                        iter++;
                        //在对应的sstable文件中获取val值
                        inFile.seekg(off, ios::beg);
                        char *buf;
                        inFile.read((char *)&buf, iter->second - off);
                        key=it->first;
                        value = buf;
                        if((k_v.find(key)==k_v.end())||(k_v.find(key)!=k_v.end()&&k_t[key]<nt)){
                            k_v[key]=value; 
                            k_t[key]=nt;
                        }                                         
                }
                utils::rmfile((char *)fileName.data());//删除文件
            }
            int size = (32 + 10240);
            SSTable tmp;
            tmp.header.time=time;
            int num =0;
            int min =k_v.begin()->first;
            int max ;
            std::map<uint64_t,std::string>::iterator it;
            for(it=k_v.begin();it!=k_v.end();it++){
                size+=12+sizeof(it->second);
                if((unsigned int)size > 2 * MB){
                    tmp.setHeader(time,num,min,max);
                    unsigned int offset = 4 * sizeof(uint64_t) + 10240 + num * (sizeof(uint64_t) + sizeof(unsigned int));
                    // std::ofstream outFile("newsstable.sst", std::ios::app | std::ios::binary);
                    std::string level1FileName = "./sstables/level1/sstable" + std::to_string(level[1].ssNum) + ".sst";
                    std::ofstream outFile(level1FileName, std::ios::app | std::ios::binary); //要写入的sstable
                    //写入header
                    outFile.write((char *)(&time), sizeof(uint64_t));
                    outFile.write((char *)(&num), sizeof(uint64_t));
                    outFile.write((char *)(&min), sizeof(uint64_t));
                    outFile.write((char *)(&max), sizeof(uint64_t));
                    //写入bloom filter
                    for (int i = 0; i < 10240; i++)
                    {
                        outFile.write((char *)tmp.bloom.table[i], sizeof(bool));
                    }
                    //获取index
                    for(std::map<uint64_t,std::string>::iterator iter=k_v.find(min);iter!=it;iter++){
                        tmp.index[iter->first]=offset;
                        offset+=iter->second.length();
                    }
                    //写入index
                    for (map<uint64_t, unsigned int>::iterator iter = tmp.index.begin(); iter != tmp.index.end(); ++iter)
                    {
                        outFile.write((char *)iter->first, sizeof(uint64_t));
                        outFile.write((char *)iter->second, sizeof(unsigned int));
                    }
                    //写入val
                    for(std::map<uint64_t,std::string>::iterator iter=k_v.find(min);iter!=it;iter++){
                        outFile.write((char *)iter->second.data(),iter->second.length());
                    }
                    level[1].ssTable.push_back(tmp);
                    level[1].ssNum++;
                    num=0;
                    min=it->first;
                    SSTable t;
                    tmp=t;
                    tmp.header.time=time;
                }
                num++;
                max=it->first;
                tmp.bloom.set(it->first);
            }
        }
        else//如果level1中有文件
        {

        }
    }
    //合并为其他层时
    else{

    }
}
