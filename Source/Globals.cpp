#include "Globals.h"
#include <time.h>
#include <fstream>

using namespace std;

bool seeded = false;

int getPseudoRand() {
	return rand();
}
int getRand() {

	if (!seeded) {
		srand((unsigned int)time(nullptr));
		seeded = true;
	}

	return rand();
}

void toFileLog(string message) {
	time_t theTime = time(NULL);
	struct tm aTime;
	localtime_s(&aTime,&theTime);

	int day = aTime.tm_mday;
	int month = aTime.tm_mon + 1; // Month is 0 – 11, add 1 to get a jan-dec 1-12 concept
	int year = aTime.tm_year + 1900; // Year is # years since 1900
	int hour = aTime.tm_hour;
	int min = aTime.tm_min;
	int sec = aTime.tm_sec;

	string path = to_string(day) + "-" + to_string(month) + "-" + to_string(hour) + "_" + to_string(min) + "_" + to_string(sec);
	path = "logs/" + path + ".txt";

	ofstream myfile;
	myfile.open(path.c_str(), std::ios::out);
	myfile << message << "\n";
	myfile.close();
}