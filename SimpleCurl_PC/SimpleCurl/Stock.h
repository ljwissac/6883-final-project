#ifndef Stock_h
#define Stock_h

#include <vector>
#include <string>
#include <map>
using namespace std;

enum GroupType { Beat, Meet, Miss };
enum DataType { AAR, CAAR };

class Stock {
private:
	GroupType group;
	vector<double> Price;
	double actEPS; //actual EPS
	double estEPS; //estimating EPS
	double surp; //surprise
	string DayZero; //releasing date
	string StartTime;
	string EndTime;
	vector<double> AR;
public:
	void SetPrice(double dailyPrice) { Price.push_back(dailyPrice); }
	void SetStartTime(const string& StartTime_) { StartTime = StartTime_; }
	void SetEndTime(const string& EndTime_) { EndTime = EndTime_; }
	void CalAR(map<string, double>* SPYReturn);
	vector<double> GetPrice(void) const { return Price; }
	string GetStartTime(void) const { return StartTime; }
	string GetEndTime(void) const { return EndTime; }
	vector<double> GetAR(void) const { return AR; }
	Stock() {}
	Stock(GroupType group_, double actEPS_, double estEPS_, double surp_,
		string DayZero_, string StartTime_, string EndTime_);
	void Display() const;


};
// Calculate SPY return
map<string, double>* CalReturn(map<string, double> SPY);

#endif
