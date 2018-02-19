#pragma once

//ע����

//��Ԫ�������
template<typename Type = int>
class QueueList
{
public:
	//�쳣����
	static enum except{
		AllocExcept = 1,		//������ʧ��
		NoElements = 2,			//��������û��Ԫ��
		TypeOverflow = 4,		//�����������
		IndexOutofBounds = 8,	//�±�Խ��
		UnknowExcept = 16
	};

	typedef unsigned char byte;
	typedef unsigned long long uint64;

	//�ڵ�
protected:
	typedef class Node
	{
	public:
		Type * m_data;
		Node * next;
		
		Node(const byte s) throw(int) :next(nullptr)
		{
			m_data = new Type[s];
			if (nullptr == m_data)
				throw int(QueueList<Type>::AllocExcept);
		}

		Node(const byte s, const Type & data) throw(int) :Node(s)
		{
			m_data[1] = data;
		}

		Node(const byte s, const Node & obj) throw(int) :Node(s)
		{
			memcpy(m_data, obj.m_data, s * sizeof(Type));
		}

		Node(const byte s, const Node * pobj) throw(int) :Node(s)
		{
			memcpy(m_data, obj->m_data, s * sizeof(Type));
		}

		~Node()
		{
			if (nullptr != m_data)
			{
				delete[] m_data;
				m_data = nullptr;
				next = nullptr;
			}
		}

	}* PNode;

public:
	QueueList(const byte s = 10) :m_nNodeSize((10 < s ? 10 : s) + 1), m_frontNode(nullptr), m_rearNode(nullptr), m_front(0), m_rear(0)
	{
	}

	QueueList(const byte s, const Type & data) throw(int) :QueueList(s)
	{
		m_rearNode = m_frontNode = new Node(m_nNodeSize, data);
		m_rear++;
	}

	QueueList(const QueueList<Type> & obj) throw(int) :m_nNodeSize(obj.m_nNodeSize), m_front(obj.m_front), m_rear(obj.m_rear)
	{
		PNode p;
		PNode q;
		if (nullptr == obj.m_frontNode)
		{
			m_rearNode = nullptr;
			m_rear = m_front = 0;
			return;
		}
		m_frontNode = new Node(m_nNodeSize, obj.m_frontNode);
		q = m_frontNode;
		p = obj.m_frontNode;
		while (p->next)
		{
			q = q->next = new Node(m_nNodeSize, p->next);
			p = p->next;
		}
		q->next = new Node(m_nNodeSize, p);
	}

	~QueueList()
	{
		PNode p;
		if (nullptr != m_frontNode)
		{
			p = m_frontNode;
			while (p->next)
			{
				delete p;
				p = p->next;
			}
			delete p;
			m_frontNode = nullptr;
		}
		m_rearNode = nullptr;
		m_rear = m_front = 0;
	}
	
	uint64 length() const throw(int)
	{
		if (nullptr == m_frontNode)
			return 0;
		else if (m_frontNode == m_rearNode)
			return (m_rear - m_front + m_nNodeSize) % m_nNodeSize;
		uint64 count = 0;
		count += m_nNodeSize - 1 - m_front + m_rear;
		PNode p = m_frontNode;
		while (p->next != m_rearNode)
		{
			count += m_nNodeSize - 1;
			if (count < 0)
				throw int(QueueList<Type>::TypeOverflow);
			p = p->next;
		}
		return count;
	}

	bool empty() const
	{
		return m_frontNode == nullptr;
	}

	void reset()
	{
		this->~QueueList<Type>();
	}

	Type front() const throw(int)
	{
		if (nullptr == m_frontNode)
			throw QueueList<Type>::NoElements;
		return m_frontNode.m_data[(m_front + 1) % m_nNodeSize];
	}

	Type rear() const throw(int)
	{
		if (nullptr == m_frontNode)
			throw QueueList<Type>::NoElements;
		return m_frontNode.m_data[m_rear];
	}

	Type& operator[](long long i) throw(int)
	{
		uint64 length = this->length();
		//�±�Խ����ж�
		if (i >= length || i < 0 && i + length < 0)
			throw int(QueueList<Type>::IndexOutofBounds);
		//�����±�ת��Ϊ���±�
		if (i < 0)
			i = (i + length) % length;
		//��һ�ڵ�
		if (m_frontNode == m_rearNode)
		{
			i = (i + m_nNodeSize) % m_nNodeSize;
			return *(m_frontNode->m_data + i);
		}
		else
		{
			PNode p;
			p = m_frontNode;
			i += m_front;
			while (i > m_nNodeSize - 1 && p->next)
			{
				i -= m_nNodeSize - 1;
				p = p->next;
			}
			if (i < 0)
				throw int(QueueList<Type>::UnknowExcept);
			return *(p->m_data + i + 1);
		}
	}

	void push(const Type & data) throw(int)
	{
		//����Ϊ��, ��������
		if (nullptr == m_frontNode)
			m_rearNode = m_frontNode = new Node(m_nNodeSize, data);
		else
		{
			//β��Ԫ���, �����µĵ�Ԫ, ����, ��������д���µĵ�Ԫ��
			if (m_frontNode == m_rearNode && m_front == (m_rear + 1) % m_nNodeSize || m_frontNode != m_rearNode && m_rear >= m_nNodeSize - 1)
			{
				//�ڵ����ݶ���
				if (m_frontNode == m_rearNode && m_front != 0)
				{
					PNode newNode = new Node(m_nNodeSize);
					memcpy(&newNode->m_data[1], &m_frontNode->m_data[m_front + 1], m_nNodeSize - 1 - m_front);
					memcpy(&newNode->m_data[m_nNodeSize - m_front], &m_frontNode->m_data[0], m_rear + 1);
					m_front = 0;
					delete m_frontNode;
					m_rearNode = m_frontNode = newNode;
				}
				PNode newNode = new Node(m_nNodeSize, data);
				m_rearNode = m_rearNode->next = newNode;
				m_rear = 1;
			}
			else
				m_rearNode->m_data[++m_rear %= m_nNodeSize] = data;
		}
	}

	Type pop() throw(int)
	{
		Type temp;
		PNode p;
		if (nullptr == m_frontNode)
		{
			throw int(QueueList<Type>::NoElements);
			return temp;
		}
		else
		{
			//��ͷ�ڵ�Ϊ��, ɾ��ͷ��Ԫ
			if (m_frontNode == m_rearNode && (m_front + 1) % m_nNodeSize == m_rear || m_frontNode != m_rearNode && m_front + 1 >= m_nNodeSize - 1)
			{
				temp = m_frontNode->m_data[m_front + 1];
				p = m_frontNode;
				m_frontNode = m_frontNode->next;
				delete p;
				//����Ϊ��, ��ʼ������
				if (nullptr == m_frontNode->next)
				{
					m_rearNode = nullptr;
					m_rear = m_front = 0;
				}
				m_front = 0;
			}
			else
				temp = m_frontNode->m_data[++m_front %= m_nNodeSize];
		}
		return temp;
	}

protected:
	PNode m_frontNode;				//��ͷ�ڵ�
	PNode m_rearNode;				//��β�ڵ�
	byte m_front;					//��ͷ���
	byte m_rear;					//��β���
	const byte m_nNodeSize;			//�ڵ㳤��(��Χ��[10, 255])

};