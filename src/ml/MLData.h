#ifndef _MLDATA_H_
#define _MLDATA_H_

#include <ml.h>
#include <vector>
#include <string>

using namespace std;

class MLData
{
public:
       MLData(const vector<int>& mask);
       ~MLData();

       	bool open(const vector<string>& vf); // apre i file memorizzati in vf (uno per ciascuna categoria e li carica in memoria)
       	void generateTrainingAndValidationData(float perc);  // genera random perc % training data (in train) e il resto come validation

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

