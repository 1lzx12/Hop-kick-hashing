#pragma once

#include "common.h"
#include <list>
#include <fstream>
#include "PacketReader.h"
#include<iostream>
using namespace std;

/**
*	���ݰ���ȡ��
*/
class PacketReader
{
protected:
	list<ifstream*>  m_fileStream;		//TRACE�ļ���
	list<string> m_filesrc;
public:
	PacketReader(ifstream* ifFile);
	PacketReader(const string src);
	PacketReader();
	virtual ~PacketReader();

	/**��ȡһ�����ݰ�
	* @param ��ȡ�������ݰ�
	* @return ���ݰ���ȡ״̬����ȡ�ɹ�Ϊtrue
	*/
	virtual bool readPacket(Packet& pkt) = 0;

	/**���һ�����ݰ��ļ�
	* @param �ļ�·��
	* @���ݰ���ȡ״̬����ȡ�ɹ�Ϊtrue
	*/
	virtual void addFile(const string src);

	/**�ر��ļ���*/
	virtual void close();
};

