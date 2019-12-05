#include "Stock.h"
#include <iostream>
using namespace std;

Stock::Stock(GroupType group_, double actEPS_, double estEPS_, double surp_,
	string DayZero_, string StartTime_, string EndTime_) : group(group_), actEPS(actEPS_),
	estEPS(estEPS_), surp(surp_), DayZero(DayZero_), StartTime(StartTime_), EndTime(EndTime_) {}

void Stock::CalAR(map<string, double>* SPYReturn) {
	AR.resize(Price.size()-1);
	vector<double>::iterator itr = Price.begin();
	itr++;
	auto it = SPYReturn->find(StartTime);
	it++;
	for (int i = 0;itr != Price.end();itr++) {
		AR[i] = (*itr - *(itr - 1)) / (*(itr - 1)) - (it->second);
		i++;
		it++;
	}
}

void Stock::Display() const
{
	string GroupString[3] = { "Beat","Meet","Miss" };
	cout << "Group: " << GroupString[group] << endl;
	cout << "Actual EPS: " << actEPS << endl;
	cout << "Estimating EPS: " << estEPS << endl;
	cout << "EPS surprise: " << surp << '%' << endl;
	cout << "Acutal EPS Releasing Date (Day Zero): " << DayZero << endl;
	cout << "Price for 61 days:" << endl;
	int i = -30;
	for (vector<double>::const_iterator itr = Price.begin(); itr != Price.end(); itr++)
	{
		cout <<"Day "<<i<<" Price:"<< *itr <<endl;
		i++;
	}
	cout << endl;
}

map<string, double>* CalReturn(map<string, double> SPY) {
	map<string, double>::iterator itr = SPY.begin();
	map<string, double>* ptr = new map<string, double>;
	for (;;) {
		double PriceToday = itr->second;
		if (++itr == SPY.end()) break;
		(*ptr)[itr->first] = ((itr)->second - PriceToday) / (PriceToday);
	}
	return ptr;
}
