// implementation of class DArray
#include "DArray.h"
#include<iostream>
// default constructor
DArray::DArray() {
	Init();
}

// set an array with default values
DArray::DArray(int nSize, double dValue) {
	m_nSize = nSize;
	m_nMax = nSize * 2;
	if (m_nSize < 0) {
		throw std::invalid_argument("Invalid arguments");
	}
	else if (m_nSize == 0) {
		m_pData = nullptr;
	}
	else {
		m_pData = new double[m_nMax];
		for (int i = 0; i < m_nSize; i++) {
			m_pData[i] = dValue;
		}
	}
}

DArray::DArray(const DArray& arr) {
	m_nSize = arr.m_nSize;
	m_nMax = m_nSize * 2;
	if (m_nSize != 0) {
		m_pData = new double[m_nMax];
		for (int i = 0; i < m_nSize; i++) {
			m_pData[i] = arr[i];
		}
	}
	else {
		m_pData = nullptr;
	}
}

// deconstructor
DArray::~DArray() {
	Free();
}

// display the elements of the array
void DArray::Print() const {
	std::cout << "size = " << m_nSize << " : ";
	for (int i = 0; i < m_nSize; i++)
	{
		std::cout << m_pData[i] << " ";
	}
	std::cout << std::endl;
}

// initilize the array
void DArray::Init() {
	m_nSize = 0;
	m_nMax = 0;
	m_pData = nullptr;
}

// free the array
void DArray::Free() {
	if (m_pData != nullptr) {
		delete[] m_pData;
	}
}

// get the size of the array
int DArray::GetSize() const {
	//TODO
	return m_nSize; // you should return a correct value
}

// set the size of the array
void DArray::SetSize(int nSize) {
	if (nSize < 0) {
		throw std::invalid_argument("Invalid arguments");
	}
	else if (nSize == 0) {
		delete[] m_pData;
		m_pData = nullptr;
	}
	else {
		double* pTempPtr = new double[2*nSize];
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
const double& DArray::GetAt(int nIndex) const {
	if (nIndex < 0 || nIndex >= m_nSize) {
		throw std::out_of_range("index out of range");
	}
	else {
		return m_pData[nIndex];
	}
}

// set the value of an element 
void DArray::SetAt(int nIndex, double dValue) {
	if (nIndex < 0 || nIndex >= m_nSize) {
		throw std::out_of_range("index out of range");
	}
	else {
		m_pData[nIndex] = dValue;
	}
}

// overload operator '[]'
double& DArray::operator[](int nIndex) {
	if (nIndex < 0 || nIndex >= m_nSize) {
		throw std::out_of_range("index out of range");
	}
	else {
		return m_pData[nIndex];
	}
}

// overload operator '[]'
const double& DArray::operator[](int nIndex) const {
	return this->GetAt(nIndex);
}

// add a new element at the end of the array
void DArray::PushBack(double dValue) {
	if (m_nMax>m_nSize) {
		m_pData[m_nSize] = dValue;
		m_nSize += 1;
	}
	else {
		double* pTempPtr = new double[(m_nSize + 1) * 2];
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
void DArray::DeleteAt(int nIndex) {
	if (nIndex < 0 || nIndex >= m_nSize) {
		throw std::out_of_range("index out of range");
	}
	else {
		for (int i = 0; i < m_nSize; i++) {
			if(i > nIndex){
				m_pData[i - 1] = m_pData[i];
			}
		}
		m_nSize -= 1;
	}
}

// insert a new element at some index
void DArray::InsertAt(int nIndex, double dValue) {
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
						m_pData[i] = m_pData[i-1];
					}
				}
			}
			else {
				double* pTempPtr = new double[(m_nSize + 1) * 2];
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
DArray& DArray::operator = (const DArray& arr) {
	if (m_pData != nullptr) {
		delete[] m_pData;
	}
	m_nSize = arr.m_nSize;
	m_nMax = 2 * m_nSize;
	if (m_nSize != 0) {
		m_pData = new double[m_nSize];
		for (int i = 0; i < m_nSize; i++) {
			m_pData[i] = arr[i];
		}
	}
	else {
		m_pData = nullptr;
	}
	return *this;
}
