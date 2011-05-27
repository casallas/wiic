#include "gyrosample.h"

using namespace std;

GyroSample::GyroSample(float r, float p, float y) 
	: Sample()
{ 
	roll_ = r; 
	pitch_ = p; 
	yaw_ = y;
	logType = LOG_GYRO; 
}

GyroSample::GyroSample(const string& line)
	: Sample()
{
	istringstream inLine(line);
	inLine >> roll_ >> pitch_ >> yaw_; 
	logType = LOG_GYRO; 
}

void GyroSample::save(ofstream& out)
{
	out << "GYRO " << getTimestampMs() << " " << roll_ << " " << pitch_ << " " << yaw_ << endl;
}
