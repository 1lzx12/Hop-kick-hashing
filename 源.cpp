#include "�¹�ϣ2.h"
#include <time.h>
#include <iostream>
#include <vector>
#include <iostream>
#include <chrono>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>
using namespace std;
LR::LR(int Counter, int row, int col, int Kick, int Kick_t)
{
	counter = Counter;
	ROW = row;
	COL = col;
	kick = Kick;
	kick_t = Kick_t;
	kick_count = 0;

	//hashTable = new Bucket * [ROW];
}

/*LR::~LR()
{
	for (ULONG i = 0; i < ROW; i++) {
		delete[] entryTable[i];
	}
	delete[] entryTable;
}*/

void LR::insert(int id)
{
	insertRecord(id);
}
//��ȡ�洢����Ŀ��Ϣ����������
ULONG LR::getFlowNum(int id)
{
	ULONG index[3];
	getPositionRecord(id, index);
	//��Ŀ�Ƿ��Ѿ����洢
	for (ULONG i = 0; i < 3; i++) {
		for (ULONG j = 0; j < COL; j++) {
			if (hashtable[index[i]].bucket[j].id == id) {
				return  hashtable[index[i]].bucket[j].count;
			}
		}
	}
	return 0;
}
int LR::getFlowFlag(int id)
{
	ULONG index[3];
	getPositionRecord(id, index);
	//��Ŀ�Ƿ��Ѿ����洢
	for (ULONG i = 0; i < 3; i++) {
		for (ULONG j = 0; j < COL; j++) {
			if (hashtable[index[i]].bucket[j].id == id){  
				return  hashtable[index[i]].bucket[j].flag;
			}
		}
	}
	return -2;
}
int LR::get_Bucketscount(int i)
{
	return hashtable[i].pos_Count;
}
int LR::getKick_count()
{
	return kick_count;
}
//���¹�ϣ����洢λ��,��index�洢������ѡͰ��λ��
void LR::getPositionRecord(int id, ULONG* index)
{
	index[0] = id % ROW;
	if ((index[0] + 1) % ROW == 0)
	{
		index[1] = 0;
	}
	else
	{
		index[1] = index[0] + 1;
	}
	if (index[0] == 0)
	{
		index[2] = ROW - 1;
	}
	else
	{
		index[2] = index[0] - 1;
	}
}

bool LR::insertRecord(int id)
{
	ULONG index[3];
	ULONG min = ULONG_MAX, row = 0, col = 0;
	int flag;		//�洢��ϣƫ����
	getPositionRecord(id, index);

	//��Ŀ�Ƿ��Ѿ����洢,�п�λ��洢
	for (ULONG i = 0; i < 3; i++) {
		for (ULONG j = 0; j < COL; j++) {
			if (hashtable[index[i]].bucket[j].id == id) {
				++hashtable[index[i]].bucket[j].count;
				return true;
			}
			else if (hashtable[index[i]].bucket[j].count == 0) {
				hashtable[index[i]].bucket[j].id = id;
				hashtable[index[i]].bucket[j].count = 1;
				if (i == 0) { hashtable[index[i]].bucket[j].flag = 0; }
				else if (i == 1) { hashtable[index[i]].bucket[j].flag = -1; hashtable[index[i]].pos_Count++; }
				else { hashtable[index[i]].bucket[j].flag = 1; hashtable[index[i]].neg_Count++; }
				return true;
			}
		}
	}
	srand((unsigned)time(NULL));
	int i = rand() % 2;
	int direction = 1;
	if (i == 0) {
		direction = -1;
		if (hashtable[index[i + 1]].neg_Count == 2) {
			direction = 1;
		}
	}
	if (i == 1) {
		direction = 1;
		if (hashtable[index[i + 1]].pos_Count == 2) {
			direction = -1;
		}
	}
	Kick_Operation_opt1(id, index[0], direction, 1, 0);
	return false;
}
bool LR::insertRecord_opt3(int id, int Count)
{

	ULONG index[3];
	ULONG min = ULONG_MAX, row = 0, col = 0;
	int flag;		//�洢��ϣƫ����
	getPositionRecord(id, index);

	//��Ŀ�Ƿ��Ѿ����洢,�п�λ��洢
	for (ULONG i = 0; i < 3; i++) {
		for (ULONG j = 0; j < COL; j++) {
			if (hashtable[index[i]].bucket[j].id == id) {
				hashtable[index[i]].bucket[j].count += Count;
				return true;
			}
			if (hashtable[index[i]].bucket[j].count < Count) {
				hashtable[index[i]].bucket[j].id = id;
				hashtable[index[i]].bucket[j].count = 1;
				if (i == 0) { hashtable[index[i]].bucket[j].flag = 0; }
				else if (i == 1) { hashtable[index[i]].bucket[j].flag = 1; }
				else { hashtable[index[i]].bucket[j].flag = -1; }
				return true;
			}
			else {
				srand((unsigned)time(NULL));
				int i = rand() % 2;
				int direction = 0;
				if (i == 0) direction = -1;
				if (i == 1) direction = 1;
				Kick_Operation_opt1(id, index[0], direction, 1, 0);
			}
		}
	}
	return false;
}
/*bool LR::replace(const FlowID& fid, ULONG row, ULONG col, int flag)
{
	entryTable[row][col].fid = fid;
	entryTable[row][col].count = 1;
	entryTable[row][col].flag = flag;
	return true;
}*/

