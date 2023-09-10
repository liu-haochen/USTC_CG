#pragma once
#include<iostream>
// interfaces of Dynamic Array class DArray
template<class Type>
class DArray {
public:

	// default constructor
	DArray() {
		Init();
	}

	// set an array with default values
	DArray(int nSize, Type dValue) {
		m_nSize = nSize;
		m_nMax = nSize * 2;
		if (m_nSize < 0) {
			throw std::invalid_argument("Invalid arguments");
		}
		else if (m_nSize == 0) {
			m_pData = nullptr;
		}
		else {
			m_pData = new Type[m_nMax];
			for (int i = 0; i < m_nSize; i++) {
				m_pData[i] = dValue;
			}
		}
	}

	DArray(const DArray& arr) {
		m_nSize = arr.m_nSize;
		m_nMax = m_nSize * 2;
		if (m_nSize != 0) {
			m_pData = new Type[m_nMax];
			for (int i = 0; i < m_nSize; i++) {
				m_pData[i] = arr[i];
			}
		}
		else {
			m_pData = nullptr;
		}
	}

	// deconstructor
	~DArray() {
		Free();
	}

	// display the elements of the array
	void Print() const {
		std::cout << "size = " << m_nSize << " : ";
		for (int i = 0; i < m_nSize; i++)
		{
			std::cout << m_pData[i] << " ";
		}
		std::cout << std::endl;
	}


	// get the size of the array
	int GetSize() const {
		//TODO
		return m_nSize; // you should return a correct value
	}

	// set the size of the array
	void SetSize(int nSize) {
		if (nSize < 0) {
			throw std::invalid_argument("Invalid arguments");
		}
		else if (nSize == 0) {
			delete[] m_pData;
			m_pData = nullptr;
		}
		else {
			Type* pTempPtr = new Type[2 * nSize];
			for (int i = 0; i < nSize; i++) {
				if (i < m_nSize) {
					pTempPtr[i] = m_pData[i];
				}
				else {
					pTempPtr[i] = 0;
				}
			}
			if (m_pData != nullptr) {
				delete[] m_pData;
			}
			m_pData = pTempPtr;
		}
		m_nSize = nSize;
		m_nMax = 2 * nSize;
	}

	// get an element at an index
	const Type& GetAt(int nIndex) const {
		if (nIndex < 0 || nIndex >= m_nSize) {
			throw std::out_of_range("index out of range");
		}
		else {
			return m_pData[nIndex];
		}
	}

	// set the value of an element 
	void SetAt(int nIndex, Type dValue) {
		if (nIndex < 0 || nIndex >= m_nSize) {
			throw std::out_of_range("index out of range");
		}
		else {
			m_pData[nIndex] = dValue;
		}
	}

	// overload operator '[]'
	Type& operator[](int nIndex) {
		if (nIndex < 0 || nIndex >= m_nSize) {
			throw std::out_of_range("index out of range");
		}
		else {
			return m_pData[nIndex];
		}
	}

	// overload operator '[]'
	const Type& operator[](int nIndex) const {
		return this->GetAt(nIndex);
	}

	// add a new element at the end of the array
	void PushBack(Type dValue) {
		if (m_nMax > m_nSize) {
			m_pData[m_nSize] = dValue;
			m_nSize += 1;
		}
		else {
			Type* pTempPtr = new Type[(m_nSize + 1) * 2];
			for (int i = 0; i < m_nSize; i++) {
				pTempPtr[i] = m_pData[i];
			}
			pTempPtr[m_nSize] = dValue;
			delete[] m_pData;
			m_pData = pTempPtr;
			m_nSize = m_nSize + 1;
			m_nMax = m_nSize * 2;
		}
	}

	// delete an element at some index
	void DeleteAt(int nIndex) {
		if (nIndex < 0 || nIndex >= m_nSize) {
			throw std::out_of_range("index out of range");
		}
		else {
			for (int i = 0; i < m_nSize; i++) {
				if (i > nIndex) {
					m_pData[i - 1] = m_pData[i];
				}
			}
			m_nSize -= 1;
		}
	}

	// insert a new element at some index
	void InsertAt(int nIndex, Type dValue) {
		if (nIndex < 0 || nIndex > m_nSize) {
			throw std::out_of_range("index out of range");
		}
		else {
			if (m_nSize == 0 || nIndex == m_nSize) {
				this->PushBack(dValue);
			}
			else {
				if (m_nMax > m_nSize) {
					for (int i = m_nSize; i >= 0; i--) {
						if (i == nIndex) {
							m_pData[i] = dValue;
						}
						else if (i > nIndex) {
							m_pData[i] = m_pData[i - 1];
						}
					}
				}
				else {
					Type* pTempPtr = new Type[(m_nSize + 1) * 2];
					for (int i = 0; i < m_nSize; i++) {
						pTempPtr[i] = m_pData[i];
					}
					delete[] m_pData;
					m_pData = pTempPtr;
					m_nMax = (m_nSize + 1) * 2;
					this->InsertAt(nIndex, dValue);
				}
			}
		}
	}

	// overload operator '='
	DArray& operator = (const DArray& arr) {
		if (m_pData != nullptr) {
			delete[] m_pData;
		}
		m_nSize = arr.m_nSize;
		m_nMax = 2 * m_nSize;
		if (m_nSize != 0) {
			m_pData = new Type[m_nSize];
			for (int i = 0; i < m_nSize; i++) {
				m_pData[i] = arr[i];
			}
		}
		else {
			m_pData = nullptr;
		}
		return *this;
	}


private:
	Type* m_pData; // the pointer to the array memory
	int m_nSize; // the size of the array
	int m_nMax;

private:
	// initilize the array
	void Init() {
		m_nSize = 0;
		m_nMax = 0;
		m_pData = nullptr;
	}

	// free the array
	void Free() {
		if (m_pData != nullptr) {
			delete[] m_pData;
		}
	}
	void Reserve(int nSize); // allocate enough memory
};
