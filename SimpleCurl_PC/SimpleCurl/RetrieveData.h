#ifndef RetrieveData_h
#define RetrieveData_h

#include "Stock.h"
#include <map>
#include <string>

using namespace std;

// load EPS-related data from csv and store them in Stock objects
void ReadCsv(string filename, vector<string>& tickerList,
	map<string, Stock>& tickerMap, GroupType group);
// download price data for one group of stocks from Yahoo Finance and store them
int RetrieveFromYahoo(map<string, Stock>& tickerMap);
// download prices for SPY during 2017-2018
int RetrieveSPY(map<string, double>& SPYPriceList);
#endif#pragma once
