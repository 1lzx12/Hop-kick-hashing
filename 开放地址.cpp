/*#include <iostream>
#include <vector>

template <typename KeyType, typename ValueType>
class OpenAddressingHashTable {
private:
    struct Node {
        KeyType key;
        ValueType value;
        bool isOccupied;

        Node() : isOccupied(false) {}
    };

    std::vector<Node> data;
    int capacity;
    int size;

    int hash(const KeyType& key) const {
        // 使用简单的取模运算作为哈希函数
        return static_cast<int>(key) % capacity;
    }

    bool isFull() const {
        return size >= capacity;
    }

    void rehash() {
        // 扩大容量为当前的两倍
        capacity *= 2;
        size = 0;
        std::vector<Node> newData(capacity);

        for (const auto& node : data) {
            if (node.isOccupied) {
                insertNode(newData, node.key, node.value);
            }
        }

        data = std::move(newData);
    }

    void insertNode(std::vector<Node>& table, const KeyType& key, const ValueType& value) {
        int index = hash(key);

        while (table[index].isOccupied) {
            index = (index + 1) % capacity;
        }

        table[index].key = key;
        table[index].value = value;
        table[index].isOccupied = true;
    }

public:
    OpenAddressingHashTable(int initialCapacity = 10) : capacity(initialCapacity), size(0) {
        data.resize(capacity);
    }

    void insert(const KeyType& key, const ValueType& value) {
        if (isFull()) {
            rehash();
        }

        insertNode(data, key, value);
        size++;
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

    bool search(const KeyType& key, ValueType& value) const {
        int index = hash(key);

        while (data[index].isOccupied) {
            if (data[index].key == key) {
                value = data[index].value;
                return true;
            }
            index = (index + 1) % capacity;
        }

        return false;
    }
};

int main() {
    OpenAddressingHashTable<int, std::string> hashTable(5);

    hashTable.insert(1, "One");
    hashTable.insert(2, "Two");
    hashTable.insert(3, "Three");

    std::string value;
    if (hashTable.search(2, value)) {
        std::cout << "Value for key 2: " << value << std::endl;
    }
    else {
        std::cout << "Key 2 not found." << std::endl;
    }

    hashTable.remove(2);

    if (hashTable.search(2, value)) {
        std::cout << "Value for key 2: " << value << std::endl;
    }
    else {
        std::cout << "Key 2 not found." << std::endl;
    }

    return 0;
}*/
#include <iostream>
#include <vector>

template <typename KeyType, typename ValueType>
class OpenAddressingHashTable {
private:
    struct Node {
        KeyType key;
        ValueType value;
        bool isOccupied;  // 标记该位置是否被占用

        Node() : isOccupied(false) {}
    };

    std::vector<Node> data;  // 存储哈希表的数组
    int capacity;  // 哈希表的容量
    int size;  // 哈希表当前存储的元素数量

    // 哈希函数，使用简单的取模运算
    int hash(const KeyType& key) const {
        return static_cast<int>(key) % capacity;
    }

    // 检查哈希表是否已满
    bool isFull() const {
        return size >= capacity;
    }

    // 重新哈希，扩大容量并重新分配所有元素
    void rehash() {
        capacity *= 2;  // 扩大容量为当前的两倍
        size = 0;
        std::vector<Node> newData(capacity);

        for (const auto& node : data) {
            if (node.isOccupied) {
                insertNode(newData, node.key, node.value);
            }
        }

        data = std::move(newData);
    }

    // 在指定的哈希表中插入一个节点
    void insertNode(std::vector<Node>& table, const KeyType& key, const ValueType& value) {
        int index = hash(key);

        // 如果该位置已被占用，则线性探测到下一个位置
        while (table[index].isOccupied) {
            index = (index + 1) % capacity;
        }

        table[index].key = key;
        table[index].value = value;
        table[index].isOccupied = true;
        size++;  // 注意：这个增加size的操作应该放在公共的insert方法中，不应该在这里
    }

public:
    // 构造函数，初始化哈希表的容量和大小
    OpenAddressingHashTable(int initialCapacity = 10) : capacity(initialCapacity), size(0) {
        data.resize(capacity);
    }

    // 插入一个键值对
    void insert(const KeyType& key, const ValueType& value) {
        if (isFull()) {
            rehash();  // 如果表已满，先进行扩容
        }

        insertNode(data, key, value);
        // size++; // 应该将size的增加移动到insertNode内部，或者在这里正确处理。正确的处理是保留这里的增加。
    }

