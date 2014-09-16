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
	// Clear the member variables since the function is called multiple times
	delete[] m_Data;
	m_Size = 0;
	
	int maxRunSize = MaxRunSize(); // Store the max run size into a variable
	int typeSize = sizeof(T); // Store the size of type T in byte into a variable	
	const T* temp = input; // Temporary pointer initialized at input
	m_Data = new T[inSize * 2]; // Memory allocation
	int currentRunCount = 0;
	// Loop while we haven't reached the end
	while (temp < (input + inSize)) {
		currentRunCount = 0;
		const T* start = temp;

		// if positive run
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
		// if negative run
		else {
			currentRunCount = 1;
			temp += typeSize;
			while ((*temp != *(temp + typeSize)) && (currentRunCount < maxRunSize)){
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
	// clear member variables
	delete[] m_Data;
	m_Size = 0;

	m_Data = new T[outSize]; // memory allocation
	const T* temp = input;
	int typeSize = sizeof(T);

	// Loop while we haven't reached the end
	while (temp < (input + inSize)) {
		int runIndicator = static_cast<unsigned int>(*temp);

		// if negative run
		if (runIndicator < 0) {
			temp += typeSize;
			for (int i = 0; i < (0 - runIndicator); i++) {		
				m_Data[m_Size] = *temp;
				m_Size++;
				temp += typeSize;
			}
		}
		// if positive run
		else {
			temp += typeSize;
			for (int i = 0; i < runIndicator; i++) {
				m_Data[m_Size] = *temp;
				m_Size++;
			}
			temp += typeSize;
		}
		
	}

}
