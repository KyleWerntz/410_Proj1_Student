/***
 * KP Utils
 */
#include <numeric>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "utilities.h"
#include "stdlib.h"

using namespace std;

//********************** private to this compilation unit **********************
vector<process_stats> stats;
vector<string> split(string s);
bool compare_process_number(process_stats a, process_stats b);
bool compare_start_time(process_stats a, process_stats b);
bool compare_cpu_time(process_stats a, process_stats b);
bool compare_io_time(process_stats a, process_stats b);


//returns the row of data split into a vector of strings

//if myString does not contain a string rep of number returns o
//if int not large enough has undefined behaviour, very fragile
int stringToInt(const char *myString) {
	return atoi(myString);
}

int loadData(const char* filename, bool ignoreFirstRow) {

	stats.clear();
	try	{
		ifstream file(filename);
		string line;
		vector<string> data;

		if (!file.is_open())
			return COULD_NOT_OPEN_FILE;
		else if (ignoreFirstRow)
			getline(file, line);

		while (file.peek() != ifstream::traits_type::eof())	{
			getline(file, line);
			data = split(line);
			bool valid = true;
			if (data.size() != 4)	{
				valid = false;
			}
			if (valid)	{
				for (string s : data)	{
					if (s == "")
						valid = false;
				}
			}

			if (valid)	{
				process_stats p;
				p.process_number = stringToInt(data.at(0).c_str());
				p.start_time = stringToInt(data.at(1).c_str());
				p.cpu_time = stringToInt(data.at(2).c_str());
				p.io_time = stringToInt(data.at(3).c_str());
				stats.push_back(p);
			}
		}
		return SUCCESS;

	} catch(fstream::failure& err)	{
		return COULD_NOT_OPEN_FILE;
	}

	return COULD_NOT_OPEN_FILE;
}


//will sort according to user preference
void sortData(SORT_ORDER mySortOrder) {
	switch (mySortOrder)	{
	case CPU_TIME:
		sort(stats.begin(), stats.end(), compare_cpu_time);
		break;

	case PROCESS_NUMBER:
		sort(stats.begin(), stats.end(), compare_process_number);
		break;

	case START_TIME:
		sort(stats.begin(), stats.end(), compare_start_time);
		break;

	case IO_TIME:
		sort(stats.begin(), stats.end(), compare_io_time);
		break;
	}
}

process_stats getNext() {
	process_stats myFirst;
	if (stats.size() != 0)	{
		process_stats myFirst = stats.at(0);
	}

	stats.erase(stats.begin());
	return myFirst;
}

//returns number of process_stats structs in the vector holding them
int getNumbRows(){
	return stats.size();
}

vector<string> split(string s)	{
	vector<string> ans;
	int start = 0;
	for (int i = 0; i < s.size(); i++)	{
		if (s[i] == ',')	{
			ans.push_back(s.substr(start,i));
			start = i+1;
		} else if (i == s.size()-1)	{
			ans.push_back(s.substr(start));
		}
	}

	return ans;
}

bool compare_process_number(process_stats a, process_stats b)	{
	return (a.process_number < b.process_number);
}

bool compare_start_time(process_stats a, process_stats b)	{
	return (a.start_time < b.start_time);
}

bool compare_cpu_time(process_stats a, process_stats b)	{
	return (a.cpu_time < b.cpu_time);
}

bool compare_io_time(process_stats a, process_stats b)	{
	return (a.io_time < b.io_time);
}

