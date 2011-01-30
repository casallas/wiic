#ifndef _MLDATA_H_
#define _MLDATA_H_

#include <ml.h>
#include <vector>
#include <string>
#include <Dataset.h>

using namespace std;

class MLData
{
public:
       MLData(const vector<int>& mask);
       ~MLData();

       	bool open(const vector<string>& vf); // Open files stored vf (one for each category)
		bool load(const vector<Dataset>& vd); // Extracts features from WiiC data (one Dataset per gesture) 
      	void generateTrainingAndValidationData(float perc);  // Generate training and testing set according to the desired percentage

       	int getNumFeatures(); // nr. of features
       	int getNumCategories(); // nr. of categories
       	int getNumSamples(); // nr. of samples
       	string categoryName(int k); // name of k-th category
       	CvMat * getTrainingInputData(); // training input data (do not delete the output matrix!!!)
       	CvMat * getTrainingOutputData(); // training output data
       	CvMat * getValidationInputData(); // validation input data
       	CvMat * getValidationOutputData(); // validation output data
       
       	void getRandomSample(CvMat *sample, float &cat);
		void normalize(const int minN =0, const int maxN =1); // This will replace old data

  private:
        CvMat *all_in, *train_in, *validation_in;
        CvMat *all_out, *train_out, *validation_out;
        vector<string> categoryNames;
		vector<int> featureMask;
		int ncategories,nfeatures,nsamples;
		float min, max;
		CvRNG rng;
};

#endif

