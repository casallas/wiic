#include <MLAlg.h>

int main(int argc, char **argv) {

  
    if (argc<2) {
		cout << "wiic-ml allows to recognize a set of gestures using OpenCV machine learning library." << endl;
		cout << "wiic-ml is part of WiiC (http://wiic.sf.net)" << endl << endl;
		cout << "Usage:  " << argv[0] << " <KNN|Bayes|SVN|DT|ANN|Boost|RT> <training_data_ratio> <data_file1> ... <data_fileN>" << endl;
		cout << "e.g. " << argv[0] << " KNN 0.75 file1.log file2.log" << endl;
		cout << "======= Supported Algorithms =======" << endl;
		cout << "KNN: k-nearest neighbor" << endl;
		cout << "Bayes: normal Bayes classifier" << endl;
		cout << "SVN: support vector machines" << endl;
		cout << "DT: decision trees" << endl;
		cout << "ANN: artificial neural networks" << endl;
		cout << "Boost: two-class discrete AdaBoost" << endl;
		cout << "RT: random trees" << endl;
		return 1;
    }
    
    vector<int> mask;
    for(int i = 1 ; i <= 8 ; i++) 
	    mask.push_back(i);

    MLData mldata(mask);
    MLAlg mlalg;
    mlalg.setType(string(argv[1]));
    
    vector<string> vf;
	for(int i = 3 ; i < argc ; i++)
    	vf.push_back(string(argv[i]));

    if(!mldata.open(vf)) {
		cout << "Unable to parse input files" << endl;
		return 1;
    }

    mldata.normalize();
    float tr = 0.75; if (argc>2) tr = atof(argv[2]);
    mldata.generateTrainingAndValidationData(tr);
    CvMat *trainIn = mldata.getTrainingInputData();
    CvMat *trainOut = mldata.getTrainingOutputData();
    CvMat *validateIn = mldata.getValidationInputData();
    CvMat *validateOut = mldata.getValidationOutputData();
    
    cout << "===================================" << endl;
    mlalg.train( trainIn, trainOut );
    
    cout << "===================================" << endl;
    mlalg.validate(validateIn,validateOut);
    
    return 0;
}
