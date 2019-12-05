#include "OpeOverLoad.h"
#include <cmath>

vector<double> operator+(const vector<double>& V, const vector<double>& W)
{
	unsigned long d = V.size();
	vector<double> U(d);
	for (int j = 0; j < d; j++) U[j] = V[j] + W[j];
	return U;
}

vector<double> operator-(const vector<double>& V, const vector<double>& W)
{
	unsigned long d = V.size();
	vector<double> U(d);
	for (int j = 0; j < d; j++) U[j] = V[j] - W[j];
	return U;
}

vector<double> operator*(const double& a, const vector<double>& V)
{
	unsigned long d = V.size();
	vector<double> U(d);
	for (int j = 0; j < d; j++) U[j] = a * V[j];
	return U;
}

vector<double> operator/(const vector<double>& V, const double& a)
{
	unsigned long d = V.size();
	vector<double> U(d);
	for (int j = 0; j < d; j++) U[j] = V[j] / a;
	return U;
}

vector<double> operator*(const vector<double>& V, const vector<double>& W)
{
	unsigned long d = V.size();
	vector<double> U(d);
	for (int j = 0; j < d; j++) U[j] = V[j]*W[j];
	return U;
}

ostream& operator<<(ostream& out, VectorD& V)
{
	int i = -30;
	for (VectorD::iterator itr = V.begin(); itr != V.end(); itr++)
	{
		out << i << " : " << *itr << endl;
		i++;
	}
	return out;
}