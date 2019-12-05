#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>


#include <locale>
#include <iomanip>

#include "curl.h"
#include "Stock.h"
#include "RetrieveData.h"
using namespace std;

void ReadCsv(string filename, vector<string>& tickerList, map<string,
	Stock>& tickerMap, GroupType group)
{
	ifstream fin(filename);
	string line, ticker, DayZero, sActEPS, sEstEPS, sSurp, StartTime, EndTime;
	while (getline(fin, line))
	{
		istringstream sin(line);
		getline(sin, ticker, ',');
		getline(sin, DayZero, ',');
		getline(sin, sActEPS, ',');
		getline(sin, sEstEPS, ',');
		getline(sin, sSurp, ',');
		getline(sin, StartTime, ',');
		getline(sin, EndTime, ',');
		double actEPS = strtod(sActEPS.c_str(), NULL);
		double estEPS = strtod(sEstEPS.c_str(), NULL);
		double surp = strtod(sSurp.c_str(), NULL);
		//ticker = ticker.substr(0, ticker.length() - 3);
		tickerList.push_back(ticker);
		tickerMap[ticker] = Stock(group, actEPS, estEPS, surp,
			DayZero, StartTime, EndTime);
	}
}

int write_data(void* ptr, int size, int nmemb, FILE* stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

struct MemoryStruct {
	char* memory;
	size_t size;
};

void* myrealloc(void* ptr, size_t size)
{
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}


int write_data2(void* ptr, size_t size, size_t nmemb, void* data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)data;

	mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

string getTimeinSeconds(string Time)
{
	std::tm t = { 0 };
	std::istringstream ssTime(Time);
	char time[100];
	memset(time, 0, 100);
	if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
	{
		sprintf(time, "%lld", mktime(&t));
		return string(time);
	}
	else
	{
		cout << "Parse failed\n";
		return "";
	}
}

// retrieve the 61-day prices for one group
int RetrieveFromYahoo(map<string, Stock>& tickerMap)
{
	ofstream fout;
	fout.open("text.txt");
	map<string, Stock>::iterator itr = tickerMap.begin();
	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;

	// file pointer to create file that store the data
	FILE* fp;

	// declaration of an object CURL
	CURL* handle;

	CURLcode result;

	// set up the program environment that libcurl needs
	curl_global_init(CURL_GLOBAL_ALL);

	// curl_easy_init() returns a CURL easy handle
	handle = curl_easy_init();

	// if everything's all right with the easy handle...
	if (handle)
	{
		string sCookies, sCrumb;
		if (sCookies.length() == 0 || sCrumb.length() == 0)
		{
			curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
			curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
			curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

			// perform, then store the expected code in result
			result = curl_easy_perform(handle);

			if (result != CURLE_OK)
			{
				// if errors have occured, tell us what is wrong with result
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}

			char cKey[] = "CrumbStore\":{\"crumb\":\"";
			char* ptr1 = strstr(data.memory, cKey);
			char* ptr2 = ptr1 + strlen(cKey);
			char* ptr3 = strstr(ptr2, "\"}");
			if (ptr3 != NULL)
				*ptr3 = NULL;

			sCrumb = ptr2;

			fp = fopen("cookies.txt", "r");
			char cCookies[100];
			if (fp) {
				while (fscanf(fp, "%s", cCookies) != EOF);
				fclose(fp);
			}
			else
				cerr << "cookies.txt does not exists" << endl;

			sCookies = cCookies;

		}
		while (true)
		{
			free(data.memory);
			data.memory = NULL;
			data.size = 0;
			if (itr == tickerMap.end())
				break;

			string StartTime_ = itr->second.GetStartTime();
			string EndTime_ = itr->second.GetEndTime();
			string startTime = getTimeinSeconds(StartTime_);
			string endTime = getTimeinSeconds(EndTime_);

			string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
			string symbol = itr->first;
			string urlB = "?period1=";
			string urlC = "&period2=";
			string urlD = "&interval=1d&events=history&crumb=";
			string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
			const char* cURL = url.c_str();
			const char* cookies = sCookies.c_str();
			curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
			curl_easy_setopt(handle, CURLOPT_URL, cURL);
			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
			result = curl_easy_perform(handle);

			if (result != CURLE_OK)
			{
				// if errors have occurred, tell us what is wrong with result
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}

			stringstream sData;
			sData.str(data.memory);
			string sValue, sDate;
			double dValue = 0;
			string line;
			getline(sData, line);
			//cout << itr->first << endl; // cout the ticker to show the process of retrieving data
			//fout << itr->first << endl;
			while (getline(sData, line)) {
				//fout << line << endl;
				line.erase(line.find_last_of(','));
				sValue = line.substr(line.find_last_of(',') + 1);
				dValue = strtod(sValue.c_str(), NULL);
				
				itr->second.SetPrice(dValue);
			}
			// if the data retrieved from Yahoo is not correct, cout a string
			if (itr->second.GetPrice().size() != 61)
				cout << "Error: amount of data is incorrect"<<endl<<itr->first << endl;
			itr++;
		}
		free(data.memory);
		data.size = 0;
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}

	// cleanup since you've used curl_easy_init
	curl_easy_cleanup(handle);

	// release resources acquired by curl_global_init()
	curl_global_cleanup();
	fout.close();
	return 0;
}

int RetrieveSPY(map<string, double>&SPYPriceList)
{
	ofstream fout;
	fout.open("spy.txt");
	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;

	// file pointer to create file that store the data
	FILE* fp;

	// declaration of an object CURL
	CURL* handle;

	CURLcode result;

	// set up the program environment that libcurl needs
	curl_global_init(CURL_GLOBAL_ALL);

	// curl_easy_init() returns a CURL easy handle
	handle = curl_easy_init();

	// if everything's all right with the easy handle...
	if (handle)
	{
		string sCookies, sCrumb;
		if (sCookies.length() == 0 || sCrumb.length() == 0)
		{
			//curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
			curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
			curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
			curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

			// perform, then store the expected code in result
			result = curl_easy_perform(handle);

			if (result != CURLE_OK)
			{
				// if errors have occured, tell us what is wrong with result
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}

			char cKey[] = "CrumbStore\":{\"crumb\":\"";
			char* ptr1 = strstr(data.memory, cKey);
			char* ptr2 = ptr1 + strlen(cKey);
			char* ptr3 = strstr(ptr2, "\"}");
			if (ptr3 != NULL)
				*ptr3 = NULL;

			sCrumb = ptr2;

			fp = fopen("cookies.txt", "r");
			char cCookies[100];
			if (fp) {
				while (fscanf(fp, "%s", cCookies) != EOF);
				fclose(fp);
			}
			else
				cerr << "cookies.txt does not exists" << endl;

			sCookies = cCookies;

		}
		free(data.memory);
		data.memory = NULL;
		data.size = 0;

		// 2-year time period to ensure it covers all stocks' 61-day price lists
		string StartTime_ = "2018-01-T16:00:00";
		string EndTime_ = "2019-12-30T16:00:00";
		string startTime = getTimeinSeconds(StartTime_);
		string endTime = getTimeinSeconds(EndTime_);

		string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
		string symbol = "SPY";
		string urlB = "?period1=";
		string urlC = "&period2=";
		string urlD = "&interval=1d&events=history&crumb=";
		string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
		const char* cURL = url.c_str();
		const char* cookies = sCookies.c_str();
		curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
		curl_easy_setopt(handle, CURLOPT_URL, cURL);

		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
		result = curl_easy_perform(handle);

		if (result != CURLE_OK)
		{
			// if errors have occurred, tell us what is wrong with result
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			return 1;
		}

		stringstream sData;
		sData.str(data.memory);
		string sValue, sDate;
		double dValue = 0;
		string line;
		getline(sData, line);
		while (getline(sData, line)) {
			fout << line << endl;
			sDate = line.substr(0, line.find_first_of(','));
			sDate += "T16:00:00";
			line.erase(line.find_last_of(','));
			sValue = line.substr(line.find_last_of(',') + 1);
			dValue = strtod(sValue.c_str(), NULL);
			SPYPriceList[sDate] = dValue; // save date and price for SPY in the map
		}
		free(data.memory);
		data.size = 0;
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}

	// cleanup since you've used curl_easy_init
	curl_easy_cleanup(handle);

	// release resources acquired by curl_global_init()
	curl_global_cleanup();

	return 0;
}