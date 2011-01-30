#include <MLAlg.h>

int main(int argc, char **argv) {

  
    if (argc<2) {
		cout << "Use  " << argv[0] << " <KNN|Bayes|SVN|DT> [training data ratio] " << endl;
		cout << "e.g. " << argv[0] << " KNN  [0.75] " << endl;
		return 1;
    }
    
    vector<int> mask;
    for(int i = 1 ; i <= 8 ; i++) 
	    mask.push_back(i);

    MLData mldata(mask);
    MLAlg mlalg;
    mlalg.setType(string(argv[1]));
    
    vector<string> vf;
    vf.push_back("data/UP");
    vf.push_back("data/DOWN");
    vf.push_back("data/LEFT");
    vf.push_back("data/RIGHT");
    vf.push_back("data/FORWARD");
    vf.push_back("data/BACKWARD");
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