/*bool LR::replace(const FlowID& fid, ULONG row, ULONG col, ULONG count, int flag)
{
	entryTable[row][col].fid = fid;
	entryTable[row][col].count = count + 1;
	entryTable[row][col].flag = flag;
	return true;
}*/

//fid:��Ŀid��index��Ŀ��洢λ�ã�direction���߷���count����Ŀ��Ϣ����������flag��ƫ����
bool LR::Kick_Operation(int id, ULONG index, int direction, ULONG count, int flag)         //�����汾���߲���
{
	kick++;
	kick_count++;
	ULONG row = 0, col = 0, min = ULONG_MAX, i = 0;
	//replace_�����ݴ���޳�����Ŀ
	int replace_id;
	ULONG replace_count = 0;
	int replace_flag = 0;
	int replace_direction = direction;
	//����direction�ҳ�Ŀ���߼�����Ͱ
	if (direction == 1)
	{
		if (index == 0)
			row = ROW - 1;
		else
			row = index - direction;
	}
	else if (direction == -1)
	{
		if ((index + 1) % ROW == 0)
			row = 0;
		else
			row = index - direction;
	}
	//�����Ƿ���ڿ�λ���п�λֱ�Ӳ��� hashtable[row].bucket[i]
	for (i = 0; i < COL; i++) {
		if (hashtable[row].bucket[i].count == 0) {
			hashtable[row].bucket[i].id = id;
			hashtable[row].bucket[i].count = count;
			hashtable[row].bucket[i].flag = flag + direction;
			if ((flag + direction) == 1) {
				hashtable[row].pos_Count++;
			}
			if ((flag + direction) == -1) {
				hashtable[row].neg_Count++;
			}
			kick = 0;
			return true;
		}
	}
	//�����ڿ�λ��ѡ��һ��ƫ����Ϊ0����Ŀ����
	if (kick <= kick_t) {                //�߲�������û������ֵ������ִ��ֱ�����ֿ�λ
		for (ULONG i = 0; i < COL; i++)
		{
			if (hashtable[row].bucket[i].flag == 0) {
				replace_id = hashtable[row].bucket[i].id;
				replace_count = hashtable[row].bucket[i].count;
				replace_flag = hashtable[row].bucket[i].flag;
				hashtable[row].bucket[i].id = id;
				hashtable[row].bucket[i].count = count;
				hashtable[row].bucket[i].flag = flag + direction;
				if ((flag + direction) == 1) {
					hashtable[row].pos_Count++;
				}
				if ((flag + direction) == -1) {
					hashtable[row].neg_Count++;
				}
				return Kick_Operation(replace_id, row, replace_direction, replace_count, replace_flag);
			}
		}
		//�߲�������û������ֵ�����Ѳ�����������������û��ƫ����Ϊ0����Ŀ������Ŀ��������߲�������������Ŀ������ʧ��
		kick = 0;
		return false;
	}
	//�߲�������������ֵ������ʧ��
	else {
		kick = 0;
		return false;
	}
}
//����߲����ĵ�һ���Ż�����������Ͱ�п���������ƫ����Ϊ-1����Ŀ����������Ͱ��������ƫ����Ϊ+1����Ŀ
bool LR::Kick_Operation_opt1(int id, ULONG index, int direction, ULONG count, int flag)
{
	kick++;
	kick_count++;
	ULONG row = 0, col = 0, min = ULONG_MAX, i = 0;
	//replace_�����ݴ���޳�����Ŀ
	int replace_id;
	ULONG replace_count = 0;
	int replace_flag = 0;
	int replace_direction = direction;
	//����direction�ҳ�Ŀ���߼�����Ͱ
	if (direction == 1)
	{
		if (index == 0)
			row = ROW - 1;
		else
			row = index - direction;
	}
	else if (direction == -1)
	{
		if ((index + 1) % ROW == 0)
			row = 0;
		else
			row = index - direction;
	}
	//�����Ƿ���ڿ�λ���п�λֱ�Ӳ��� 
	for (i = 0; i < COL; i++) {
		if (hashtable[row].bucket[i].count == 0) {
			hashtable[row].bucket[i].id = id;
			hashtable[row].bucket[i].count = count;
			hashtable[row].bucket[i].flag = flag + direction;
			if ((flag + direction) == 1) {
				hashtable[row].pos_Count++;
			}
			if ((flag + direction) == -1) {
				hashtable[row].neg_Count++;
			}
			kick = 0;
			return true;
		}
	}
	//ѡ��һ��ƫ������Ϊ0����Ŀ�߻���ӳ��λ��
	if (kick <= kick_t) {                     //�߲�������û������ֵ������ִ��ֱ�����ֿ�λ
		if (direction == -1) {
			for (ULONG i = 0; i < COL; i++) {
				if (hashtable[row].bucket[i].flag == 1) {
					replace_id = hashtable[row].bucket[i].id;
					replace_count = hashtable[row].bucket[i].count;
					replace_flag = hashtable[row].bucket[i].flag;
					hashtable[row].bucket[i].id = id;
					hashtable[row].bucket[i].count = count;
					hashtable[row].bucket[i].flag = flag + direction;
					hashtable[row].pos_Count--;
					return Kick_Operation_opt1(replace_id, row, replace_direction, replace_count, replace_flag);
				}
			}
			//�߲�������û������ֵ�����ڵ�һ���Ż��������Ѳ�����������������û��ƫ����Ϊ0����Ŀ������Ŀ��ת�����û����汾���߲���
			return Kick_Operation(id, index, direction, count, flag);
		}
		if (direction == 1) {
			for (ULONG i = 0; i < COL; i++) {
				if (hashtable[row].bucket[i].flag == -1) {
					replace_id = hashtable[row].bucket[i].id;
					replace_count = hashtable[row].bucket[i].count;
					replace_flag = hashtable[row].bucket[i].flag;
					hashtable[row].bucket[i].id = id;
					hashtable[row].bucket[i].count = count;
					hashtable[row].bucket[i].flag = flag + direction;
					hashtable[row].neg_Count--;
					return Kick_Operation_opt1(replace_id, row, replace_direction, replace_count, replace_flag);
				}
			}
			return Kick_Operation(id, index, direction, count, flag);
		}
	}
	//�߲�������������ֵ������ʧ��
	else {
		kick = 0;
		return false;
	}
}
/*����߲����ĵ�2���Ż�������Ȩ�����Ȳ��ԣ����߲����н�������Ϊ�ķ������ϴ��δ��������������Ŀ�洢����ӳ��Ͱ�У�
�Լ��ٲ���ͷ��ʵ�ʱ�临�Ӷȡ�����������һЩ��̫��Ҫ����Ŀ�����ǿ����ڵ�һ���߲���ʱ�ͽ��䶪�������ٶ�����߲�����*/
/*bool LR::Kick_Operation_opt2(int id, ULONG index, int direction, ULONG count, int flag)
{
	kick++;
	ULONG row = 0, col = 0, min = ULONG_MAX, i = 0;
	//replace_�����ݴ���޳�����Ŀ
	int replace_id;
	ULONG replace_count = 0;
	int replace_flag = 0;
	int replace_direction = direction;
	//����direction�ҳ�Ŀ���߼�����Ͱ
	if (direction == 1)
	{
		if (index == 0)
			row = ROW - 1;
		else
			row = index - direction;
	}
	else if (direction == -1)
	{
		if ((index + 1) % ROW == 0)
			row = 0;
		else
			row = index - direction;
	}
	for (i = 0; i < COL; i++) {
		if (hashtable[row].bucket[i].count == 0) {
			hashtable[row].bucket[i].id = id;
			hashtable[row].bucket[i].count = count;
			hashtable[row].bucket[i].flag = flag + direction;
			kick = 0;
			return true;
		}
	}
	//����Ȩ�����Ȳ��Խ����߲���
	if (kick <= kick_t) {                     //�߲�������û������ֵ������ִ��ֱ�����ֿ�λ
		if (direction == -1) {                //��������Ͱ��
			int count = 0, pos = -1;
			for (ULONG i = 0; i < COL; i++)   //����Ȩ����������ƫ����Ϊ1����Ŀ
			{
				if ((hashtable[row].bucket[i].count > count) && (hashtable[row].bucket[i].flag == 1)) {
					count = hashtable[row].bucket[i].count;
					pos = i;
				}
			}
			if (pos != -1) {                  //�ҵ��˷�������������Ŀ,����ִ���Ż�2���߲���
				replace_id = hashtable[row].bucket[i].id;
				replace_count = hashtable[row].bucket[i].count;
				replace_flag = hashtable[row].bucket[i].flag;
				hashtable[row].bucket[i].id = id;
				hashtable[row].bucket[i].count = count;
				hashtable[row].bucket[i].flag = flag + direction;
				return Kick_Operation_opt2(replace_id, row, replace_direction, replace_count, replace_flag);
			}
			else {                             //û�ҵ���������������Ŀ,ת��ִ���Ż�1���߲���
				return Kick_Operation_opt1(id, index, direction, count, flag);
			}
		}
		else if (direction == 1) {             //��������Ͱ��
			int count = 0, pos = 0;
			for (ULONG i = 0; i < COL; i++)
			{
				if ((hashtable[row].bucket[i].count > count) && (hashtable[row].bucket[i].flag == -1)) {
					count = hashtable[row].bucket[i].count;
					pos = i;
				}
			}
			if (pos != -1) {
				replace_id = hashtable[row].bucket[i].id;
				replace_count = hashtable[row].bucket[i].count;
				replace_flag = hashtable[row].bucket[i].flag;
				hashtable[row].bucket[i].id = id;
				hashtable[row].bucket[i].count = count;
				hashtable[row].bucket[i].flag = flag + direction;
				return Kick_Operation_opt2(replace_id, row, replace_direction, replace_count, replace_flag);
			}
			else {
				return Kick_Operation_opt1(id, index, direction, count, flag);
			};
		}
	}
	//�߲�������������ֵ������ʧ��
	else {
		kick = 0;
		return false;
	}
}*/
int main() {
	LR hk(0, 5000, 2, 0, 8);
	/*// ���������Ŀ����
	// �����������
	std::srand(static_cast<unsigned int>(std::time(0)));
	for (int i = 0; i < 10000; ++i) {
		int randomNum = std::rand() % 300;  // ����0��99֮��������
		hk.insert(randomNum);
	}
	cout << "����ɹ�" << endl;
	for (int i = 0; i < 300; ++i) {
		cout << hk.getFlowNum(i) << " " << hk.getFlowFlag(i) << "   ";
		if ((i+1) % 8 == 0) {
			cout << endl;
		}
	}*/
	hk.insert(5);
	hk.insert(15);
	hk.insert(25);
	hk.insert(35);
	hk.insert(3);
	hk.insert(13);
	hk.insert(23);
	hk.insert(4);
	hk.insert(65);
	cout << "����ɹ�" << endl;
	cout << hk.getFlowFlag(5);
	cout << hk.getFlowFlag(15);
	cout << hk.getFlowFlag(25);
	cout << hk.getFlowFlag(35);
	cout << hk.getFlowFlag(3);
	cout << hk.getFlowFlag(13);
	cout << hk.getFlowFlag(23);
	cout << hk.getFlowFlag(4);
	cout << hk.getFlowFlag(65) << endl;
	// ���� �� ��ϣ������
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 1000000);

	// �������������
	std::vector<int> keys;
	for (int i = 0; i < 10000; ++i) {
		keys.push_back(dis(gen));
	}
	auto start = std::chrono::steady_clock::now();

	for (const auto& key : keys) {
		hk.insert(key);
	}
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> cuckooHashDuration = end - start;
	cout << hk.getKick_count() << endl;
	std::cout << "Cuckoo Hashing Time: " << cuckooHashDuration.count() << " seconds" << std::endl;
	return 0;
}
