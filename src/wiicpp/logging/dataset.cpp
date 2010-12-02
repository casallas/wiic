#include "dataset.h"

/***
 * Dataset methods
 */
Dataset::~Dataset()
{
	clear();
}

/***
 *  Add a new training to the dataset
 */
void Dataset::addTraining(Training* t)
{
	training.push_back(t);
}

/**
* Load dataset from a file into a class Dataset
*/
void Dataset::loadDataset(const string& nomefile)
{
	int size, dummy;
	ifstream infile;
	
	infile.open(nomefile.c_str());
	if(!infile.is_open()) {
		cout<<"[Error] Unable to open the dataset file"<<endl;
	}
	else {
		training.clear();
		infile >> size >> dummy; // Number of trainings (dummy is used for the CR)

		for(int i = 0 ; i < size ; i++) {
			// Each training is inserted in the training vector	
			training.push_back(new Training());	
			// Each training is filled with acc values
			training[i]->loadTraining(infile);
		}
	}
	infile.close();
}

void Dataset::clear()
{
	for(int i = 0 ; i < training.size() ; i++) {
		if(training[i]) {
			delete training[i];
			training[i] = 0;
		}
	}
	training.clear();
}

/**
Save data vector for recognition or acquisition into a file
*
* @param file name
*/
bool Dataset::save(const char* file)
{
	bool result = false;
	bool mode = false;

	// Open file
	ofstream out(file, ios::trunc);
	if(!out.is_open()) { // File does not exist, hence I create it
		cout << "[Error] Unable to open " << file << endl;
		return false;
	}
	
	out << size() << endl;	
	
	// For each training
	for(int i = 0 ; i < size() ; i++)	{
		const Training* t = trainingAt(i);
		if(t) {
			// For each X sample
			for(int j = 0 ; j < t->size() ; j++) {
				out << t->xSample(j) << " ";
			}
			out << endl ;
			
			// For each Y sample
			for(int j = 0 ; j < t->size() ; j++) {
				out << t->ySample(j) << " ";
			}
			out << endl ;
			
			// For each Z sample
			for(int j = 0 ; j < t->size() ; j++) {
				out << t->zSample(j) << " ";
			}
			out << endl ;
		}
	}
	
	out.close();

	return true;
}


