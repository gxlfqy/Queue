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
		TypeOverflow = 3		//数据类型溢出
	};
	//字节类型
	typedef unsigned char byte;

protected:
	//单元
	typedef class Node
	{
	public:
		Type * m_data;			//数据地址
		Node * next;			//下一个单元的地址
		Node(const byte s) throw(int) :next(nullptr)
		{
			//为单元申请内存空间
			m_data = new Type[s];
			if (nullptr == m_data)
				throw int(QueueList<Type>::AllocExcept);
		}
		Node(const byte s, const Type data) throw(int) :Node(s)
		{
			//将数据存入队列的队尾
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
		//尾单元溢出, 创建新的单元, 关联, 并将数据写入新的单元中
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
		//头单元为空, 删除头单元, 并后移头单元
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

	//成员变量
protected:
	Node * m_headNode;				//头部节点
	byte m_front;					//队头编号
	Node * m_rearNode;				//尾部节点
	byte m_rear;					//队尾编号
	const byte m_nNodeSize;			//节点长度(范围是[10, 255])

};