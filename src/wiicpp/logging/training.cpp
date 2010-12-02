#include "training.h"

/***
 * Training methods
 */
Training::Training()
{

}

Training::~Training()
{
	ax.clear();
	ay.clear();
	az.clear();
}

/***
 * Load data into a single training
 */
void Training::loadTraining(ifstream& infile)
{
	string line;
	float nwVal;

	// X Measurements
	getline(infile,line);
	istringstream xlstream(line);	
	while(xlstream >> nwVal) {
		ax.push_back(nwVal);
	}

	// Y Measurements
	getline(infile,line);
	istringstream ylstream(line);
	while(ylstream >> nwVal) {
		ay.push_back(nwVal);
	}

	// Z Measurements
	getline(infile,line);
	istringstream zlstream(line);
	while(zlstream >> nwVal) {
		az.push_back(nwVal);
	}	
}

void Training::addSample(double x, double y, double z)
{
	// X measures
	ax.push_back(x);

	// Y measures
	ay.push_back(y);
	
	// Z Measures
	az.push_back(z);
}
