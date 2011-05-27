#include "dataset.h"
#include "wiic_internal.h"

/**
 * Dataset methods
 */
Dataset::~Dataset()
{
	clear();
}

/**
 *  Add a new training to the dataset
 */
void Dataset::addTraining(Training* t)
{
	if(t)
		trainings.push_back(t);
		
	loaded = true;
}

/**
 * Load dataset from a file into a class Dataset
 */
bool Dataset::loadDataset(const string& nomefile)
{
	int size, version; 
	string line, dummy;
	ifstream infile(nomefile.c_str());

	if(!infile.is_open()) {
		cout<<"[Error] Unable to open the dataset file"<<endl;
		return false;
	}
	else {
		trainings.clear();
		
		// Version
		getline(infile,line); // First line is WiiC log version
		if(line.find("WiiC") == string::npos) {
			cout << "[Error] Bad log format." << endl;
			return false;
		}
		istringstream iline(line);
		iline >> dummy >> version;
		if(version > WIIC_LOG_VERSION) {
			cout << "[Error] Unsupported WiiC log version." << endl;
			return false;
		}
		getline(infile,line); // Date (we don't need this...)
		getline(infile,line); // Wiimote address (we don't need this...)
		
		// Number of trainings	
		getline(infile,line); 	
		iline.str(line);
		iline >> size;
		
		// For each training
		for(int i = 0 ; i < size ; i++) {
			getline(infile,line);
			if(line == "START") {
				// Each training is inserted in the training vector	
				trainings.push_back(new Training());
				if(!(trainings[i]->loadTraining(infile))) {
					cout << "[Error] Unable to load a training in the dataset" << endl ;
					return false;
				}
			}
		}
	}
	infile.close();
	loaded = true;
	
	return loaded;
}

void Dataset::clear()
{
	for(int i = 0 ; i < trainings.size() ; i++) {
		if(trainings[i]) {
			delete trainings[i];
			trainings[i] = 0;
		}
	}
	trainings.clear();
	loaded = false;
}

/**
 * Save data vector for recognition or acquisition into a file
 *
 * @param file name
 */
bool Dataset::save(const char* file, const char* addr) const
{
	// Open file
	ofstream out(file, ios::trunc);
	if(!out.is_open()) { // File does not exist, hence I create it
		cout << "[Error] Unable to open " << file << endl;
		return false;
	}
	
	// Save header
	saveHeader(out, addr);
	
	// For each training
	for(int i = 0 ; i < size() ; i++)	{
		const Training* training = trainingAt(i);
		if(training)
			training->save(out);
	}
	
	out.close();

	return true;
}

void Dataset::saveHeader(ofstream& out, const char* addr) const
{
	// Log Version
	out << "WiiC " << WIIC_LOG_VERSION << endl;

	// Date
    time_t tim=time(NULL);
    char *s=ctime(&tim);
    s[strlen(s)-1]=0;        // remove \n
	out << s << endl;
	
	// Mac Address
	out << addr << endl;
	out << size() << endl;		
}


