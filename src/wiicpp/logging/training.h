#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "accsample.h"

using namespace std;

/**
 * Class Training to save the individual training of a gesture  
 */
class Training
{
public:
	/**
	 * Constructor void
	 */
	Training() {}
	~Training();

	void loadTraining(ifstream&);
	void save(ofstream&) const;
	void addSample(Sample*);
	void clear();

	inline const Sample* sampleAt(int i) const { 
		if(i < samples.size())
			return samples[i]; 
		else {
			cout << "[Error]: requested out of array bound index in training." << endl;
			return 0;
		}
	}

	inline Sample* sampleAt(int i) { 
		if(i < samples.size())
			return samples[i]; 
		else {
			cout << "[Error]: requested out of array bound index in training." << endl;
			return 0;
		}
	}

	inline int size() const { return samples.size(); }

private:
	vector<Sample*> samples;
};
