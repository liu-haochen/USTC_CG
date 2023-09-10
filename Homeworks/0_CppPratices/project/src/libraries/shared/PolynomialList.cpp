#include "PolynomialList.h"
#include<fstream>
#include<iostream>
#include<regex>
using namespace std;

PolynomialList::PolynomialList(const PolynomialList& other) {
    Term TTemp;
    if (!other.m_Polynomial.empty()) {
        for (auto& it : other.m_Polynomial) {
            TTemp.deg = it.deg;
            TTemp.cof = it.cof;
            m_Polynomial.push_back(TTemp);
        }
    }
}

PolynomialList::PolynomialList(const string& file) {
    this->ReadFromFile(file);
}

PolynomialList::PolynomialList(const double* cof, const int* deg, int n) {
    Term TTemp;
    for (int i = 0; i < n; i++) {
        TTemp.cof = cof[i];
        TTemp.deg = deg[i];
        m_Polynomial.push_back(TTemp);
    }
    this->compress();
}

PolynomialList::PolynomialList(const vector<int>& deg, const vector<double>& cof) {
    if (deg.size() != cof.size()) {
        cerr << "the size of deg and cof isn't match";
    }
    else {
        Term TTemp;
        int n = deg.size();
        for (int i = 0; i < n; i++) {
            TTemp.cof = cof[i];
            TTemp.deg = deg[i];
            m_Polynomial.push_back(TTemp);
        }
        this->compress();
    }
}

double PolynomialList::coff(int i) const {
    if (!m_Polynomial.empty()) {
        for (auto& TTerm : m_Polynomial) {
            if (TTerm.deg == i) {
                return TTerm.cof;
            }
        }
    }
    return 0.; // you should return a correct value
}

double& PolynomialList::coff(int i) {
    if (!m_Polynomial.empty()) {
        for (auto& TTerm : m_Polynomial) {
            if (TTerm.deg == i) {
                return TTerm.cof;
            }
        }
    }
    else {
        Term TTerm(i,0);
        m_Polynomial.push_back(TTerm);
        auto& it = m_Polynomial.end();
        it--;
        return it->cof;
    }
}

void PolynomialList::compress() {
    if (!m_Polynomial.empty()) {
        vector<int> vDeg;
        vector<double> vCof;
        for (auto& TTerm : m_Polynomial) {
            vDeg.push_back(TTerm.deg);
            vCof.push_back(TTerm.cof);
        }
        vector<int> vDegTemp;
        vDegTemp.assign(vDeg.begin(), vDeg.end());
        sort(vDegTemp.begin(), vDegTemp.end());
        vDegTemp.erase(unique(vDegTemp.begin(), vDegTemp.end()), vDegTemp.end());
        vector<double> vCofTemp;
        int iLenth = vDegTemp.size();
        vCofTemp.resize(iLenth, 0);
        for (int i = 0; i < vDeg.size(); i++) {
            for (int j = 0; j < iLenth; j++) {
                if (vDegTemp[j] == vDeg[i]) {
                    vCofTemp[j] += vCof[i];
                    break;
                }
            }
        }
        Term tTemp;
        m_Polynomial.clear();
        for (int i = 0; i < iLenth; i++) {
            tTemp.deg = vDegTemp[i];
            tTemp.cof = vCofTemp[i];
            m_Polynomial.push_back(tTemp);
        }
    }
    for (auto& it = m_Polynomial.begin(); it != m_Polynomial.end();) {
        if (it->cof == 0) {
            it = m_Polynomial.erase(it);
        }
        else {
            it++;
        }
    }
}

PolynomialList PolynomialList::operator+(const PolynomialList& right) const {
    vector<int> vDeg;
    vector<double> vCof;
    if (!m_Polynomial.empty()) {
        for (auto& TTerm : m_Polynomial) {
            vDeg.push_back(TTerm.deg);
            vCof.push_back(TTerm.cof);
        }
    }
    if (!right.m_Polynomial.empty()) {
        for (auto& TTerm : right.m_Polynomial) {
            vDeg.push_back(TTerm.deg);
            vCof.push_back(TTerm.cof);
        }
    }
    PolynomialList TPolynomialList(vDeg,vCof) ;
    TPolynomialList.compress();
    return TPolynomialList; // you should return a correct value
}

