#include "新哈希.h"
#include"PacketReader.h"
#include"HSNPacketReader.h"
#include"HashFunctions.h"
#include"common.h"
#include <time.h>
#include <fstream>
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


	entryTable = new Entry * [ROW];
	for (ULONG i = 0; i < ROW; i++) {
		entryTable[i] = new Entry[COL];
	}

}

LR::~LR()
{
	for (ULONG i = 0; i < ROW; i++) {
		delete[] entryTable[i];
	}
	delete[] entryTable;
}

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
			find_count++;
			if (entryTable[index[i]][j].id == id) {
				return  entryTable[index[i]][j].count;
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
			if (entryTable[index[i]][j].id == id) {
				return  entryTable[index[i]][j].flag;
			}
		}
	}
	return -2;
}
int LR::getKick_count()
{
	return kick_count;
}
int LR::getConflict()
{
	return conflict;
}
int LR::getVisit_count()
{
	return visit_count;
}
int LR::getFind_count()
{
	return find_count;
}
int LR::getHash_count()
{
	return hash_count;
}
double LR::getMemory_utilizationrate()
{
	double items = 0;
	for (ULONG i = 0; i < ROW; i++) {
		for (ULONG j = 0; j < COL; j++) {
			if (entryTable[i][j].count != 0 ) {
				items++;
			}
		}
	}
	return items;
}
//用新哈希计算存储位置,用index存储三个候选桶的位置
void LR::getPositionRecord(int id, ULONG* index)
{
	index[0] = id % ROW;
	hash_count++;
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
			visit_count++;
			if (entryTable[index[i]][j].id == id) {
				++entryTable[index[i]][j].count;
				return true;
			}
			else if (entryTable[index[i]][j].count == 0) {
				entryTable[index[i]][j].id = id;
				entryTable[index[i]][j].count = 1;
				if (i == 0) { entryTable[index[i]][j].flag = 0; }
				else if (i == 1) { entryTable[index[i]][j].flag = -1; }
				else { entryTable[index[i]][j].flag = 1; }
				return true;
			}		
		}
	}
	srand((unsigned)time(NULL));
	int i = rand() % 2;
	int direction = 1;
	if (i == 0) direction = -1;
	if (i == 1) direction = 1;
	conflict++;
	Kick_Operation_opt1(id, index[0], direction, 1, 0);
	return false;
}

