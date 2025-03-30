#pragma once
#include <list>
typedef unsigned long ULONG;
class LR
{
private:
	typedef struct Entry {
		int id;		//fid
		ULONG count;	//������
		int flag;	//��־λ

		Entry() : count(0),flag(0) {}

		//����=���������������
		inline Entry& operator=(const Entry& entry) {
			id = entry.id;
			count = entry.count;
			flag = entry.flag;
			return *this;
		}

		//�жϵ�ǰ���Ƿ�Ϊ��
		
	}Entry;
	ULONG counter, kick;					    //counter:��ͳ�Ƶ����ݰ�������kick�߲���ִ�д���
	Entry** entryTable;							//ͳ�Ʊ�
	ULONG ROW, COL;		                        //��ϣ���У�Ͱ�������У��ۣ���,����
	ULONG kick_t;                               //�߲�����ֵ
	int kick_count = 0;
	int conflict = 0;
	int visit_count = 0;
	int find_count = 0;
	int hash_count = 0;

public:
	LR(int Counter, int row, int col, int Kick, int Kick_t);
	virtual ~LR();
	void insert(int id);// �������
	ULONG getFlowNum(int id);
	int getFlowFlag(int id);
	int getKick_count();
	int getConflict();
	int getVisit_count();
	int getFind_count();
	int getHash_count();
	double getMemory_utilizationrate();
private:
	void getPositionRecord(int id, ULONG* index); //���¹�ϣ����洢λ�ã���index�洢������ѡͰ��λ��
	bool insertRecord(int id);//����������
	bool insertRecord_opt3(int id,int Count);
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

