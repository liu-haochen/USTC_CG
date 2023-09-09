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
	if (m_nSize < 0) {
		throw std::invalid_argument("Invalid arguments");
	}
	else if(m_nSize == 0){
		m_pData = nullptr;
	}
	else {
		m_pData = new double[m_nSize];
		for (int i = 0; i < m_nSize; i++) {
			m_pData[i] = dValue;
		}
	}
}

DArray::DArray(const DArray& arr) {
	m_nSize = arr.m_nSize;
	if (m_nSize != 0) {
		m_pData = new double[m_nSize];
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
	std::cout<<"size = " << m_nSize <<" : ";
	for (int i = 0; i < m_nSize; i++)
	{
		std::cout << m_pData[i] << " ";
	}
	std::cout << std::endl;
}

// initilize the array
void DArray::Init() {
	m_nSize = 0;
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
		double* pTempPtr = new double[nSize];
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
	
}

// get an element at an index
const double& DArray::GetAt(int nIndex) const {
	if (nIndex < 0 || nIndex >= m_nSize){
		throw std::out_of_range("index out of range");
	}
	else{
		return m_pData[nIndex];
	}
}

// set the value of an element 
void DArray::SetAt(int nIndex, double dValue) {
	if (nIndex < 0 || nIndex >= m_nSize){
		throw std::out_of_range("index out of range");
	}
	else{
		m_pData[nIndex] = dValue;
	}
}

// overload operator '[]'
const double& DArray::operator[](int nIndex) const {
	return this->GetAt(nIndex); // you should return a correct value
}

// add a new element at the end of the array
void DArray::PushBack(double dValue) {
	double* pTempPtr = new double[m_nSize + 1];
	if (m_pData != nullptr) {
		for (int i = 0; i < m_nSize; i++){
			pTempPtr[i] = m_pData[i];
		}
		pTempPtr[m_nSize] = dValue;
		delete[] m_pData;
		m_nSize += 1 ;
		m_pData = pTempPtr;
	}
	else{
		m_nSize += 1;
		m_pData = new double(m_nSize);
		m_pData[0] = dValue;
	}
}

// delete an element at some index
void DArray::DeleteAt(int nIndex) {
	if (nIndex < 0 || nIndex >= m_nSize){
		throw std::out_of_range("index out of range");
	}
	else {
		m_nSize -= 1;
		if (m_nSize == 0){
			delete[] m_pData;
			m_pData = nullptr;
		}
		else {
			double* pTempPtr = new double[m_nSize];
			for (int i = 0; i < m_nSize + 1; i++) {
				if (i < nIndex) {
					pTempPtr[i] = m_pData[i];
				}
				else if (i > nIndex) {
					pTempPtr[i - 1] = m_pData[i];
				}
			}
			delete[] m_pData;
			m_pData = pTempPtr;
		}
	}
}

// insert a new element at some index
void DArray::InsertAt(int nIndex, double dValue) {
	if (nIndex < 0 || nIndex > m_nSize) {
		throw std::out_of_range("index out of range");
	}
	else {
		if (nIndex == 0) {
			if (m_nSize == 0) {
				this->PushBack(dValue);
			}
			else {
				double* pTempPtr = new double[m_nSize+1];
				pTempPtr[nIndex] = dValue;
				for (int i = 1; i < m_nSize + 1; i++) {
					pTempPtr[i] = m_pData[i - 1];
				}
				delete[] m_pData;
				m_pData = pTempPtr;
				m_nSize += 1;
			}
		}
		else if (nIndex > 0 && nIndex < m_nSize){
			double* pTempPtr = new double[m_nSize + 1];
			for (int i = 0; i < m_nSize + 1; i++) {
				if (i < nIndex) {
					pTempPtr[i] = m_pData[i];
				}
				else if (i > nIndex) {
					pTempPtr[i] = m_pData[i-1];
				}
				else {
					pTempPtr[i] = dValue;
				}
			}
		}
		else {
			this->PushBack(dValue);
		}
	}
}

// overload operator '='
DArray& DArray::operator = (const DArray& arr) {
	m_nSize = arr.m_nSize;
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
