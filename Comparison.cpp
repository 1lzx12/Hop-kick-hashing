
#include <iostream>
#include <chrono>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>
#include"Cuckoo.h"
#include"Hopscotch.h"
#include"Linear_probing.h"
#include"Hop_kick_hashing.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

//各种哈希算法的插入操作所用时间/踢操作次数
//链式哈希
double Insert_chainHash(int num, int keys[]) {
    auto start = std::chrono::steady_clock::now();
    HashMap<int, int> hashMap(10000);
    //std::unordered_map<int, int> chainHash;
    //HashMap<int, int> hashMap(100000);
    for (int i = 0; i < num; ++i) {
        //chainHash[key] = key;
        hashMap.insert(keys[i], 1);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> chainHashDuration = end - start;
    return chainHashDuration.count();
}
//开放地址哈希
double Insert_openAddressHash(int num, int keys[]) {
    auto start = std::chrono::steady_clock::now();
    OpenAddressingHashTable<int, std::string> hashTable(10000);
    //std::unordered_set<int> openAddressHash;
    for (int i = 0; i < num; ++i) {
        //openAddressHash.insert(key);
        hashTable.insert(keys[i], "1");
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> openAddressHashDuration = end - start;
    return openAddressHashDuration.count();
}
//cuckoo哈希
double Insert_cuckoo(int num, int keys[]) {
    auto start = std::chrono::steady_clock::now();
    CuckooHashing ch(5000, 2, [](const int& value) { return value % 5000; }, [](const int& value) { return (value * 3 + 7) % 5000; });
    for (int i = 0; i < num; ++i) {
        ch.insert(keys[i]);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> cuckooHashDuration = end - start;
    //return cuckooHashDuration.count();
    return ch.getkick_count();
}
//新哈希
int Insert_hop(int num, int keys[]) {
    auto start = std::chrono::steady_clock::now();
    LR hk(0, 1667, 6, 0, 6);
    for (int i = 0; i < num; i++) {
        hk.insert(keys[i]);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> hop_kickHashDuration = end - start;
    return hop_kickHashDuration.count();
}
//各种哈希算法的
int cuckoo_kick(int num, int keys[]) {
    CuckooHashing ch(5000, 2, [](const int& value) { return value % 5000; }, [](const int& value) { return (value * 3 + 7) % 5000; });
    for (int i = 0; i < num; i++) {
        ch.insert(keys[i]);
    }
    return ch.getkick_count();
}
int cuckoo_conflict(int num, int keys[]) {
    CuckooHashing ch(5000, 2, [](const int& value) { return value % 5000; }, [](const int& value) { return (value * 3 + 7) % 5000; });
    for (int i = 0; i < num; i++) {
        ch.insert(keys[i]);
    }
    return ch.getconflict();
}
double cuckoo_memory(int num, int keys[]) {
    CuckooHashing ch(5000, 2, [](const int& value) { return value % 5000; }, [](const int& value) { return (value * 3 + 7) % 5000; });
    for (int i = 0; i < num; i++) {
        ch.insert(keys[i]);
    }
    return ch.getMemory_utilizationrate()/num;
}
int hop_kick(int num, int keys[]) {
    LR hk(0, 1667, 6, 0, 6);
    for (int i = 0; i < num; i++) {
        hk.insert(keys[i]);
    }
    return hk.getKick_count();
}
int hop_conflict(int num, int keys[]) {
    LR hk(0, 1667, 6, 0, 6);
    for (int i = 0; i < num; i++) {
        hk.insert(keys[i]);
    }
    return hk.getConflict();
}
double hop_memory(int num, int keys[]) {
    LR hk(0, 1667, 6, 0, 6);
    for (int i = 0; i < num; i++) {
        hk.insert(keys[i]);
    }
    return hk.getMemory_utilizationrate()/num;
}
int time33Hash(int num) {
    // 将整数转换为字符串
    std::string numStr = std::to_string(num);

    // 初始化哈希值为0
    int hashValue = 0;

    // 对每个字符应用Time33算法并累加结果
    for (char c : numStr) {
        int charCode = static_cast<int>(c);
        hashValue += charCode * 3;
    }

    return hashValue;
}
int main() {
    
    // 可以使用生成的结果来生成图表（这里仅作示例）
    double openAddress[10];
    double Chain[10];
    double Cuckoo[10];
    double Hop_kick[10];
    int cuckoo_kc[10];
    int hop_kc[10];
    std::vector<int> x = { 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 }; // x轴数据
    std::vector<double> y1(10); //  开放地址哈希冲突次数
    std::vector<int> y2(10); //  开放地址插入内存访问次数
    std::vector<int> y3(10); //  开放地址查找内存访问次数
    std::vector<double> y4(10); //  cuckoo哈希冲突次数
    std::vector<int> y5(10); //  cuckoo插入内存访问次数
    std::vector<int> y6(10); //  cuckoo查找内存访问次数
    std::vector<double> y7(10); //  hop_kick哈希冲突次数
    std::vector<int> y8(10); //  hop_kick插入内存访问次数
    std::vector<int> y9(10); //  hop_kick查找内存访问次数
    std::vector<double> y10(10); //  跳房子哈希冲突次数
    std::vector<int> y11(10); //  跳房子插入内存访问次数
    std::vector<int> y12(10); //  跳房子查找内存访问次数
    std::vector<int> y13(10); //  跳房子插入hash次数
    std::vector<int> y14(10); //  cuckoo插入hash次数
    std::vector<double> y15(10); //  开放地址内存利用率
    std::vector<double> y16(10); //  cuckoo内存利用率
    std::vector<double> y17(10); //  hopkick内存利用率
    std::vector<double> y18(10); //  hopscotch内存利用率
    for (int i = 0; i < 10; i++) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 1000000);

        // 生成随机键集合
        std::vector<int> keys1;
        for (int j = 0; j < x[i]; ++j) {
            keys1.push_back(dis(gen));
        }
        int* keys = &keys1[0];
        int find_size = (x[i] / 2);
        int insert_size = x[i];
        std::vector<int> sfind_keys;
        sfind_keys.resize(find_size);
        std::vector<int> usfind_keys;
        usfind_keys.resize(find_size);
        for (int k = 0, j = 0; k < find_size; k++) {
            sfind_keys[k] = keys[j];
            j += 2;
        }
        int x = 0;
        for (int j = 0; j < 100000; j++) {
            int judge = 1;
            for (int k = 0; k < insert_size; k++) {
                if (j == keys[k]) {
                    judge = 0;
                    break;
                }
            }
            if (judge == 1) {
                usfind_keys[x] = j;
                x++;
            }  
            if (x == find_size) {
                break;
            }
        }
        HashMap<int, int> hashMap(1000);
        //std::unordered_map<int, int> chainHash;
        for (int k = 0; k < insert_size; k++) {
            hashMap.insert(keys[k], 1);
          
        }
        int value;
        for (int k = 0; k < find_size; k++) {
            hashMap.search(usfind_keys[k],value);
        }
        // 测试开放地址哈希的性能
        OpenAddressingHashTable<int, std::string> hashTable(10000);
        //std::unordered_set<int> openAddressHash;
        for (int k = 0; k < insert_size; k++) {
            hashTable.insert(keys[k], "1");
        }
        string value2;
        for (int k = 0; k < find_size; k++) {
       
            hashTable.search(usfind_keys[k], value2);
        }
        // 测试 Cuckoo 哈希的性能
        CuckooHashing ch(5000, 6, [](const int& value) { return value % 5000; }, [](const int& value) { return (value * 3 + 7) % 5000; });
        for (int k = 0; k < insert_size; k++) {
            ch.insert(keys[k]);
        }
        for (int k = 0; k < find_size; k++) {
            
            ch.find(usfind_keys[k]);
        }
        // 测试 hopscotch 哈希的性能
        hopscotchHashTable<int, int> Hop(10000, 4);
        for (int k = 0; k < insert_size; k++) {
            Hop.insert(keys[k],1);
        }
        for (int k = 0; k < find_size; k++) {

            Hop.search(usfind_keys[k],value);
        }
        // 测试新哈希的性能
        LR hk(0, 5000, 2, 0, 6);
        for (int k = 0; k < insert_size; k++) {
            hk.insert(keys[k]);
        }
        for (int k = 0; k < find_size; k++) {
       
            hk.getFlowNum(sfind_keys[k]);
        }
        y1[i] = hashTable.getconflict();
        y2[i] = hashTable.getinsert_visit();
        y3[i] = hashTable.getfind_visit();
        y4[i] = ch.getconflict();
        y5[i] = ch.getinsert_visit();
        y6[i] = ch.getfind_visit();
        y7[i] = hk.getConflict();
        y8[i] = hk.getinsert_visit();
        y9[i] = hk.getfind_visit();
        y10[i] = Hop.getconflict();
        y11[i] = Hop.getinsert_visit();
        y12[i] = Hop.getfind_visit();
        y13[i] = Hop.gethash_count();
        y14[i] = ch.gethash_count();
        y15[i] = hashTable.getMemory_utilizationrate();
        y16[i] = ch.getMemory_utilizationrate();
        y17[i] = hk.getMemory_utilizationrate();
        y18[i] = Hop.getMemory_utilizationrate();

    }
    cout << "不同数量数据集下cuckoo和hop_kick的哈希冲突率与踢操作次数比较" << endl;
    cout << "项目数量: 1000  2000  3000  4000  5000  6000  7000  8000  9000  10000 " << endl;
    cout << "cuckoo_kick:";
    for (int j = 0; j < 10; j++) {
        cout << y1[j] << " ";
    }
    cout << endl;
    cout << "cuckoo_find:";
    for (int j = 0; j < 10; j++) {
        cout << y4[j] <<" ";
    }
    cout << endl;
    cout << "hop_kick:";
    for (int j = 0; j < 10; j++) {
        cout << y7[j] <<" ";
    }
    cout << endl;
    cout << "hop_conflict:";
    for (int j = 0; j < 10; j++) {
        cout << y10[j]<<" ";
    }
    cout << endl;
    cout << "hop_find:";
    for (int j = 0; j < 10; j++) {
        cout << y7[j] << " ";
    }
    cout << endl;
    cout << "hop_memory:";
    for (int j = 0; j < 10; j++) {
        cout << y8[j] << " ";
    }
    std::ofstream outputFile("c_k4.csv"); // 创建一个名为"data.csv"的文件

    //将数据写入CSV文件
    for (size_t i = 0; i < x.size(); ++i) {
        outputFile << x[i] << "," << y1[i] << "," << y2[i] << "," << y3[i] << "," << y4[i] << "," << y5[i] << "," << y6[i] << "," << y7[i] << "," << y8[i] << "," << y9[i] << "," << y10[i] << ","
            << y11[i] << "," << y12[i] << "," << y13[i] << "," << y14[i] << ",";
    }

    outputFile.close(); // 关闭文件

    return 0;
}
