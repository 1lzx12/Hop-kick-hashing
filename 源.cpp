#include "新哈希2.h"
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
//获取存储的项目信息，例如数量
ULONG LR::getFlowNum(int id)
{
	ULONG index[3];
	getPositionRecord(id, index);
	//项目是否已经被存储
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
	//项目是否已经被存储
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
//用新哈希计算存储位置,用index存储三个候选桶的位置
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
	int flag;		//存储哈希偏移量
	getPositionRecord(id, index);

	//项目是否已经被存储,有空位则存储
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
	int flag;		//存储哈希偏移量
	getPositionRecord(id, index);

	//项目是否已经被存储,有空位则存储
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

//fid:项目id，index：目标存储位置，direction：踢方向，count：项目信息（数量），flag：偏移量
bool LR::Kick_Operation(int id, ULONG index, int direction, ULONG count, int flag)         //基础版本的踢操作
{
	kick++;
	kick_count++;
	ULONG row = 0, col = 0, min = ULONG_MAX, i = 0;
	//replace_用于暂存待剔出的项目
	int replace_id;
	ULONG replace_count = 0;
	int replace_flag = 0;
	int replace_direction = direction;
	//根据direction找出目标逻辑相邻桶
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
	//查找是否存在空位，有空位直接插入 hashtable[row].bucket[i]
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
	//不存在空位，选择一条偏移量为0的项目踢走
	if (kick <= kick_t) {                //踢操作次数没到达阈值，反复执行直到出现空位
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
		//踢操作次数没到达阈值，但已不存在满足踢条件（没有偏移量为0的项目）的项目，则放弃踢操作，丢弃该项目，插入失败
		kick = 0;
		return false;
	}
	//踢操作次数到达阈值，插入失败
	else {
		kick = 0;
		return false;
	}
}
//针对踢操作的第一个优化：在上相邻桶中可以优先踢偏移量为-1的项目，在下相邻桶中优先踢偏移量为+1的项目
bool LR::Kick_Operation_opt1(int id, ULONG index, int direction, ULONG count, int flag)
{
	kick++;
	kick_count++;
	ULONG row = 0, col = 0, min = ULONG_MAX, i = 0;
	//replace_用于暂存待剔出的项目
	int replace_id;
	ULONG replace_count = 0;
	int replace_flag = 0;
	int replace_direction = direction;
	//根据direction找出目标逻辑相邻桶
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
	//查找是否存在空位，有空位直接插入 
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
	//选择一条偏移量不为0的项目踢回其映射位置
	if (kick <= kick_t) {                     //踢操作次数没到达阈值，反复执行直到出现空位
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
			//踢操作次数没到达阈值，但在第一个优化条件下已不存在满足踢条件（没有偏移量为0的项目）的项目，转而采用基础版本的踢操作
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
	//踢操作次数到达阈值，插入失败
	else {
		kick = 0;
		return false;
	}
}
/*针对踢操作的第2个优化：采用权重优先策略，在踢操作中将我们认为的访问量较大或未来插入次数多的项目存储在其映射桶中，
以减少插入和访问的时间复杂度。甚至，对于一些不太重要的项目，我们可以在第一次踢操作时就将其丢弃，减少多余的踢操作。*/
/*bool LR::Kick_Operation_opt2(int id, ULONG index, int direction, ULONG count, int flag)
{
	kick++;
	ULONG row = 0, col = 0, min = ULONG_MAX, i = 0;
	//replace_用于暂存待剔出的项目
	int replace_id;
	ULONG replace_count = 0;
	int replace_flag = 0;
	int replace_direction = direction;
	//根据direction找出目标逻辑相邻桶
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
	//采用权重优先策略进行踢操作
	if (kick <= kick_t) {                     //踢操作次数没到达阈值，反复执行直到出现空位
		if (direction == -1) {                //在下相邻桶中
			int count = 0, pos = -1;
			for (ULONG i = 0; i < COL; i++)   //查找权重最大的流且偏移量为1的项目
			{
				if ((hashtable[row].bucket[i].count > count) && (hashtable[row].bucket[i].flag == 1)) {
					count = hashtable[row].bucket[i].count;
					pos = i;
				}
			}
			if (pos != -1) {                  //找到了符合踢条件的项目,继续执行优化2的踢操作
				replace_id = hashtable[row].bucket[i].id;
				replace_count = hashtable[row].bucket[i].count;
				replace_flag = hashtable[row].bucket[i].flag;
				hashtable[row].bucket[i].id = id;
				hashtable[row].bucket[i].count = count;
				hashtable[row].bucket[i].flag = flag + direction;
				return Kick_Operation_opt2(replace_id, row, replace_direction, replace_count, replace_flag);
			}
			else {                             //没找到符合踢条件的项目,转而执行优化1的踢操作
				return Kick_Operation_opt1(id, index, direction, count, flag);
			}
		}
		else if (direction == 1) {             //在上相邻桶中
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
	//踢操作次数到达阈值，插入失败
	else {
		kick = 0;
		return false;
	}
}*/
int main() {
	LR hk(0, 5000, 2, 0, 8);
	/*// 生成随机项目集合
	// 设置随机种子
	std::srand(static_cast<unsigned int>(std::time(0)));
	for (int i = 0; i < 10000; ++i) {
		int randomNum = std::rand() % 300;  // 生成0到99之间的随机数
		hk.insert(randomNum);
	}
	cout << "插入成功" << endl;
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
	cout << "插入成功" << endl;
	cout << hk.getFlowFlag(5);
	cout << hk.getFlowFlag(15);
	cout << hk.getFlowFlag(25);
	cout << hk.getFlowFlag(35);
	cout << hk.getFlowFlag(3);
	cout << hk.getFlowFlag(13);
	cout << hk.getFlowFlag(23);
	cout << hk.getFlowFlag(4);
	cout << hk.getFlowFlag(65) << endl;
	// 测试 新 哈希的性能
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 1000000);

	// 生成随机键集合
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
