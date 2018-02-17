#pragma once

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;


template<typename Type>
class GxQueue
{
public:
	static enum except{
		AllocExcept = 1,
	};

protected:
	class GxUnit
	{
	public:
		Type * m_data;									//单元的存储地址
		uint8 front;									//单元头编号
		uint8 rear;										//单元尾编号
		GxUnit * next;									//下一个单元的地址
		GxUnit(const uint8 s) throw(int) :next(nullptr)
		{
			m_data = new Type[s];
			if (nullptr == m_data)
				throw GxQueue::AllocExcept;
			front = 0;
			rear = 0;
		}
		GxUnit(const uint8 s, const Type data) throw(int) :GxUnit(s)
		{
			//this->GxUnit::GxUnit(size);
			memcpy(m_data + 1, &data, sizeof(Type));
			rear = 1;
		}
		~GxUnit()
		{
			if (nullptr != m_data)
			{
				delete[] m_data;
				m_data = nullptr;
			}
		}
		
	};
	typedef GxUnit * PGxUnit;

public:
	//UnitsMaxSize的范围是[10, 255]
	GxQueue(uint8 nUnitMaxSize = 0) throw(int) :m_unitsSize((0 == nUnitMaxSize /*|| 10 >= nUnitMaxSize*/) ? 10 : nUnitMaxSize)
	{
		m_rearUnit = m_headUnit = new GxUnit(m_unitsSize);
	}
	GxQueue(uint8 nUnitMaxSize, Type data) throw(int) :m_unitsSize((0 == nUnitMaxSize || 10 >= nUnitMaxSize) ? 10 : nUnitMaxSize)
	{
		m_rearUnit = m_headUnit = new GxUnit(m_unitsSize, data);
	}
	void push(Type data) throw(int)
	{
		if (nullptr == m_headUnit)
			m_rearUnit = m_headUnit = new GxUnit(m_unitsSize, data);
		//尾单元溢出, 创建新的单元, 关联, 并将数据写入新的单元中
		else if (m_rearUnit->front == (m_rearUnit->rear + 1) % m_unitsSize)
		{
			PGxUnit newUnit = new GxUnit(m_unitsSize, data);
			m_rearUnit = m_rearUnit->next = newUnit;
		}
		else
		{
			++m_rearUnit->rear;
			m_rearUnit->rear %= m_unitsSize;
			m_rearUnit->m_data[m_rearUnit->rear] = data;
		}
	}
	Type pop()
	{
		//头单元为空, 删除头单元, 并后移头单元
		Type temp;
		uint8 & front = m_headUnit->front;
		uint8 & rear = m_headUnit->rear;
		Type * data = m_headUnit->m_data;
		PGxUnit p;
		++front %= m_unitsSize;
		if (front == rear)
		{
			temp = data[m_headUnit->rear];
			p = m_headUnit;
			m_headUnit = (nullptr == m_headUnit->next) ? m_rearUnit = nullptr, nullptr : m_headUnit->next;
			delete p;
		}
		else
			temp = m_headUnit->m_data[front];
		return temp;
	}

protected:
	GxUnit * m_headUnit;
	GxUnit * m_rearUnit;
	const uint8 m_unitsSize;

};