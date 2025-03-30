
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

	//����=���������������
	inline Item& operator=(const Item& entry) {
		id = entry.id;
		count = entry.count;
		flag = entry.flag;
		return *this;
	}
};

class Bucket {
public:
	std::vector<Item> items; // �洢��Ŀ��Ϣ������
	int pos_Count; // Ͱ��ƫ����Ϊ+1����Ŀ����
	int neg_Count; // Ͱ��ƫ����Ϊ-1����Ŀ����

	// ���캯������ʼ����Ŀ����Ϊ 0
	Bucket() : pos_Count(0),neg_Count(0) {
		// ע�⣬���������ﲻԤ�ȷ���ռ�� items����Ϊ��ȡ���� m ��ֵ
	}
};

class HashTable {
private:
	int n; // Ͱ������
	std::vector<Bucket> buckets; // �������洢����Ͱ
	ULONG counter, kick;					    //counter:��ͳ�Ƶ����ݰ�������kick�߲���ִ�д���
	Item** entryTable;							//ͳ�Ʊ�
	ULONG ROW, COL;		                        //��ϣ���У�Ͱ�������У��ۣ���,����
	ULONG kick_t;                               //�߲�����ֵ
	int kick_count;

public:
	HashTable(int n_buckets) : n(n_buckets) {
		buckets.resize(n);
	}

	// Hash ����������ȷ����ĿӦ�÷����ĸ�Ͱ
	int hash(int key) {
		return key % n;
	}

	// ���ϣ���в�����Ŀ
	void insertItem(int id, int offset, int count) {
		Item newItem = { id, offset, count };
		int index = hash(id);
		buckets[index].addItem(newItem);
	}

	// �����������������ܣ�����������ɾ����Ŀ

};*/


class LR
{
private:
	typedef struct Item {
		int id;
		int flag;
		ULONG count;
		Item() : count(0), flag(0) {}

		//����=���������������
		inline Item& operator=(const Item& entry) {
			id = entry.id;
			count = entry.count;
			flag = entry.flag;
			return *this;
		}
	};
	typedef struct Bucket {

		Item bucket[2]; // �洢��Ŀ��Ϣ������
		int pos_Count; // Ͱ��ƫ����Ϊ+1����Ŀ����
		int neg_Count; // Ͱ��ƫ����Ϊ-1����Ŀ����
		// ���캯������ʼ����Ŀ����Ϊ 0
		Bucket() : pos_Count(0), neg_Count(0) {
			// ע�⣬���������ﲻԤ�ȷ���ռ�� items����Ϊ��ȡ���� m ��ֵ
		}
	}Bucket;
	Bucket hashtable[5000];
	ULONG counter, kick;					    //counter:��ͳ�Ƶ����ݰ�������kick�߲���ִ�д���
	Bucket* hashTable;							//ͳ�Ʊ�
	ULONG ROW, COL;		                        //��ϣ���У�Ͱ�������У��ۣ���,����
	ULONG kick_t;                               //�߲�����ֵ
	int kick_count;

public:
	LR(int Counter, int row, int col, int Kick, int Kick_t);
	//virtual ~LR();
	void insert(int id);// �������
	ULONG getFlowNum(int id);
	int getFlowFlag(int id);
	int getKick_count();
	int get_Bucketscount(int i);
private:
	void getPositionRecord(int id, ULONG* index); //���¹�ϣ����洢λ�ã���index�洢������ѡͰ��λ��
	bool insertRecord(int id);//����������
	bool insertRecord_opt3(int id, int Count);
	//bool replace(int id, ULONG row, ULONG col, int flag);//�滻����
	//bool replace(int id, ULONG row, ULONG col, ULONG count, int flag);//�����滻����
	//bool reportRecord();

	/// <param name="id">���ߵ���ID</param>
	/// <param name="index">�������ڼ����е���</param>
	/// <param name="direction">���ߵķ���</param>
	/// <param name="count">���ߵ�����С</param>
	/// <param name="flag">���ߵ�����־λ</param>
	bool Kick_Operation(int id, ULONG index, int direction, ULONG count, int flag);//�߲���
	bool Kick_Operation_opt1(int id, ULONG index, int direction, ULONG count, int flag);//�Ż�1���߲���
	bool Kick_Operation_opt2(int id, ULONG index, int direction, ULONG count, int flag);//�Ż�2���߲���
};

