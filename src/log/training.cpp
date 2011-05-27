#include "training.h"

Training::~Training()
{
	clear();
}

/***
 * Load data into a single training
 */
bool Training::loadTraining(ifstream& training)
{
	string sample, cmd;
	
	// Get a sample	
	getline(training, sample);
	
	// Parse every sample to get the log type
	istringstream sStr(sample);
	sStr >> cmd;
	
	while(!training.eof() && cmd != "END" && cmd != "") {	
		//cout << "comando " << cmd << endl;
		if(cmd == "ACC") 
			addSample(new AccSample(sStr.str()));
		else if(cmd == "GYRO") 
			addSample(new GyroSample(sStr.str()));
		else {
			cout << "[Error] Bad log type." << endl;	
			return false;	
		}
		
		getline(training, sample);
		sStr.str(sample);
		sStr >> cmd;
	}		
	
	return true;
}

void Training::save(ofstream& out) const
{	
	// Training Header
	out << "START" << endl;
	
	// Samples
	for(int i = 0 ; i < samples.size() ; i++)
		samples[i]->save(out);
		
	out << "END" << endl;
}

void Training::addSample(Sample* s)
{
	// We retrieve the overall gesture timestamp
	struct timeval t;
	if(!samples.size()) {
		gettimeofday(&timestamp,0);
		t = timestamp;
	}
	else
		gettimeofday(&t,0);
		
	// We compute the relative timestamp in msec
	double t_msec = ((t.tv_sec*1000000.0 + t.tv_usec) - (timestamp.tv_sec*1000000.0 + timestamp.tv_usec))/1000.0;
	
	if(s) {
		s->setTimestampMs(t_msec);
		samples.push_back(s);
	}
}

void Training::clear()
{
	for(int i = 0 ; i < samples.size() ; i++) {
		if(samples[i]) {
			delete samples[i];
			samples[i] = 0;
		}
	}
	samples.clear();
}
