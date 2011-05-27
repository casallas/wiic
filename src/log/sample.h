#ifndef SAMPLE_H
#define SAMPLE_H

#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <sstream>

namespace WiiC {
	enum LogType {
		LOG_NONE,
		LOG_ACC,
		LOG_GYRO
	};
}

using namespace std;
using namespace WiiC;

class Sample
{
public:
	Sample() { gettimeofday(&timestamp,0); }
	~Sample() {}
	virtual void save(ofstream& out) =0;
	
	inline void setLogType(LogType l) { logType = l; }
	inline LogType getLogType() const { return logType; }
	inline void setTimestampMs(double t) { relTimestamp = t; }
	inline double getTimestampMs() { return relTimestamp; }
	
protected:	

	struct timeval timestamp; // Absolute
	double relTimestamp; // msec (from the beginning of the gesture)
	LogType logType;
};

#endif

