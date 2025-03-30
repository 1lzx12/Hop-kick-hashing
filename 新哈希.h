#pragma once
#include <list>
typedef unsigned long ULONG;
class LR
{
private:
	typedef struct Entry {
		int id;		//fid
		ULONG count;	//计数器
		int flag;	//标志位

		Entry() : count(0),flag(0) {}

		//重载=运算符，拷贝函数
		inline Entry& operator=(const Entry& entry) {
			id = entry.id;
			count = entry.count;
			flag = entry.flag;
			return *this;
		}

		//判断当前项是否为空
		
	}Entry;
	ULONG counter, kick;					    //counter:已统计的数据包个数，kick踢操作执行次数
	Entry** entryTable;							//统计表
	ULONG ROW, COL;		                        //哈希表行（桶）数，列（槽）数,段数
	ULONG kick_t;                               //踢操作阈值
	int kick_count = 0;
	int conflict = 0;
	int visit_count = 0;
	int find_count = 0;
	int hash_count = 0;

public:
	LR(int Counter, int row, int col, int Kick, int Kick_t);
	virtual ~LR();
	void insert(int id);// 插入操作
	ULONG getFlowNum(int id);
	int getFlowFlag(int id);
	int getKick_count();
	int getConflict();
	int getVisit_count();
	int getFind_count();
	int getHash_count();
	double getMemory_utilizationrate();
private:
	void getPositionRecord(int id, ULONG* index); //用新哈希计算存储位置，用index存储三个候选桶的位置
	bool insertRecord(int id);//具体插入操作
	bool insertRecord_opt3(int id,int Count);
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