    // 移除一个元素
    void remove(const KeyType& key) {
        int index = hash(key);

        // 线性探测寻找要删除的键
        while (data[index].isOccupied) {
            if (data[index].key == key) {
                data[index].isOccupied = false;
                size--;
                return;
            }
            index = (index + 1) % capacity;
        }
    }

    // 查找一个键，并返回是否找到以及其值
    bool search(const KeyType& key, ValueType& value) const {
        int index = hash(key);

        while (data[index].isOccupied) {
            if (data[index].key == key) {
                value = data[index].value;
                return true;
            }
            index = (index + 1) % capacity;
        }

        return false;
    }
};

int main() {
    OpenAddressingHashTable<int, std::string> hashTable(5);  // 初始化一个容量为5的哈希表

    // 插入键值对
    hashTable.insert(1, "One");
    hashTable.insert(2, "Two");
    hashTable.insert(3, "Three");

    std::string value;
    // 搜索键2，并打印其对应的值
    if (hashTable.search(2, value)) {
        std::cout << "Value for key 2: " << value << std::endl;
    }
    else {
        std::cout << "Key 2 not found." << std::endl;
    }

    // 移除键2
    hashTable.remove(2);

    // 再次搜索键2，确认其已被移除
    if (hashTable.search(2, value)) {
        std::cout << "Value for key 2: " << value << std::endl;
    }
    else {
        std::cout << "Key 2 not found." << std::endl;
    }

    return 0;
}

/*#pragma once
#include <iostream>
#include <vector>

template <typename KeyType, typename ValueType>
class OpenAddressingHashTable {
private:
    struct Node {
        KeyType key;
        ValueType value;
        bool isOccupied;

        Node() : isOccupied(false) {}
    };

    std::vector<Node> data;
    int capacity;
    int size;

    int hash(const KeyType& key) const {
        // 使用简单的取模运算作为哈希函数
        return static_cast<int>(key) % capacity;
    }

    bool isFull() const {
        return size >= capacity;
    }

    void rehash() {
        // 扩大容量为当前的两倍
        capacity *= 2;
        size = 0;
        std::vector<Node> newData(capacity);

        for (const auto& node : data) {
            if (node.isOccupied) {
                insertNode(newData, node.key, node.value);
            }
        }

        data = std::move(newData);
    }

    void insertNode(std::vector<Node>& table, const KeyType& key, const ValueType& value) {
        int index = hash(key);

        while (table[index].isOccupied) {
            index = (index + 1) % capacity;
        }

        table[index].key = key;
        table[index].value = value;
        table[index].isOccupied = true;
    }
    void insertNode(std::vector<Node>& table, const KeyType& key) {
        int index = hash(key);

        while (table[index].isOccupied) {
            index = (index + 1) % capacity;
        }

        table[index].key = key;
        //table[index].value = value;
        table[index].value += 1;
        table[index].isOccupied = true;
    }

public:
    OpenAddressingHashTable(int initialCapacity = 10000) : capacity(initialCapacity), size(0) {
        data.resize(capacity);
    }

    void insert(const KeyType& key, const ValueType& value) {
        if (isFull()) {
            rehash();
        }

        insertNode(data, key, value);
        size++;
    }
    void insert(const KeyType& key) {
        if (isFull()) {
            rehash();
        }

        insertNode(data, key);
        size++;
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

    bool search(const KeyType& key, ValueType& value) const {
        int index = hash(key);

        while (data[index].isOccupied) {
            if (data[index].key == key) {
                value = data[index].value;
                return true;
            }
            index = (index + 1) % capacity;
        }

        return false;
    }
};

int main() {
    OpenAddressingHashTable<int, std::string> hashTable(5);

    hashTable.insert(1, "One");
    hashTable.insert(2, "Two");
    hashTable.insert(3, "Three");

    std::string value;
    if (hashTable.search(2, value)) {
        std::cout << "Value for key 2: " << value << std::endl;
    }
    else {
        std::cout << "Key 2 not found." << std::endl;
    }

    hashTable.remove(2);

    if (hashTable.search(2, value)) {
        std::cout << "Value for key 2: " << value << std::endl;
    }
    else {
        std::cout << "Key 2 not found." << std::endl;
    }

    return 0;
}*/