#pragma once

#include "common.h"
#include <list>
#include <fstream>
#include "PacketReader.h"
#include<iostream>
using namespace std;

/**
*	数据包读取类
*/
class PacketReader
{
protected:
	list<ifstream*>  m_fileStream;		//TRACE文件流
	list<string> m_filesrc;
public:
	PacketReader(ifstream* ifFile);
	PacketReader(const string src);
	PacketReader();
	virtual ~PacketReader();

	/**读取一个数据包
	* @param 读取到的数据包
	* @return 数据包读取状态，读取成功为true
	*/
	virtual bool readPacket(Packet& pkt) = 0;

	/**添加一个数据包文件
	* @param 文件路径
	* @数据包读取状态，读取成功为true
	*/
	virtual void addFile(const string src);

	/**关闭文件流*/
	virtual void close();
};

