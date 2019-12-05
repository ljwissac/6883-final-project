#include "Group.h"
#include "OpeOverLoad.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
vector<double> vsqrt(const vector<double>& V)
{
	unsigned long d = V.size();
	vector<double> U(d);
	for (int j = 0; j < d; j++) U[j] = sqrt(V[j]);
	return U;
}
double sd(const vector<double> V)
{
	unsigned long d = V.size();
	int temp = 0;
	int tempsq = 0;
	for (int i = 0; i < d; i++)
	{
		temp = (i / (i + 1.0)) * temp + V[i]/(i+1.0);
		tempsq = (i / (i + 1.0)) * temp + V[i] * V[i]/(i+1.0);
	}
	return sqrt(tempsq - temp * temp);
}
void Group::CalStockART(map<string, double>* SPYptr) {
	for (map<string, Stock>::iterator itr = Stockmap.begin(); itr != Stockmap.end(); itr++) {
		itr->second.CalAR(SPYptr);
	}
}
void Group::CalAvgCAAR(int N) {
	AvgCAAR.resize(60);
	AvgAAR.resize(60);
	AvgAARSD.resize(60);
	AvgCAARSD.resize(60);
	vector<double> AARsq(60,0);
	vector<double> CAARsq(60,0);
	//Run sampling for N times
	
	for (int i=0;i < N;i++) {
		vector<int> Sample = GenerateRandomSample(30,i+1);//Random select 30 no-repeat index		
		vector<double> S_AvgARR(60,0);
		vector<double> S_AvgCAAR(60,0);
		
		for (int j = 0;j < Sample.size();j++) {
			S_AvgARR = (j / (j + 1.0)) * S_AvgARR + (1.0 / (j + 1.0)) * Stockmap[Stocklist[Sample[j]]].GetAR();
		}
		S_AvgCAAR[0] = S_AvgARR[0];
		for (int m = 1; m < 60; m++) {
			S_AvgCAAR[m] = S_AvgCAAR[m - 1] + S_AvgARR[m];
		}
		// Update data member AvgAAR
		AvgAAR = (i / (i+1.0)) * AvgAAR + (1.0 / (i+1.0)) * S_AvgARR;
		// Update data member AvgCAAR
		AvgCAAR = (i / (i + 1.0)) * AvgCAAR + (1.0 / (i + 1.0)) * S_AvgCAAR;

		AARsq = (i / (i + 1.0)) * AARsq + (1.0 / (i + 1.0)) * (S_AvgARR * S_AvgARR);
		CAARsq = (i / (i + 1.0)) * CAARsq + (1.0 / (i + 1.0)) * (S_AvgCAAR * S_AvgCAAR);
	}
	AvgAARSD = vsqrt(AARsq - (AvgAAR * AvgAAR));
	AvgCAARSD = vsqrt(CAARsq - (AvgCAAR * AvgCAAR));

	
}
vector<int> Group::GenerateRandomSample(int N,int m)
{
	vector<int> v1;
	int temp;
	srand((int)time(0));
	for (int i = 0; i < N; i++) {
		while (1){
			
			temp = m*rand() % num;
			if (find(v1.begin(), v1.end(), temp) != v1.end())
				continue;
			else
				break;
		}
		v1.push_back(temp);
	}
	return v1;
}