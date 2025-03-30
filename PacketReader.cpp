#include "PacketReader.h"

PacketReader::PacketReader(ifstream* ifFile)
{
	m_fileStream.push_back(ifFile);
}

PacketReader::PacketReader(const string src)
{
	addFile(src);
}

PacketReader::PacketReader()
{
}

PacketReader::~PacketReader()
{
	close();
}

void PacketReader::addFile(const string src)
{
	ifstream* ifFile = new ifstream;
	ifFile->open(src, ios_base::in | ios_base::binary);
	if (ifFile->fail()) {
		cout << "���ݰ���ȡ�ļ���ʧ��!" << endl;
		return;
	}
	m_fileStream.push_back(ifFile);
	//m_filesrc.push_back(src);
}

void PacketReader::close()
{
	//����ļ���
	while (!m_fileStream.empty()) {
		ifstream* f = m_fileStream.front();
		f->close();
		f->clear();
		m_fileStream.pop_front();
		delete f;
		f = NULL;
	}
}
