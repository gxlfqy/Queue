#pragma once

/*
1. �ö������̰߳�ȫ����
2. �ö����еĶ��󲢲�ͨ��
3. ��������������Ŀ���
*/

//��Ԫ�������
template<typename Type = int>
class QueueList
{
	//��������
public:
	//�쳣����
	static enum except{
		AllocExcept = 1,		//������ʧ��
		NoElements = 2,			//��������û��Ԫ��
		TypeOverflow = 3		//�����������
	};
	//�ֽ�����
	typedef unsigned char byte;

protected:
	//��Ԫ
	typedef class Node
	{
	public:
		Type * m_data;			//���ݵ�ַ
		Node * next;			//��һ����Ԫ�ĵ�ַ
		Node(const byte s) throw(int) :next(nullptr)
		{
			//Ϊ��Ԫ�����ڴ�ռ�
			m_data = new Type[s];
			if (nullptr == m_data)
				throw int(QueueList<Type>::AllocExcept);
		}
		Node(const byte s, const Type data) throw(int) :Node(s)
		{
			//�����ݴ�����еĶ�β
			m_data[1] = data;
		}
		~Node()
		{
			if (nullptr != m_data)
			{
				delete[] m_data;
				m_data = nullptr;
			}
		}
		
	}* PNode;

public:
	QueueList(const byte nNodeMaxSize = 0) throw(int) :m_nNodeSize((0 == nNodeMaxSize /*|| 10 >= nNodeMaxSize*/) ? 10 : nNodeMaxSize), m_front(0), m_rear(0)
	{
		m_rearNode = m_headNode = new Node(m_nNodeSize);
	}
	QueueList(const byte nNodeMaxSize, const Type data) throw(int) :m_nNodeSize((0 == nNodeMaxSize || 10 >= nNodeMaxSize) ? 10 : nNodeMaxSize), m_front(0), m_rear(1)
	{
		m_rearNode = m_headNode = new Node(m_nNodeSize, data);
	}
	~QueueList()
	{
		PNode p;
		if (nullptr != m_headNode)
		{
			p = m_headNode;
			while (p)
			{
				delete p;
				p = m_headNode->next;
			}
			m_rearNode = m_headNode = nullptr;
			m_rear = m_front = 0;
		}
	}
	void push(const Type data) throw(int)
	{
		if (nullptr == m_headNode)
			m_rearNode = m_headNode = new Node(m_nNodeSize, data);
		//β��Ԫ���, �����µĵ�Ԫ, ����, ��������д���µĵ�Ԫ��
		else if (m_headNode == m_rearNode && m_front == (m_rear + 1) % m_nNodeSize || m_headNode != m_rearNode && m_rear >= m_nNodeSize - 1)
		{
			PNode newUnit = new Node(m_nNodeSize, data);
			m_rearNode = m_rearNode->next = newUnit;
			m_rear = 1;
		}
		else
			m_rearNode->m_data[++m_rear %= m_nNodeSize] = data;
	}
	Type pop() throw(int)
	{
		Type temp;
		PNode p;
		//ͷ��ԪΪ��, ɾ��ͷ��Ԫ, ������ͷ��Ԫ
		if (nullptr == m_headNode)
		{
			throw int(QueueList<Type>::NoElements);
			return temp;
		}
		if (m_headNode == m_rearNode && (m_front + 1) % m_nNodeSize == m_rear || m_headNode != m_rearNode && m_front + 1 >= m_nNodeSize - 1)
		{
			temp = m_headNode->m_data[m_front + 1];
			p = m_headNode;
			m_headNode = (nullptr == m_headNode->next) ? m_rearNode = nullptr, nullptr : m_headNode->next;
			delete p;
			m_front = 0;
		}
		else
			temp = m_headNode->m_data[++m_front %= m_nNodeSize];
		return temp;
	}
	long length() const throw(int)
	{
		PNode p;
		long count = 0;
		if (nullptr == m_headNode || m_headNode == m_rearNode)
			return (m_rear - m_front + m_nNodeSize) % m_nNodeSize;
		count += m_nNodeSize - 1 - m_front + m_rear;
		p = m_headNode;
		while (p->next != m_rearNode)
		{
			count += m_nNodeSize - 1;
			if (count < 0)
				throw int(QueueList<Type>::TypeOverflow);
			p = p->next;
		}
		return count;
	}

	//��Ա����
protected:
	Node * m_headNode;				//ͷ���ڵ�
	byte m_front;					//��ͷ���
	Node * m_rearNode;				//β���ڵ�
	byte m_rear;					//��β���
	const byte m_nNodeSize;			//�ڵ㳤��(��Χ��[10, 255])

};