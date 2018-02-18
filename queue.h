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
		TypeOverflow			//�����������
	};
	//�ֽ�����
	typedef unsigned char byte;

protected:
	//��Ԫ
	typedef class Node
	{
	public:
		Type * m_data;		//���ݵ�ַ
		byte front;			//ͷ���
		byte rear;			//β���
		Node * next;		//��һ����Ԫ�ĵ�ַ
		Node(const byte s) throw(int) :next(nullptr), front(0), rear(0)
		{
			//Ϊ��Ԫ�����ڴ�ռ�
			m_data = new Type[s];
			if (nullptr == m_data)
				throw QueueList::AllocExcept;
		}
		Node(const byte s, const Type data) throw(int) :Node(s)
		{
			//�����ݴ�����еĶ�β
			m_data[++rear] = data;
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
	QueueList(const byte nNodeMaxSize = 0) throw(int) :m_nNodeSize((0 == nNodeMaxSize || 10 >= nNodeMaxSize) ? 10 : nNodeMaxSize)
	{
		m_rear = m_head = new Node(m_nNodeSize);
	}
	QueueList(const byte nNodeMaxSize, const Type data) throw(int) :m_nNodeSize((0 == nNodeMaxSize || 10 >= nNodeMaxSize) ? 10 : nNodeMaxSize)
	{
		m_rear = m_head = new Node(m_nNodeSize, data);
	}
	~QueueList()
	{
		PNode p;
		if (nullptr != m_head)
		{
			p = m_head;
			while (p)
			{
				delete p;
				p = m_head->next;
			}
			m_rear = m_head = nullptr;
		}
	}
	void push(const Type data) throw(int)
	{
		if (nullptr == m_head)
			m_rear = m_head = new Node(m_nNodeSize, data);
		//β��Ԫ���, �����µĵ�Ԫ, ����, ��������д���µĵ�Ԫ��
		else if (m_rear->front == (m_rear->rear + 1) % m_nNodeSize)
		{
			PNode newUnit = new Node(m_nNodeSize, data);
			m_rear = m_rear->next = newUnit;
		}
		else
			m_rear->m_data[++m_rear->rear %= m_nNodeSize] = data;
	}
	Type pop() throw(int)
	{
		Type temp;
		PNode p;
		//ͷ��ԪΪ��, ɾ��ͷ��Ԫ, ������ͷ��Ԫ
		if (nullptr == m_head)
			throw QueueList::NoElements;
		if ((m_head->front + 1) % m_nNodeSize == m_head->rear)
		{
			temp = m_head->m_data[m_head->rear];
			p = m_head;
			m_head = (nullptr == m_head->next) ? m_rear = nullptr, nullptr : m_head->next;
			delete p;
		}
		else
			temp = m_head->m_data[++m_head->front %= m_nNodeSize];
		return temp;
	}
	long length() const throw(int)
	{
		PNode p;
		long count = 0;
		if (nullptr == m_head)
			return 0;
		p = m_head;
		while (p)
		{
			if (count < 0)
				throw QueueList::TypeOverflow;
			count += (p->rear - p->front + m_nNodeSize) % m_nNodeSize;
			p = p->next;
		}
		return count;
	}

	//��Ա����
protected:
	Node * m_head;					//ͷ���ڵ�
	Node * m_rear;					//β���ڵ�
	const byte m_nNodeSize;			//�ڵ㳤��(��Χ��[10, 255])

};