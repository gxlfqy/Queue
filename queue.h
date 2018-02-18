#pragma once

/*
1. 该对象有线程安全问题
2. 该对象中的对象并不通用
3. 长度数据有溢出的可能
*/

//多元链表队列
template<typename Type = int>
class QueueList
{
	//基本类型
public:
	//异常类型
	static enum except{
		AllocExcept = 1,		//堆申请失败
		NoElements = 2,			//队列链中没有元素
		TypeOverflow			//数据类型溢出
	};
	//字节类型
	typedef unsigned char byte;

protected:
	//单元
	typedef class Node
	{
	public:
		Type * m_data;		//数据地址
		byte front;			//头编号
		byte rear;			//尾编号
		Node * next;		//下一个单元的地址
		Node(const byte s) throw(int) :next(nullptr), front(0), rear(0)
		{
			//为单元申请内存空间
			m_data = new Type[s];
			if (nullptr == m_data)
				throw QueueList::AllocExcept;
		}
		Node(const byte s, const Type data) throw(int) :Node(s)
		{
			//将数据存入队列的队尾
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
		//尾单元溢出, 创建新的单元, 关联, 并将数据写入新的单元中
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
		//头单元为空, 删除头单元, 并后移头单元
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

	//成员变量
protected:
	Node * m_head;					//头部节点
	Node * m_rear;					//尾部节点
	const byte m_nNodeSize;			//节点长度(范围是[10, 255])

};