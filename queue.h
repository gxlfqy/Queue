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
	class Unit
	{
	public:
		Type * m_data;									//单元的存储地址
		uint8 front;									//单元头编号
		uint8 rear;										//单元尾编号
		Unit * next;									//下一个单元的地址
		Unit(const uint8 s) throw(int) :next(nullptr)
		{
			m_data = new Type[s];
			if (nullptr == m_data)
				throw GxQueue::AllocExcept;
			front = 0;
			rear = 0;
		}
		Unit(const uint8 s, const Type data) throw(int) :Unit(s)
		{
			//this->GxUnit::GxUnit(size);
			memcpy(m_data + 1, &data, sizeof(Type));
			rear = 1;
		}
		~Unit()
		{
			if (nullptr != m_data)
			{
				delete[] m_data;
				m_data = nullptr;
			}
		}
		
	};
	typedef Unit * PGxUnit;

public:
	//UnitsMaxSize的范围是[10, 255]
	GxQueue(uint8 nUnitMaxSize = 0) throw(int) :m_unitsSize((0 == nUnitMaxSize /*|| 10 >= nUnitMaxSize*/) ? 10 : nUnitMaxSize)
	{
		m_rearUnit = m_headUnit = new Unit(m_unitsSize);
	}
	GxQueue(uint8 nUnitMaxSize, Type data) throw(int) :m_unitsSize((0 == nUnitMaxSize || 10 >= nUnitMaxSize) ? 10 : nUnitMaxSize)
	{
		m_rearUnit = m_headUnit = new Unit(m_unitsSize, data);
	}
	void push(Type data) throw(int)
	{
		if (nullptr == m_headUnit)
			m_rearUnit = m_headUnit = new Unit(m_unitsSize, data);
		//尾单元溢出, 创建新的单元, 关联, 并将数据写入新的单元中
		else if (m_rearUnit->front == (m_rearUnit->rear + 1) % m_unitsSize)
		{
			PGxUnit newUnit = new Unit(m_unitsSize, data);
			m_rearUnit = m_rearUnit->next = newUnit;
		}
		else
			m_rearUnit->m_data[++m_rearUnit->rear %= m_unitsSize] = data;
	}
	Type pop()
	{
		//头单元为空, 删除头单元, 并后移头单元
		Type temp;
		PGxUnit p;
		if ((m_headUnit->front + 1) % m_unitsSize == m_headUnit->rear)
		{
			temp = m_headUnit->m_data[m_headUnit->rear];
			p = m_headUnit;
			m_headUnit = (nullptr == m_headUnit->next) ? m_rearUnit = nullptr, nullptr : m_headUnit->next;
			delete p;
		}
		else
			temp = m_headUnit->m_data[++m_headUnit->front %= m_unitsSize];
		return temp;
	}

protected:
	Unit * m_headUnit;
	Unit * m_rearUnit;
	const uint8 m_unitsSize;

};