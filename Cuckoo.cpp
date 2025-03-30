
#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <__msvc_chrono.hpp>
#include"PacketReader.h"
#include"HSNPacketReader.h"
#include"HashFunctions.h"
#include"common.h"
using namespace std;
using namespace std;
typedef unsigned long ULONG;

class CuckooHashing {
    typedef struct Item {
        uint32_t key;
        ULONG value;
        Item() : value(0) {}

        //重载=运算符，拷贝函数
        inline Item& operator=(const Item& entry) {
            key = entry.key;
            value = entry.value;
            return *this;
        }
    };
public:
    CuckooHashing(int size, int step_max, std::function<int(uint32_t)> hash1, std::function<int(uint32_t)> hash2)
        : table1(size), table2(size), hash1(hash1), hash2(hash2), step_max(step_max) {}

    bool insert(uint32_t key) {
        int index1 = hash1(key);
        int index2 = hash2(key);
        hash_count += 2;

        while (step < step_max) {
            insert_visit++;
            if (table1[index1].value == 0) {
                table1[index1].key = key;
                table1[index1].value++;
                step = 0;
                return true;
            }
            else if (table1[index1].key == key) {
                table1[index1].value++;
                step = 0;
                return true;
            }
            else if (table2[index2].key == key) {
                table2[index2].value++;
                step = 0;
                return true;
            }
            else {
                kick_count++;
                step++;
                uint32_t replace_key = table1[index1].key;
                int replace_value = table1[index1].value;
                insert_visit++;
                table1[index1].key = key;
                table1[index1].value = 1;
                int index1 = hash1(replace_key);
                int index2 = hash2(replace_key);
                hash_count += 2;
                if (table2[index2].value == 0) {
                    table2[index2].key = replace_key;
                    table2[index2].value++;
                    step = 0;
                    return true;
                }
                else {
                    uint32_t replace_key2 = table2[index2].key;
                    table2[index2].key = replace_key;
                    table2[index2].value = replace_value;
                    insert_visit++;
                    return insert(replace_key2);
                }
            }
        }
        conflict++;
        step = 0;
        return false;
    }
    bool find(uint32_t key) {
        int index1 = hash1(key);
        int index2 = hash2(key);
        find_visit++;
        if (table1[index1].key != key) {
            find_visit++;
        }
        if (table1[index1].key == key || table2[index2].key == key) {
            return true;
        }
        else {
            return false;
        }
    }
    bool remove(int key) {
        int index1 = hash1(key);
        int index2 = hash2(key);
        if (find(key)) {
            if (table1[index1].key == key) {
                table1[index1].key = 0;
                table1[index1].value = 0;
                return true;
            }
            else {
                table2[index2].key = 0;
                table2[index2].value = 0;
                return true;
            }
        }
        else
            return false;
    }
    int getkick_count() {
        return kick_count;

    }
    int getconflict() {
        return conflict;
    }
    int getinsert_visit() {
        return insert_visit;
    }
    int getfind_visit() {
        return find_visit;
    }
    int gethash_count() {
        return hash_count;
    }
    double getMemory_utilizationrate() {
        double items = 0;
        for (int i = 0; i < table1.size(); i++) {
            if (table1[i].value != 0) {
                items++;
            }
            if (table2[i].value != 0) {
                items++;
            }
        }
        return items;
    }
private:
    std::vector<Item> table1;
    std::vector<Item> table2;
    int kick_count = 0;
    int conflict = 0;
    int step = 0;
    int step_max;
    int insert_visit = 0;
    int find_visit = 0;
    int hash_count = 0;
    std::function<int(uint32_t)> hash1;
    std::function<int(uint32_t)> hash2;
};
void ShowCuckoo(int conflict[], int insert_visit[], int find_visit[]) {
    HSNPacketReader reader("D:/Feb_21_235505.hsn");
    Packet pkt;
    FlowID flows[10000];
    uint32_t flowdata[10000];
    int num = 0;
    while (reader.readPacket(pkt))
    {
        //std::cout << "时间戳： " << pkt.time << std::endl;
        flows[num].proto = pkt.proto;
        flows[num].src = pkt.src;
        flows[num].dst = pkt.dst;
        num++;
        if (num == 10000) {
            break;
        }
    }
    UCHAR buf[FID_LEN];
    for (int j = 0; j < 10000; j++) {
        flows[j].ToData(buf);
        flowdata[j] = RS(buf, FID_LEN);
        cout << flowdata[j] % 5000 << " ";
        if (j % 15 == 0) {
            cout << endl;
        }
    }
    std::vector<uint32_t> sfind_keys;
    sfind_keys.resize(5000);
    std::vector<uint32_t> usfind_keys;
    usfind_keys.resize(5000);
    for (int k = 0, j = 0; k < 5000; k++) {
        sfind_keys[k] = flowdata[j];
        j += 2;
    }
    cout << 2;
    int y = 0;
    for (int j = 0; j < 100000; j++) {
        int judge = 1;
        for (int k = 0; k < 5000; k++) {
            if (j == flowdata[k]) {
                judge = 0;
                break;
            }
        }
        if (judge == 1) {
            usfind_keys[y] = j;
            y++;
        }
        if (y == 5000) {
            break;
        }
    }
    cout << 2.5 << endl;
    for (int i = 2; i < 12; i++) {
        CuckooHashing ch(5000, i, [](const uint32_t& value) { return value % 5000; }, [](const uint32_t& value) { return (value * 3 + 7) % 5000; });
        for (int k = 0; k < 10000; k++) {
            ch.insert(flowdata[k]);
        }
        for (int k = 0; k < 5000; k++) {
            ch.find(sfind_keys[k]);
        }
        conflict[i - 2] = ch.getconflict();
        insert_visit[i - 2] = ch.getinsert_visit();
        find_visit[i - 2] = ch.getfind_visit();
        //find_visit1[i - 2] = ch.getfind_visit();
    }
}
int main() {
    /*CuckooHashing ch1(10, 4, [](const int& value) { return value % 10; }, [](const int& value) { return (value * 3 + 7) % 10; });
    ch1.insert(2);
    ch1.insert(3);
    ch1.insert(4);
    ch1.insert(5);
    ch1.insert(12);
    ch1.insert(22);
    ch1.insert(2);
    cout << ch1.find(2) << endl;
    cout << ch1.find(12) << endl;
    cout << ch1.find(22) << endl;
    cout << ch1.getvisit() << endl;
    CuckooHashing ch(5000,4, [](const int& value) { return value % 5000; }, [](const int& value) { return (value * 3 + 7) % 5000; });
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100000);

    // 生成随机键集合
    std::vector<int> keys;
    for (int i = 0; i < 10000; ++i) {
        keys.push_back(dis(gen));
    }
    auto start = std::chrono::steady_clock::now();

    for (const auto& key : keys) {
        ch.insert(key);
    }
    //for (const auto& key : keys) {
        //cout << key << " ";
    //}
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> cuckooHashDuration = end - start;
    cout << ch.getkick_count() << endl;
    cout << ch.getconflict() << endl;
    cout << ch.getvisit() << endl;
    cout << ch.gethash_count() << endl;
    cout << ch.getMemory_utilizationrate()/10000 << endl;

    std::cout << "Cuckoo Hashing Time: " << cuckooHashDuration.count() << " seconds" << std::endl;*/

    std::vector<int> x = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }; // x轴数据
    int y1[10]; // 第一个数组的数据
    int y2[10]; // 第二个数组的数据
    int y3[10]; // 第四个数组的数据
    ShowCuckoo(y1,y2,y3);
    std::ofstream outputFile("cuckoo_tiaocan_flow.csv"); // 创建一个名为"data.csv"的文件

    //将数据写入CSV文件
    for (size_t i = 0; i < x.size(); ++i) {
        outputFile << x[i] << "," << y1[i] << "," << y2[i] << "," << y3[i] << ",";
    }

    outputFile.close(); // 关闭文件
    return 0;
}
