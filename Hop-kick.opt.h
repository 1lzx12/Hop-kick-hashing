
#pragma once
#include <list>
#include <iostream>
#include <vector>
typedef unsigned long ULONG;


/*struct Item {
	int id;
	int flag;
	int count;
	Item() : count(0), flag(0) {}

	//重载=运算符，拷贝函数
	inline Item& operator=(const Item& entry) {
		id = entry.id;
		count = entry.count;
		flag = entry.flag;
		return *this;
	}
};

class Bucket {
public:
	std::vector<Item> items; // 存储项目信息的向量
	int pos_Count; // 桶中偏移量为+1的项目数量
	int neg_Count; // 桶中偏移量为-1的项目数量

	// 构造函数，初始化项目数量为 0
	Bucket() : pos_Count(0),neg_Count(0) {
		// 注意，我们在这里不预先分配空间给 items，因为这取决于 m 的值
	}
};

class HashTable {
private:
	int n; // 桶的数量
	std::vector<Bucket> buckets; // 向量，存储所有桶
	ULONG counter, kick;					    //counter:已统计的数据包个数，kick踢操作执行次数
	Item** entryTable;							//统计表
	ULONG ROW, COL;		                        //哈希表行（桶）数，列（槽）数,段数
	ULONG kick_t;                               //踢操作阈值
	int kick_count;

public:
	HashTable(int n_buckets) : n(n_buckets) {
		buckets.resize(n);
	}

	// Hash 函数，用于确定项目应该放入哪个桶
	int hash(int key) {
		return key % n;
	}

	// 向哈希表中插入项目
	void insertItem(int id, int offset, int count) {
		Item newItem = { id, offset, count };
		int index = hash(id);
		buckets[index].addItem(newItem);
	}

	// 这里可以添加其他功能，比如搜索和删除项目

};*/


class LR
{
private:
	typedef struct Item {
		int id;
		int flag;
		ULONG count;
		Item() : count(0), flag(0) {}

		//重载=运算符，拷贝函数
		inline Item& operator=(const Item& entry) {
			id = entry.id;
			count = entry.count;
			flag = entry.flag;
			return *this;
		}
	};
	typedef struct Bucket {

		Item bucket[2]; // 存储项目信息的向量
		int pos_Count; // 桶中偏移量为+1的项目数量
		int neg_Count; // 桶中偏移量为-1的项目数量
		// 构造函数，初始化项目数量为 0
		Bucket() : pos_Count(0), neg_Count(0) {
			// 注意，我们在这里不预先分配空间给 items，因为这取决于 m 的值
		}
	}Bucket;
	Bucket hashtable[5000];
	ULONG counter, kick;					    //counter:已统计的数据包个数，kick踢操作执行次数
	Bucket* hashTable;							//统计表
	ULONG ROW, COL;		                        //哈希表行（桶）数，列（槽）数,段数
	ULONG kick_t;                               //踢操作阈值
	int kick_count;

public:
	LR(int Counter, int row, int col, int Kick, int Kick_t);
	//virtual ~LR();
	void insert(int id);// 插入操作
	ULONG getFlowNum(int id);
	int getFlowFlag(int id);
	int getKick_count();
	int get_Bucketscount(int i);
private:
	void getPositionRecord(int id, ULONG* index); //用新哈希计算存储位置，用index存储三个候选桶的位置
	bool insertRecord(int id);//具体插入操作
	bool insertRecord_opt3(int id, int Count);
	//bool replace(int id, ULONG row, ULONG col, int flag);//替换操作
	//bool replace(int id, ULONG row, ULONG col, ULONG count, int flag);//重载替换操作
	//bool reportRecord();

	/// <param name="id">被踢的流ID</param>
	/// <param name="index">被踢流在监测表中的行</param>
	/// <param name="direction">被踢的方向</param>
	/// <param name="count">被踢的流大小</param>
	/// <param name="flag">被踢的流标志位</param>
	bool Kick_Operation(int id, ULONG index, int direction, ULONG count, int flag);//踢操作
	bool Kick_Operation_opt1(int id, ULONG index, int direction, ULONG count, int flag);//优化1的踢操作
	bool Kick_Operation_opt2(int id, ULONG index, int direction, ULONG count, int flag);//优化2的踢操作
};

