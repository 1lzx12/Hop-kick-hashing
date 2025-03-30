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
        // ʹ�ü򵥵�ȡģ������Ϊ��ϣ����
        return static_cast<int>(key) % capacity;
    }

    bool isFull() const {
        return size >= capacity;
    }

    void rehash() {
        // ��������Ϊ��ǰ������
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
        bool isOccupied;  // ��Ǹ�λ���Ƿ�ռ��

        Node() : isOccupied(false) {}
    };

    std::vector<Node> data;  // �洢��ϣ�������
    int capacity;  // ��ϣ�������
    int size;  // ��ϣ��ǰ�洢��Ԫ������

    // ��ϣ������ʹ�ü򵥵�ȡģ����
    int hash(const KeyType& key) const {
        return static_cast<int>(key) % capacity;
    }

    // ����ϣ���Ƿ�����
    bool isFull() const {
        return size >= capacity;
    }

    // ���¹�ϣ���������������·�������Ԫ��
    void rehash() {
        capacity *= 2;  // ��������Ϊ��ǰ������
        size = 0;
        std::vector<Node> newData(capacity);

        for (const auto& node : data) {
            if (node.isOccupied) {
                insertNode(newData, node.key, node.value);
            }
        }

        data = std::move(newData);
    }

    // ��ָ���Ĺ�ϣ���в���һ���ڵ�
    void insertNode(std::vector<Node>& table, const KeyType& key, const ValueType& value) {
        int index = hash(key);

        // �����λ���ѱ�ռ�ã�������̽�⵽��һ��λ��
        while (table[index].isOccupied) {
            index = (index + 1) % capacity;
        }

        table[index].key = key;
        table[index].value = value;
        table[index].isOccupied = true;
        size++;  // ע�⣺�������size�Ĳ���Ӧ�÷��ڹ�����insert�����У���Ӧ��������
    }

public:
    // ���캯������ʼ����ϣ��������ʹ�С
    OpenAddressingHashTable(int initialCapacity = 10) : capacity(initialCapacity), size(0) {
        data.resize(capacity);
    }

    // ����һ����ֵ��
    void insert(const KeyType& key, const ValueType& value) {
        if (isFull()) {
            rehash();  // ������������Ƚ�������
        }

        insertNode(data, key, value);
        // size++; // Ӧ�ý�size�������ƶ���insertNode�ڲ���������������ȷ������ȷ�Ĵ����Ǳ�����������ӡ�
    }

    // �Ƴ�һ��Ԫ��
    void remove(const KeyType& key) {
        int index = hash(key);

        // ����̽��Ѱ��Ҫɾ���ļ�
        while (data[index].isOccupied) {
            if (data[index].key == key) {
                data[index].isOccupied = false;
                size--;
                return;
            }
            index = (index + 1) % capacity;
        }
    }

    // ����һ�������������Ƿ��ҵ��Լ���ֵ
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
    OpenAddressingHashTable<int, std::string> hashTable(5);  // ��ʼ��һ������Ϊ5�Ĺ�ϣ��

    // �����ֵ��
    hashTable.insert(1, "One");
    hashTable.insert(2, "Two");
    hashTable.insert(3, "Three");

    std::string value;
    // ������2������ӡ���Ӧ��ֵ
    if (hashTable.search(2, value)) {
        std::cout << "Value for key 2: " << value << std::endl;
    }
    else {
        std::cout << "Key 2 not found." << std::endl;
    }

    // �Ƴ���2
    hashTable.remove(2);

    // �ٴ�������2��ȷ�����ѱ��Ƴ�
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
        // ʹ�ü򵥵�ȡģ������Ϊ��ϣ����
        return static_cast<int>(key) % capacity;
    }

    bool isFull() const {
        return size >= capacity;
    }

    void rehash() {
        // ��������Ϊ��ǰ������
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