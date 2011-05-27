#include "accsample.h"

using namespace std;

AccSample::AccSample(float x, float y, float z)
	: Sample() 
{ 
	ax = x; 
	ay = y; 
	az = z; 
	logType = LOG_ACC;
}

AccSample::AccSample(const string& line)
	: Sample()
{
	istringstream inLine(line);
	inLine >> ax >> ay >> az; 
	logType = LOG_ACC;
}

void AccSample::save(ofstream& out)
{
	out << "ACC " << getTimestampMs() << " " << ax << " " << ay << " " << az << endl;
}
