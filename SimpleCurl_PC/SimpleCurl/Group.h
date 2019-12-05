#ifndef Group_h
#define Group_h

#include<stdlib.h>
#include<time.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "Stock.h"
using namespace std;

class Group {
private:
	vector<string> Stocklist; //a vector containing all the ticker of the stocks in that Group
	vector<double> AvgCAAR;
	vector<double> AvgCAARSD;
	map<string, Stock> Stockmap;// Map that contains all stocks
	vector<double> AvgAAR;
	vector<double>AvgAARSD;
	int num;
public:
	vector<string> GetTickerlist(void) { return Stocklist; }
	vector<double> GetAvgAAR(void) { return AvgAAR;}
	vector<double> GetAvgCAAR(void) { return AvgCAAR; }
	vector<double> GetAvgCAARSD(void) { return AvgCAARSD; }
	vector<double> GetAvgAARSD(void) { return AvgAARSD; }
	void CalStockART(map<string, double>* SPYptr);
	void CalAvgCAAR(int N);//Repeat steps N times 
	void Set(vector<string> Stocklist_, map<string, Stock> Stockmap_) { Stocklist = Stocklist_; num = Stocklist.size(); Stockmap = Stockmap_; }
	vector<int> GenerateRandomSample(int N,int m);//Select no-repeat N stocksindex from stocklist 
	Group(vector<string> Stocklist_, map<string, Stock> Stockmap_) :Stocklist(Stocklist_), num(Stocklist.size()), Stockmap(Stockmap_) {}
	Group() {}



};

#endif 
