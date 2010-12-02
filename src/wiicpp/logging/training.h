#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

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
	Training();
	~Training();

	void loadTraining(ifstream& infile);
	void addSample(double, double, double);
	
	inline float xSample(int i) const {
		if(i < ax.size())
			return ax[i];
		else
			return -1;
	}
	
	inline float ySample(int i) const {
		if(i < ay.size())
			return ay[i];
		else
			return -1;
	}
	
	inline float zSample(int i) const {
		if(i < az.size())
			return az[i];
		else
			return -1;
	}
	
	inline int size() const { return ax.size(); }

	private:
	/** vector<float> ax,ay,az is a vector with the values of the accelerations */
	vector<float> ax;
	vector<float> ay;
	vector<float> az;
};
