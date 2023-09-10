#include "PolynomialMap.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <regex>
using namespace std;

PolynomialMap::PolynomialMap(const PolynomialMap& other) {
    m_Polynomial = other.m_Polynomial;
    this->compress();
}

PolynomialMap::PolynomialMap(const string& file) {
    ReadFromFile(file);
    this->compress();
}

PolynomialMap::PolynomialMap(const double* cof, const int* deg, int n) {
    for (int i = 0; i < n; i++) {
        auto it = m_Polynomial.find(deg[i]);
        auto itEnd = m_Polynomial.end();
        m_Polynomial[deg[i]] = (it != m_Polynomial.end()) ? m_Polynomial[deg[i]] + cof[i] : cof[i];
    }
    this->compress();
}

PolynomialMap::PolynomialMap(const vector<int>& deg, const vector<double>& cof) {
	assert(deg.size() == cof.size());
    int n = deg.size();
    for (int i = 0; i < n; i++) {
        auto it = m_Polynomial.find(deg[i]);
        auto itEnd = m_Polynomial.end();
        m_Polynomial[deg[i]] = (it != m_Polynomial.end()) ? m_Polynomial[deg[i]] + cof[i] : cof[i];
    }
    this->compress();
}

double PolynomialMap::coff(int i) const {
    auto iFind = m_Polynomial.find(i);
    if (iFind != m_Polynomial.end()) {
        return iFind->second;
    }
    else{
        return 0;
    }
}

double& PolynomialMap::coff(int i) {
    auto iFind = m_Polynomial.find(i);
    if (iFind != m_Polynomial.end()) {
        return iFind->second;
    }
    else {
        m_Polynomial[i] = 0;
        return m_Polynomial[i];
    }
}

void PolynomialMap::compress() {
    for (auto& it = m_Polynomial.begin(); it != m_Polynomial.end();){
        if (it->second==0) {
            it=m_Polynomial.erase(it);
        }
        else {
            it++;
        }
    }
}

PolynomialMap PolynomialMap::operator+(const PolynomialMap& right) const {
    PolynomialMap PolyTemp;
    for (auto& it : m_Polynomial) {
        auto itF = PolyTemp.m_Polynomial.find(it.first);
        auto itE = PolyTemp.m_Polynomial.end();
        PolyTemp.m_Polynomial[it.first] = (itF != itE ? PolyTemp.m_Polynomial[it.first] + it.second:it.second );
    }
    for (auto& it : right.m_Polynomial) {
        auto itF = PolyTemp.m_Polynomial.find(it.first);
        auto itE = PolyTemp.m_Polynomial.end();
        PolyTemp.m_Polynomial[it.first] = (itF != itE ? PolyTemp.m_Polynomial[it.first] + it.second : it.second);
    }
    PolyTemp.compress();
	return PolyTemp; // you should return a correct value
}

PolynomialMap PolynomialMap::operator-(const PolynomialMap& right) const {
    PolynomialMap PolyTemp;
    for (auto& it : m_Polynomial) {
        auto itF = PolyTemp.m_Polynomial.find(it.first);
        auto itE = PolyTemp.m_Polynomial.end();
        PolyTemp.m_Polynomial[it.first] = (itF != itE ? PolyTemp.m_Polynomial[it.first] + it.second : it.second);
    }
    for (auto& it : right.m_Polynomial) {
        auto itF = PolyTemp.m_Polynomial.find(it.first);
        auto itE = PolyTemp.m_Polynomial.end();
        PolyTemp.m_Polynomial[it.first] = (itF != itE ? PolyTemp.m_Polynomial[it.first] - it.second : -it.second);
    }
    PolyTemp.compress();
    return PolyTemp; // you should return a correct value
}

PolynomialMap PolynomialMap::operator*(const PolynomialMap& right) const {
    PolynomialMap PolyTemp;
    for (auto& itL : m_Polynomial) {
        for (auto& itR : right.m_Polynomial) {
            int nDeg = itL.first + itR.first;
            auto itF = PolyTemp.m_Polynomial.find(nDeg);
            auto itE = PolyTemp.m_Polynomial.end();
            PolyTemp.m_Polynomial[nDeg] = (itF != itE ? PolyTemp.m_Polynomial[nDeg] + itL.second*itR.second : itL.second * itR.second);
        }
    }
    PolyTemp.compress();
	return PolyTemp; // you should return a correct value
}

PolynomialMap& PolynomialMap::operator=(const PolynomialMap& right) {
    m_Polynomial = right.m_Polynomial;
    this->compress();
	return *this;
}

void PolynomialMap::Print() const {
    for (auto& it=m_Polynomial.begin(); it != m_Polynomial.end(); it++) {
        cout << it->second << "x^" << it->first << (next(it)!= m_Polynomial.end()? " + " : "\n");
    }
    //cout << m_Polynomial.size()<<endl;
}

bool PolynomialMap::ReadFromFile(const string& file) {
    m_Polynomial.clear();
    ifstream ifs;
    ifs.open(file, ios::in);
    bool bFlag = ifs.is_open();
    if (!bFlag) {
        cerr << "cannot open file" << endl;
    }
    else {
        string strPoly;
        string strLine;
        while (getline(ifs, strLine)) {
            strPoly += strLine + "\n";
        }
        //std::cout << strPoly<<std::endl;
        ifs.close();
        vector<int> vDeg;
        vector<double> vCof;
        regex regNum(R"(([-+]?\d+) ([-+]?\d+(\.\d+)?))");
        sregex_iterator sregxIter(strPoly.begin(), strPoly.end(), regNum);
        for (sregex_iterator end; sregxIter != end; sregxIter++) {
            vDeg.push_back(stoi((*sregxIter)[1]));
            vCof.push_back(stod((*sregxIter)[2]));
        }
        for (int i = 0; i < vDeg.size(); i++) {
            auto it = m_Polynomial.find(vDeg[i]);
            auto itEnd = m_Polynomial.end();
            m_Polynomial[vDeg[i]] = (it != m_Polynomial.end()) ? m_Polynomial[vDeg[i]] + vCof[i] : vCof[i];
        }
        this->compress();
    }
    return bFlag; // you should return a correct value
}
