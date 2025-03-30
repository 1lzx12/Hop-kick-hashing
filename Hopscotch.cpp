#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <fstream>
using namespace std;
template <typename KeyType, typename ValueType>
class  hopscotchHashTable {
private:
    struct Node {
        KeyType key;
        ValueType value;
        bool isOccupied;
        Node() : isOccupied(false) {}
    };

    std::vector<Node> data;
    int capacity;
    int H;
    int size;
    int conflict = 0;
    int insert_visit = 0;
    int find_visit = 0;
    int hash_count = 0;

    int hash(const KeyType& key) {
        // 使用简单的取模运算作为哈希函数
        hash_count++;
        return static_cast<uint32_t>(key) % capacity;
    }

    int linear_probe(int i) {
        insert_visit++;
        while (data[i].isOccupied) { // 如果桶不为空
            insert_visit++;
            ++i; // 继续向后查找
            if (i == capacity) { // 如果到达桶的末尾
                i = 0; // 从头开始查找
            }
        }
        return i; // 返回空桶的位置
    }

public:
    hopscotchHashTable(int initialCapacity, int h) : capacity(initialCapacity), H(h), size(0) {
        data.resize(capacity);
    }
    int getconflict() {
        return conflict;
    }
    int gethash_count() {
        return hash_count;
    }
    int getinsert_visit() {
        return insert_visit;
    }
    int getfind_visit() {
        return find_visit;
    }
    double getMemory_utilizationrate() {
        return size;
    }
    bool insert(const KeyType& key, const ValueType& value) {
        int i = hash(key); // 计算元素 key 的哈希值对应的桶索引
        for (int k = 0; k < H; k++) {
            insert_visit++;
            int index = (i + k) % capacity;
            if (data[index].key == key) {
                return true;
            }
        }
        int j = linear_probe(i); // 线性探测找到空桶的位置
        int swap_attempts = 0;
        int isconflict = 0;
        while (j - i >= H) { // 如果距离超过跳表的层数
            isconflict = 1;
            int y = find_swap_candidate(j); // 寻找交换候选桶
            if (y == j || swap_attempts >= H) {
                // 如果找不到合适的交换候选桶或者交换尝试次数达到上限
                // 可以选择跳出循环或者进行其他处理
                conflict++;
                return false;
            }
            swap(y, j); // 交换桶中的元素
            j = linear_probe(i); // 重新线性探测找到空桶的位置
            swap_attempts++;
        }
        if (isconflict == 1) {
            conflict++;
        }
        data[j].key = key;
        data[j].value = value;
        data[j].isOccupied = true;
        size++;
        return true;
    }
    void swap(int x, int y) {
        data[y].key = data[x].key;
        data[y].value = data[x].value;
        data[y].isOccupied = true;
        data[x].isOccupied = false;
    }
    int find_swap_candidate(int j) {
        for (int k = j - H + 1; k < j; k++) { // 遍历跳表的层数
            insert_visit++;
            if (hash(data[k].key) > j - H) { // 如果桶中的第一个元素的哈希值满足条件
                return k; // 返回该桶作为交换候选桶
            }
        }
        return j; // 如果没有找到合适的桶，返回当前桶作为交换候选桶
    }
    void remove(const KeyType& key) {
        int index = hash(key);

        while (data[index].isOccupied) {
            if (data[index].key == key) {
                data[index].isOccupied = false;
                size--;
                return;
            }
            index = (index + 1) % capacity;
        }
    }

    bool search(const KeyType& key, ValueType& value) {
        int index = hash(key);
        int find_time = 0;
        find_visit++;
        while (data[index].isOccupied && find_time < H) {
            find_visit++;
            if (data[index].key == key) {
                value = data[index].value;
                return true;
            }
            index = (index + 1) % capacity;
            find_time++;
        }
        return false;
    }
};
void ShowHopscotch(int conflict[], int insert_visit[], int find_visit[], double memory[], int hash_count[]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100000);

    // 生成随机键集合
    std::vector<int> keys;
    for (int i = 0; i < 10000; ++i) {
        keys.push_back(dis(gen));
    }
    std::vector<int> sfind_keys;
    sfind_keys.resize(5000);
    for (int k = 0, j = 0; k < 5000; k++) {
        sfind_keys[k] = keys[j];
        j += 2;
    }
    for (int i = 2; i < 12; i++) {
        hopscotchHashTable<int, int> hashTable(10000, i);

        for (const auto& key : keys) {
            hashTable.insert(key, 1);
        }
        int value;
        for (int k = 0; k < 5000; k++) {
            hashTable.search(sfind_keys[k], value);
        }
        conflict[i - 2] = hashTable.getconflict();
        insert_visit[i - 2] = hashTable.getinsert_visit();
        find_visit[i - 2] = hashTable.getfind_visit();
        memory[i - 2] = hashTable.getMemory_utilizationrate() / 10000;
        hash_count[i - 2] = hashTable.gethash_count();
    }
}
int main() {
    hopscotchHashTable<int, int> hashTable(10, 3);
    hashTable.insert(1, 1);
    hashTable.insert(2, 1);
    hashTable.insert(12, 1);
    //hashTable.insert(3, 1);
    //hashTable.insert(22, 1);
    cout << hashTable.getconflict() << endl;
    cout << hashTable.getinsert_visit() << endl;
    
    return 0;
}