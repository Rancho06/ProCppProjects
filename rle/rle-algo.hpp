#pragma once
#include <ostream>

// RLE Compression/Decompression
template <typename T>
struct RLE
{
	// Memory which stores either compressed or decompressed data
	T* m_Data;
	// Number of elements of type T that data is pointing to
	int m_Size;

	RLE()
	: m_Data(nullptr)
	, m_Size(0)
	{ }

	~RLE()
	{
		delete[] m_Data;
	}
	
	// Compresses input data and stores it in m_Data
	void Compress(const T* input, int inSize);

	// Decompresses input data and stores it in m_Data
	void Decompress(const T* input, int inSize, int outSize);

	// Outputs m_Data
	friend std::ostream& operator<< (std::ostream& stream, const RLE<T>& rhs)
	{
		for (int i = 0; i < rhs.m_Size; i++)
		{
			stream << rhs.m_Data[i];
		}
		return stream;
	}

	// Shenanigans to get the maximum run size
	int MaxRunSize();
};

template <typename T>
void RLE<T>::Compress(const T* input, int inSize)
{
	delete[] m_Data;
	m_Size = 0;

	int maxRunSize = MaxRunSize();
	int typeSize = sizeof(T);
	const T* temp = input;

	m_Data = new T[inSize * 2];
	int currentRunCount = 0;
	
	while (temp < (input + inSize)) {
		currentRunCount = 0;
		const T* start = temp;
		if (*temp == *(temp + typeSize)) {
			temp += typeSize;
			currentRunCount = 2;
			while (*temp == *(temp + typeSize) && currentRunCount < maxRunSize) {
				temp += typeSize;
				currentRunCount++;
			}
			temp++;
			m_Data[m_Size] = currentRunCount;
			m_Size++;
			m_Data[m_Size] = *start;
			m_Size++;
		}
		else {
			currentRunCount = 1;
			temp += typeSize;
			while (*temp != *(temp + typeSize)) {
				temp++;
				currentRunCount++;
			}
			m_Data[m_Size] = 0 - currentRunCount;
			m_Size++;
			for (int i = 0; i < currentRunCount; i++) {
				m_Data[m_Size] = *(start+i);
				m_Size++;
			}
		}
	}


}

template <typename T>
void RLE<T>::Decompress(const T* input, int inSize, int outSize)
{
	//delete[] m_Data;
	//m_Size = 0;

	//m_Data = new T[outSize];


}
