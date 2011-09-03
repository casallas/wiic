#ifndef DATASET_H
#define DATASET_H

#include "training.h"

class Dataset
{
public:
	/** 
	 * Default constructor.
	 */
	Dataset() : loaded(false) { }
	
	/** 
	 * Creates a Dataset object by loading a dataset stored in a file.
	 * 
	 * @param filename Pathname of the dataset to load
	 */
	Dataset(const string& filename) : loaded(false) { loadDataset(filename); }
	~Dataset();

	/** 
	 * Returns the number of trainings in the current dataset. 
	 */
	inline int size() const { return trainings.size(); }

	/** 
	 * Returns the i-th training of the dataset as a constant pointer. 
	 * 
	 * @param i Training index in the dataset
	 */
	inline const Training* trainingAt(unsigned int i) const { 
		if(i < trainings.size())
			return trainings[i]; 
		else {
			cout << "[Error]: requested out of array bound index in dataset." << endl;
			return 0;
		}
	}

	/** 
	 * Returns the i-th training of the dataset as a pointer. 
	 * 
	 * @param i Training index in the dataset
	 */
	inline Training* trainingAt(unsigned int i) { 
		if(i < trainings.size())
			return trainings[i]; 
		else {
			cout << "[Error]: requested out of array bound index in dataset." << endl;
			return 0;
		}
	}

	/** 
	 * Adds a training to the dataset. 
	 * 
	 * @param training Training to add
	 */
	void addTraining(Training* training);

	/** 
	 * Loads a new dataset from file, erasing the current one (if not saved on file). 
	 * 
	 * @param filename Filename of the dataset to load
	 */
	bool loadDataset(const string& filename);
	
	/** 
	 * Saves the current dataset in a file. 
	 * 
	 * @param filename Filename for the dataset to save
	 * @param addr MAC address of the source device for the dataset
	 */
	bool save(const char* filename, const char* addr) const;
	
	/** 
	 * Erases the current elements of the dataset.
	 */
	void clear() ;

	/**
	 * Checks if the instance has a valid dataset loaded.
	 */
	inline bool isValid() const { return loaded; }

protected:
	void saveHeader(ofstream& out, const char* addr) const;

private:
	vector<Training*> trainings;
	bool loaded;
};

#endif
