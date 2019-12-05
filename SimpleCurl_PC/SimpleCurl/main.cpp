#include <iostream>
#include "OpeOverLoad.h"
#include "Group.h"
#include <map>
#include "Stock.h"
#include "RetrieveData.h"
#include <vector>
using namespace std;

int main()
{
	// Initializing empty maps and lists for future use
	vector<string> BeatList, MeetList, MissList;
	map<string, Stock> BeatMap, MeetMap, MissMap;
	map<string, double> SPY;
	map<string, double>* SPYptr = nullptr;
	// Initializing null pointer for future use
	Group* BeatPtr = nullptr;
	Group* MeetPtr = nullptr;
	Group* MissPtr = nullptr;
	// Initializing empty vectors and matrixes for future use
	Matrix DataMatirx;
	VectorV BeatVector(4);
	VectorV MeetVector(4);
	VectorV MissVector(4);

	int Option = 0;

	//Use do...while sytax to help user navigate back to main menu
	do
	{
		cout << "Please enter the options' number to proceed:" << endl;
		cout << "1. Retrieve historical price data for all stocks.(This option MUST be run first)" << endl
			 << "2. Pull information for on stock from one group." << endl
			 << "3. Show AAR or CAAR for one group." << endl
			 << "4. Show the Gunplot graphs with CAAR for all 3 groups." << endl
			 << "5. Exit your program" << endl;
		cout << endl;
		cout << "Enter your option: "; cin >> Option;

		

		switch (Option)
		{
		case 1: {
			//Populate empty lists and maps
			ReadCsv("beat1.csv", BeatList, BeatMap, Beat);RetrieveFromYahoo(BeatMap);
			ReadCsv("meet1.csv", MeetList, MeetMap, Meet);
			ReadCsv("miss1.csv", MissList, MissMap, Miss);
			
			RetrieveFromYahoo(MeetMap);
			RetrieveFromYahoo(MissMap);
			cout << "beat amount:" << BeatList.size() << endl;
			cout << "meet amount:" << MeetList.size() << endl;
			cout << "Miss amount:" << MissList.size() << endl;
			RetrieveSPY(SPY);
			SPYptr = CalReturn(SPY);
			cout << "Retrieve completed" << endl;
			break;
		}

		case 2: {
			char temp;
			// Test whether user run option 1 firstly
			if (BeatList.size() == 0 || MeetList.size() == 0 || MissList.size() == 0)
			{
				cout << "WARNING: Option 1 need to be run first." << endl << endl << endl;
				break;
			}

			do {
				string TickerName;
				cout << "Please enter the stock's ticker: "; cin >> TickerName;
				int isSP = 0;
				for (int i = 0; i < BeatList.size(); i++)
				{
					if (BeatList[i] == TickerName) { BeatMap[TickerName].Display(); isSP = 1; }
				}
				for (int i = 0; i < MeetList.size(); i++)
				{
					if (MeetList[i] == TickerName) { MeetMap[TickerName].Display(); isSP = 1; }
				}
				for (int i = 0; i < MissList.size(); i++)
				{
					if (MissList[i] == TickerName) { MissMap[TickerName].Display(); isSP = 1; }
				}
				if (isSP == 0) cout << "Your Ticker is not in S&P500" << endl;
				cout << endl;
				cout << "Do you want to look for other stock's information? (Y/N) "; cin >> temp;
			} while (temp == 'Y' || temp == 'y');
			break;
		}

		case 3: {
			if (BeatList.size() == 0 || MeetList.size() == 0 || MissList.size() == 0)
			{
				cout << "WARNING: Option 1 need to be run first." << endl << endl << endl;
				break;
			}
			Group *BeatPtr = new Group(BeatList, BeatMap);
			Group* MeetPtr = new Group(MeetList, MeetMap);
			Group* MissPtr = new Group(MissList, MissMap);
			
			BeatPtr->CalStockART(SPYptr);
			MeetPtr->CalStockART(SPYptr);
			MissPtr->CalStockART(SPYptr);


			BeatPtr->CalAvgCAAR(2);
			MeetPtr->CalAvgCAAR(2);
			MissPtr->CalAvgCAAR(2);

			BeatVector[0] = BeatPtr->GetAvgAAR();
			BeatVector[1] = BeatPtr->GetAvgAARSD();
			BeatVector[2] = BeatPtr->GetAvgCAAR();
			BeatVector[3] = BeatPtr->GetAvgCAARSD();



			MeetVector[0] = MeetPtr->GetAvgAAR();
			MeetVector[1] = MeetPtr->GetAvgAARSD();
			MeetVector[2] = MeetPtr->GetAvgCAAR();
			MeetVector[3] = MeetPtr->GetAvgCAARSD();


			MissVector[0] = MissPtr->GetAvgAAR();
			MissVector[1] = MissPtr->GetAvgAARSD();
			MissVector[2] = MissPtr->GetAvgCAAR();
			MissVector[3] = MissPtr->GetAvgCAARSD();

			DataMatirx.push_back(BeatVector);
			DataMatirx.push_back(MeetVector);
			DataMatirx.push_back(MissVector);
			
			int GroupName;
			int DataName;
			string GroupString[3] = { "Beat", "Meet", "Miss" };
			string DataString[4] = { "AAR","AARSD", "CAAR","CAARSD" };

			char temp;
			do {
				cout << "1.Beat   2.Meet   3.Miss:  "; cin >> GroupName;
				cout << "1.AAR    2.AARSD  3.CAAR   4.CAARSD:  "; cin >> DataName;
				cout << GroupString[GroupName - 1] << " " << DataString[DataName - 1] << endl;
				cout << DataMatirx[GroupName - 1][DataName - 1] << endl;
				cout << "Do you want to look for other data? (Y/N)  "; cin >> temp;
				cout << endl;
			} while (temp == 'Y' || temp == 'y');

			
			break;
			
			
			
			

		}


		


		case 5: {
			cout << "Promgram terminated!" << endl
				<< endl
				<< endl
				<< endl
				<< endl
				<< "Good luck with your finals and have a good summer vacation!!!!!!!!" << endl
				<< endl
				<< endl;
			break;

		default: {
			cout << endl << "Invalid input. Input should be intenger within range [1,5]" << endl << endl;
			Option = 0; //Reset Option
			break;
		}
		}
		}
	} while (Option >= 0 && Option < 5);

	delete BeatPtr;
	BeatPtr = NULL;
	delete MeetPtr;
	MeetPtr = NULL;
	delete MissPtr;
	MissPtr = NULL;
	delete SPYptr;
	SPYptr = NULL;

	return 0;
}