#include "training.h"

/**
 * Class Dataset for manage data from wiimote for a neural net application
 */
class Dataset
{
	public:
	Dataset() {}
	Dataset(const string& filename) { loadDataset(filename); }
	~Dataset();

	/** number of training for current dataset */
	inline int size() const { return training.size(); }

	inline const Training* trainingAt(int i) const { 
		if(i < training.size())
			return training[i]; 
		else
			return 0;
	}

	inline Training* trainingAt(int i) { 
		if(i < training.size())
			return training[i]; 
		else
			return 0;
	}

	/*** Adds a new training to the dataset */
	void addTraining(Training*);

	/**
	 * load value for dataset form a file
	 */
	void loadDataset(const string& nomefile);
	
	/**
	 * save training onto dataset
	 */
	void getDataTraining(ifstream& infile, Training* training);

	/**
	 * Delete all trainings and clear the buffer
	 */
	void clear() ;
	
	/**
	Save data vector for recognition or acquisition into a file
	*
	* @param file name
	*/
	bool save(const char* file);

private:
	vector<Training*> training;
};
