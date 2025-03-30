
#include "HSNPacketReader.h"
#include <winsock.h>

HSNPacketReader::HSNPacketReader(ifstream* ifFile) :PacketReader(ifFile)
{
}

HSNPacketReader::HSNPacketReader(const string src) : PacketReader(src)
{
}

HSNPacketReader::HSNPacketReader()
{
}

HSNPacketReader::~HSNPacketReader()
{
}

/**��ȡһ�����ݰ�
* @param ��ȡ�������ݰ�
* @return ��ȡ�����ݰ���״̬
*/
bool HSNPacketReader::readPacket(Packet& pkt)
{
	ULONG a = 0;
	while (!m_fileStream.empty()) {
		ifstream* f = m_fileStream.front();
		while (!f->eof())
		{
			a++;
			if (a > 100) {
				f->close();
				f->clear();
				m_fileStream.pop_front();
				delete f;
				f = NULL;
				if (!m_fileStream.empty()) {
					f = m_fileStream.front();
				}
				else
					return false;
			}
			bool bRet = readPacket(f, pkt);
			if (!bRet)   // bRetΪtrue��ֱ��return
			{
				//Log::error("���ݰ���ȡ����!");
				continue;
			}
			return true;
		}
		f->close();
		f->clear();
		m_fileStream.pop_front();
		delete f;
		f = NULL;
	}
	return false;
}

bool HSNPacketReader::readPacket(ifstream* ifFile, Packet& pkt)
{
	//��ȡ���ݰ���ʱ���
	ifFile->read((char*)&(pkt.time), sizeof(pkt.time));
	if (!ifFile)
	{
		cout << "read timestamp failure!" << endl;
		return false;
	}

	//��ȡIP�ײ�����
	IpHeader* pIpHeader = (IpHeader*) new char[sizeof(IpHeader)];
	if (NULL == pIpHeader)
	{
		cout << "new array error!" << endl;
		return false;
	}

	ifFile->read((char*)pIpHeader, sizeof(IpHeader));
	if (!ifFile)
	{
		cout << "read IP header failure!" << endl;
		return false;
	}

	//��ȡIP�ײ��ؼ��ֶ���Ϣ
	USHORT usIpHdrLen = (pIpHeader->ver_ihl & 0x0f) * 4;//��ȡIPͷ������
	USHORT usIpTotalLen = ntohs(pIpHeader->total_len); 	//��ȡIP���ܳ���
	pkt.src.ip = ntohl(pIpHeader->srcIP);
	pkt.dst.ip = ntohl(pIpHeader->dstIP);
	pkt.proto = pIpHeader->protocol;

	delete[] pIpHeader;
	pIpHeader = NULL;

	//����IPѡ�����
	if (usIpHdrLen > 20)
	{
		ifFile->seekg(4, ios_base::cur);
		if (!ifFile)
		{
			cout << "seek file failure!" << endl;
			return false;
		}
	}

	//����Э���ֶ�������ȡ���������
	if (pkt.proto == PROTO_TCP)
	{
		//��ȡTCP�ײ�����
		TcpHeader* pTcpHeader = (TcpHeader*) new char[sizeof(TcpHeader)];
		if (NULL == pTcpHeader)
		{
			cout << "new array error!" << endl;
			return false;
		}

		ifFile->read((char*)pTcpHeader, sizeof(TcpHeader));
		if (!ifFile)
		{
			cout << "read TCP header failure!" << endl;
			return false;
		}

		//��ȡ�˿ں�
		pkt.src.port = ntohs(pTcpHeader->src_port);
		pkt.dst.port = ntohs(pTcpHeader->dst_port);

		delete[] pTcpHeader;
		pTcpHeader = NULL;
	}
	else if (pkt.proto == PROTO_UDP)
	{
		//��ȡUDP�ײ�����
		UdpHeader* pUdpHeader = (UdpHeader*)new char[sizeof(UdpHeader)];
		if (NULL == pUdpHeader)
		{
			cout << "new array error!" << endl;
			return false;
		}

		ifFile->read((char*)pUdpHeader, sizeof(UdpHeader));
		if (!ifFile)
		{
			cout << "read UDP Header Failure!" << endl;
			return false;
		}

		//��ȡ�˿ں�
		pkt.src.port = ntohs(pUdpHeader->src_port);
		pkt.dst.port = ntohs(pUdpHeader->dst_port);

		delete[] pUdpHeader;
		pUdpHeader = NULL;
	}
	else	//��TCP��UDP����GRE(47)
	{
		ifFile->seekg(8, ios_base::cur);
		if (!ifFile)
			cout << "seek file failure!\n" << endl;

		return false;
	}

	return true;
}