/*bool LR::replace(const FlowID& fid, ULONG row, ULONG col, int flag)
{
	entryTable[row][col].fid = fid;
	entryTable[row][col].count = 1;
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
	//查找是否存在空位，有空位直接插入 
	for (i = 0; i < COL; i++) {
		visit_count++;
		if (entryTable[row][i].count == 0) {
			entryTable[row][i].id = id;
			entryTable[row][i].count = count;
			entryTable[row][i].flag = flag + direction;
			kick = 0;
			return true;
		}
	}
	//不存在空位，选择一条偏移量为0的项目踢走
	if (kick <= kick_t) {                //踢操作次数没到达阈值，反复执行直到出现空位
		for (ULONG i = 0; i < COL; i++)  {
			visit_count++;
			if (entryTable[row][i].flag == 0) {
				replace_id = entryTable[row][i].id;
				replace_count = entryTable[row][i].count;
				replace_flag = entryTable[row][i].flag;
				entryTable[row][i].id = id;
				entryTable[row][i].count = count;
				entryTable[row][i].flag = flag + direction;
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
		visit_count++;
		if (entryTable[row][i].count == 0) {
			entryTable[row][i].id = id;
			entryTable[row][i].count = count;
			entryTable[row][i].flag = flag + direction;
			kick = 0;
			return true;
		}
	}
	//选择一条偏移量不为0的项目踢回其映射位置
	if (kick <= kick_t) {                     //踢操作次数没到达阈值，反复执行直到出现空位
		if (direction == -1) {
			for (ULONG i = 0; i < COL; i++) {
				visit_count++;
				if (entryTable[row][i].flag == 1) {
					replace_id = entryTable[row][i].id;
					replace_count = entryTable[row][i].count;
					replace_flag = entryTable[row][i].flag;
					entryTable[row][i].id = id;
					entryTable[row][i].count = count;
					entryTable[row][i].flag = flag + direction;
					return Kick_Operation_opt1(replace_id, row, replace_direction, replace_count, replace_flag);
				}
			}
			//踢操作次数没到达阈值，但在第一个优化条件下已不存在满足踢条件（没有偏移量为0的项目）的项目，转而采用基础版本的踢操作
			return Kick_Operation(id, index, direction, count, flag);
		}
		if (direction == 1) {
			for (ULONG i = 0; i < COL; i++) {
				visit_count++;
				if (entryTable[row][i].flag == -1) {
					replace_id = entryTable[row][i].id;
					replace_count = entryTable[row][i].count;
					replace_flag = entryTable[row][i].flag;
					entryTable[row][i].id = id;
					entryTable[row][i].count = count;
					entryTable[row][i].flag = flag + direction;
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
bool LR::Kick_Operation_opt2(int id, ULONG index, int direction, ULONG count, int flag)
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
		if (entryTable[row][i].count == 0) {
			entryTable[row][i].id = id;
			entryTable[row][i].count = count;
			entryTable[row][i].flag = flag + direction;
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
				if ((entryTable[row][i].count > count) && (entryTable[row][i].flag == 1)) {
					count = entryTable[row][i].count;
					pos = i;
				}
			}
			if (pos != -1) {                  //找到了符合踢条件的项目,继续执行优化2的踢操作
				replace_id = entryTable[row][i].id;
				replace_count = entryTable[row][i].count;
				replace_flag = entryTable[row][i].flag;
				entryTable[row][i].id = id;
				entryTable[row][i].count = count;
				entryTable[row][i].flag = flag + direction;
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
				if ((entryTable[row][i].count > count) && (entryTable[row][i].flag == -1)) {
					count = entryTable[row][i].count;
					pos = i;
				}
			}
			if (pos != -1) {
				replace_id = entryTable[row][i].id;
				replace_count = entryTable[row][i].count;
				replace_flag = entryTable[row][i].flag;
				entryTable[row][i].id = id;
				entryTable[row][i].count = count;
				entryTable[row][i].flag = flag + direction;
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
}
void ShowCuckoo_row(int row[], int conflict[],  int insert_visit[], int find_visit[]) {
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
	for (int k = 0, j = 0; k < 5000; k++) {
		sfind_keys[k] = flowdata[j];
		j += 2;
	}
	for (int i = 0; i < 10; i++) {
		int j = 5000 / row[i];
		LR hk(0, j, row[i], 0, 6);
		for (int k = 0; k < 10000; k++) {
			//ch.insert(keys[k]);
			hk.insert(flowdata[k]);
		}
		for (const auto& key : sfind_keys) {
			hk.getFlowNum(key);
		}
		cout << "单元格数量：" << i*2 <<"哈希桶数量："<< j << endl;
		conflict[i] = hk.getConflict();
		insert_visit[i] = hk.getVisit_count();
		find_visit[i] = hk.getFind_count();
		cout << hk.getConflict() << endl;
		cout << hk.getVisit_count() << endl;
	}
}
void ShowCuckoo_kick(int kick[], int conflict[], int insert_visit[], int find_visit[]) {
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
	for (int k = 0, j = 0; k < 5000; k++) {
		sfind_keys[k] = flowdata[j];
		j += 2;
	}
	for (int i = 0; i < 10; i++) {
		LR hk(0, 2000, 2, 0, kick[i]);
		for (int k = 0; k < 10000; k++) {
			//ch.insert(keys[k]);
			hk.insert(flowdata[k]);
		}
		for (const auto& key : sfind_keys) {
			hk.getFlowNum(key);
		}
		cout << "踢操作阈值：" << i << endl;
		conflict[i] = hk.getConflict();
		insert_visit[i] = hk.getVisit_count();
		find_visit[i] = hk.getFind_count();
		cout << hk.getKick_count() << endl;
		cout << hk.getConflict() << endl;
		cout << hk.getMemory_utilizationrate() / 10000 << endl;
	}
}
int main() {

	/*LR hk(0, 1667, 6, 0, 6);
	// 生成随机项目集合
	// 设置随机种子  
	// 测试 新 哈希的性能
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 100000);

	// 生成随机键集合
	std::vector<int> keys;
	for (int i = 0; i < 10000; ++i) {
		keys.push_back(dis(gen));
	}
	int sfind_keys[2000];
	int usfind_keys[2000];
	for (int i = 0, j = 0; i < 10000; j++) {
		int judge = 1;
		for (int k = 0; k < 10000; k++) {
			if (j == keys[k]) {
				judge = 0;
			}
		}
		if (judge == 1) {
			usfind_keys[i++] = j;
		}
		if (i == 2000) {
			break;
		}
	}
	for (int i = 0,j = 0; i < 2000; i++) {
		sfind_keys[i] = keys[j];
		j += 3;
	}
	//auto start = std::chrono::steady_clock::now();
	for (const auto& key : keys) {
		hk.insert(key);
	}
	//auto end = std::chrono::steady_clock::now();
	//std::chrono::duration<double> hopkickinserttime = end - start;
	//0.00047
	auto start = std::chrono::steady_clock::now();
	for (const auto& key : usfind_keys) {
		hk.getFlowNum(key);
	}
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> hopkick_usfindtime = end - start;
	//0.0023
	cout << hk.getKick_count() << endl;
	cout << hk.getConflict() << endl;
	cout << hk.getVisit_count() << endl;
	cout << hk.getHash_count() << endl;
	cout << hk.getMemory_utilizationrate()/10000 << endl;
	//std::cout << "Cuckoo Hashing Time: " << hopkickinserttime.count() << " seconds" << std::endl;
	//std::cout << "Cuckoo usfind Time: " << hopkick_usfindtime.count() << " seconds" << std::endl;
	std::cout << "Cuckoo usfind Time: " << hopkick_usfindtime.count() << " seconds" << std::endl; */
	int row[] = {2, 3, 4, 6, 8, 10, 13, 17, 22, 27}; // x轴数据
	int kick[] = { 2, 3, 4, 6, 8, 10, 13, 17, 22, 27 };
	int y1[10]; // 第一个数组的数据
	int y2[10]; // 第二个数组的数据
	int y3[10]; // 第三个数组的数据
	int y4[10]; // 第四个数组的数据
	int y5[10]; // 第一个数组的数据
	int y6[10]; // 第二个数组的数据
	int y7[10]; // 第三个数组的数据
	int y8[10]; // 第四个数组的数据
	int y9[10]; // 第四个数组的数据
	int y10[10]; // 第四个数组的数据
	ShowCuckoo_row(row, y1, y2, y3);
	ShowCuckoo_kick(kick, y4, y5, y6);
	std::ofstream outputFile("hopkick_tiaocan_flow.csv"); // 创建一个名为"data.csv"的文件

    //将数据写入CSV文件
	for (size_t i = 0; i < 10; ++i) {
		outputFile << row[i] << "," << y1[i] << "," << y2[i] << "," << y3[i] << "," << y4[i] << "," << y5[i] << "," << y6[i] << "," ;
	}
	outputFile.close(); // 关闭文件
	return 0;
}
