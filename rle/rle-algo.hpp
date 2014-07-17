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

}

template <typename T>
void RLE<T>::Decompress(const T* input, int inSize, int outSize)
{

}