PolynomialList PolynomialList::operator-(const PolynomialList& right) const {
    vector<int> vDeg;
    vector<double> vCof;
    if (!m_Polynomial.empty()) {
        for (auto& TTerm : m_Polynomial) {
            vDeg.push_back(TTerm.deg);
            vCof.push_back(TTerm.cof);
        }
    }
    if (!right.m_Polynomial.empty()) {
        for (auto& TTerm : right.m_Polynomial) {
            vDeg.push_back(TTerm.deg);
            vCof.push_back(-TTerm.cof);
        }
    }
    PolynomialList TPolynomialList(vDeg, vCof);
    TPolynomialList.compress();
    return TPolynomialList; // you should return a correct value
}

PolynomialList PolynomialList::operator*(const PolynomialList& right) const {
    vector<int> vDeg;
    vector<double> vCof;
    if (!m_Polynomial.empty() && !right.m_Polynomial.empty()) {
        for (auto& LTerm : m_Polynomial) {
            for (auto&& RTerm : right.m_Polynomial) {
                vDeg.push_back(LTerm.deg + RTerm.deg);
                vCof.push_back(LTerm.cof * RTerm.cof);
            }
        }
    }
    PolynomialList TPolynomialList(vDeg, vCof);
    TPolynomialList.compress();
    return TPolynomialList; // you should return a correct value
}

PolynomialList& PolynomialList::operator=(const PolynomialList& right) {
    m_Polynomial.clear();
    for (auto& TTerm : right.m_Polynomial) {
        m_Polynomial.push_back(TTerm);
    }
    return *this;
}

void PolynomialList::Print() const {
    if (!m_Polynomial.empty()) {
        for (auto& itT = m_Polynomial.begin(); itT != m_Polynomial.end(); itT++) {
            if (next(itT) != m_Polynomial.end()) {
                if (itT->deg == 0) {
                    cout << itT->cof << " + ";
                }
                else {
                    cout << itT->cof << "x^" << itT->deg << " + ";
                }
            }
            else {
                if (itT->deg == 0) {
                    cout << itT->cof;
                }
                else {
                    cout << itT->cof << "x^" << itT->deg;
                }
            }
        }
        cout << endl;
    }
    else {
        cout << "0 "<<endl;
    }
}

bool PolynomialList::ReadFromFile(const string& file) {
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
            //std::cout << stoi((*sregxIter)[1]) << " " << stoi((*sregxIter)[2]) << std::endl;
        }
        Term TTemp;
        for (int i = 0; i < vDeg.size(); i++) {
            TTemp.cof = vCof[i];
            TTemp.deg = vDeg[i];
            m_Polynomial.push_back(TTemp);
        }
        this->compress();
        //vector<int> vDegTemp;
        //vDegTemp.assign(vDeg.begin(), vDeg.end());
        //sort(vDegTemp.begin(),vDegTemp.end());
        //vDegTemp.erase(unique(vDegTemp.begin(), vDegTemp.end()), vDegTemp.end());
        //vector<double> vCofTemp;
        //int iLenth = vDegTemp.size();
        //vCofTemp.resize(iLenth,0);
        //for (int i = 0; i < vDeg.size(); i++) {
        //    for (int j = 0; j < iLenth; j++) {
        //        if (vDegTemp[j] == vDeg[i]) {
        //            vCofTemp[j] += vCof[i];
        //            break;
        //        }
        //    }
        //}
        //Term tTemp;
        //for (int i = 0; i < iLenth; i++) {
        //    tTemp.deg = vDegTemp[i];
        //    tTemp.cof = vCofTemp[i];
        //    m_Polynomial.push_back(tTemp);
        //}
    }
    return bFlag; // you should return a correct value
}

PolynomialList::Term& PolynomialList::AddOneTerm(const Term& term) {
    m_Polynomial.push_back(term);
    this->compress();
    for (auto& Tit : m_Polynomial) {
        if (Tit.deg == term.deg) {
            return Tit;
        }
    }
    return *(new Term(term.deg, 0)); // you should return a correct value
}
