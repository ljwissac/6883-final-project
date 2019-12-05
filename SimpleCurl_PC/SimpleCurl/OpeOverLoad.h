#ifndef OpeOverLoad_h
#define OpeOverLoad_h

#include "Stock.h"
#include <vector>
#include <iostream>
using namespace std;

typedef vector <double> VectorD;
typedef vector <VectorD> VectorV;
typedef vector <VectorV> Matrix;

vector<double> operator*(const double& a, const vector<double>& V);
vector<double> operator/(const vector<double>& V, const double& a);
vector<double> operator+(const vector<double>& V, const vector<double>& W);
vector<double> operator-(const vector<double>& V, const vector<double>& W);
vector<double> operator*(const vector<double>& V, const vector<double>& W);
ostream& operator<<(ostream& out, VectorD& V);

#endif #pragma once
