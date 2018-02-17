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
		Type * m_data;									//��Ԫ�Ĵ洢��ַ
		uint8 front;									//��Ԫͷ���
		uint8 rear;										//��Ԫβ���
		GxUnit * next;									//��һ����Ԫ�ĵ�ַ
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
	//UnitsMaxSize�ķ�Χ��[10, 255]
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
		//β��Ԫ���, �����µĵ�Ԫ, ����, ��������д���µĵ�Ԫ��
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
		//ͷ��ԪΪ��, ɾ��ͷ��Ԫ, ������ͷ��Ԫ
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