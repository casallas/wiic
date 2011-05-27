/*
 *	logger.cpp
 *
 *	This file is part of WiiC, written by:
 *		Gabriele Randelli
 *		Email: randelli@dis.uniroma1.it
 *
 *	Copyright 2010
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <fstream>
#include <unistd.h>
#include <wiicpp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

void acquireGesture(CWii& wii, CWiimote& wiimote, Training* training)
{
	cout << "  - Press PLUS button to acquire the training" << endl;
	cout << "  - Release PLUS button to end the training acquisition" << endl;
	int enbAcq = 1;

	while(enbAcq) {
		wii.Poll();
		
		//Inizio acquisizione dati
		while(enbAcq && wiimote.Buttons.isPressed(CButtons::BUTTON_PLUS)) {
			float x, y, z;
			wiimote.Accelerometer.GetGravityVector(x,y,z);
			training->addSample(new AccSample(x,y,z));	
		  	enbAcq = 2;		
		  	usleep(10000);
			wii.Poll();					
		}
		
		if(enbAcq == 2) {
			enbAcq = 0;		
		}
	}
}

void randomAcquisition(CWii& wii, CWiimote& wiimote, const int samples, vector<string>& files)
{
	// Total iterations
	int totSamples = samples*files.size();
	srand(time(0));
	cout << "=======================================" << endl;
	cout << "You are going to acquire " << samples << endl;
	cout << "samples per " << files.size() << " files "
		 << " = " << totSamples << " total samples" << endl << endl;
	
	// Open datasets
	vector<Dataset*> outs;
	for(int i = 0 ; i < files.size() ; i++) 
		outs.push_back(new Dataset());		
		
	// Number of samples per file
	vector<int> fileSamples;
	for(int i = 0 ; i < files.size() ; i++)
		fileSamples.push_back(samples);
	
	for(int i = 0 ; i < totSamples ; i++) {
		// Random generation
		int selection = rand() % files.size() ;

		// Gesture acquisition
		cout << "Selected gesture: " << files[selection] << endl;
		Training* training = new Training();
		acquireGesture(wii, wiimote, training);
		cout << "Training " << (i+1) << " acquired" << endl;
		
		// Gesture sadd
		outs[selection]->addTraining(training);
		
		// Sample decrement and check whether to close the file
		if(!(--fileSamples[selection])) {
			outs[selection]->save(files[selection].c_str(),wiimote.GetAddress()); // Stream close
			if(outs[selection])
				delete outs[selection]; // Stream deallocation
			outs.erase(outs.begin() + selection); // Stream erase
			files.erase(files.begin() + selection); // Filename erase
			fileSamples.erase(fileSamples.begin() + selection); // File sample erase
		}
	}	
}

void timeAcquisition(CWii& wii, CWiimote& wiimote, const string logfile, const int start, const int length)
{
	int count = 0;
	int tick = length*100;
	Dataset* dataset = new Dataset();
	Training* training = new Training();

	cout << "Training will be stored in " << logfile << endl;
	cout << "Logging will start in " << start << " seconds..." << endl;
	sleep(start);

	cout << "Logging for " << length << " seconds..." << endl;	
	wii.Poll();
		
	//Inizio acquisizione dati
	while(count < tick) {
		float x, y, z;
		wiimote.Accelerometer.GetRawGravityVector(x,y,z);
		training->addSample(new AccSample(x,y,z));		
	  	usleep(10000);
		++count;
		wii.Poll();					
	}	
	
	dataset->addTraining(training);
	dataset->save(logfile.c_str(),wiimote.GetAddress());

	if(dataset)
		delete dataset;
	dataset = 0;
}

void dataAcquisition(CWii& wii, CWiimote& wiimote, const string logfile)
{
	int enbAcq = 0;
	int count = 0;
	
	string fileacc;
	
	Dataset* dataset = new Dataset();
	Training* training = new Training();

	cout << "Trainings will be stored in " << logfile << endl;
	cout << "  - Press PLUS button to acquire the training" << endl;
	cout << "  - Release PLUS button to end the training acquisition" << endl;
	cout << "  - Press HOME button to go back to the main menu" << endl;
	enbAcq = 1;

	while(enbAcq) {
		wii.Poll();
		
		//Inizio acquisizione dati
		while(enbAcq && wiimote.Buttons.isPressed(CButtons::BUTTON_PLUS)) {
			float x, y, z;
			wiimote.Accelerometer.GetGravityVector(x,y,z);
			training->addSample(new AccSample(x,y,z));	
		  	enbAcq = 2;		
		  	usleep(10000);
			wii.Poll();					
		}
		
		if(enbAcq == 2) {
			dataset->addTraining(training);
			count++;
			cout << "Training " << count << " acquired" << endl;
			cout << "  - Press PLUS button to acquire the training" << endl;
			cout << "  - Release PLUS button to end the training acquisition" << endl;
			cout << "  - Press HOME button to go back to the main menu" << endl;
			enbAcq = 1;		
			training = new Training();
		}

		if(enbAcq == 1 && wiimote.Buttons.isJustPressed(CButtons::BUTTON_HOME)) {
			dataset->save(logfile.c_str(),wiimote.GetAddress());
			enbAcq = 0;
		}
	}
	
	if(training)
		delete training;
	training = 0;

	if(dataset)
		delete dataset;
	dataset = 0;		
}


int main(int argc, char** argv)
{
    CWii wii; // Defaults to 4 remotes
    bool help = false;

	string option; 
	if(argc == 2) {
		option = string(argv[1]);
		if(option == "help") {
			cout << endl << "wiic-logger is a utility to collect accelerometer data in a log." << endl;
			cout << "wiic-logger is part of WiiC (http://wiic.sf.net)" << endl << endl;
			cout << "Usage: wiic-logger <filename>" << endl;
			cout << "       wiic-logger RANDOM <num_samples> <filename1> ... <filenameN>" << endl;
			cout << "       wiic-logger TIME <start_time> <length> <filename>" << endl;
			cout << "   <filename>: full pathname of the output log file" << endl;
			cout << "   <num_samples>: number of samples for each file" << endl << endl;
			cout << "   <filename1> ... <filenameN>: full pathname of each gesture file" << endl << endl;
			cout << "	<start_time>: sleep time (in seconds) before automatic logging" << endl;
			cout << "	<length>: log length (in seconds)" << endl;
		
			return 1;
		}
	}
	else if(argc >= 3) {
		option = string(argv[1]);
		if(option != "RANDOM" && option != "TIME") {
			cout << endl << "Bad Syntax!" << endl;
			cout << "Type wiic-logger help for a brief documentation." << endl << endl;
		
			return 1;
		}
	}
	else {
		cout << endl << "Bad Syntax!" << endl;
		cout << "Type wiic-logger help for a brief documentation." << endl << endl;
		
		return 1;	
	}

	// Find and connect to the wiimotes
    std::vector<CWiimote>& wiimotes = wii.FindAndConnect();
	
	if(!wiimotes.size()) {
		cout << "Error: no connected Wiimote" << endl;
		return 1;		
	}
	CWiimote& wiimote = wiimotes[0];
	wiimote.SetMotionSensingMode(CWiimote::ON);

	if(option == "RANDOM") {
		// Num of samples for each file
		int numSamples = atoi(argv[2]);
		
		// Load candidate files
		vector<string> gestures;
		int i = 3;
		while (i < argc) 
			gestures.push_back(string(argv[i++]));
		
		randomAcquisition(wii, wiimote, numSamples, gestures);
	}
	else if(option == "TIME") {
		// File name
		int start = atoi(string(argv[2]).c_str());
		int length = atoi(string(argv[3]).c_str()); 
		string filename = string(argv[4]);
		timeAcquisition(wii, wiimote, filename, start, length);
	}
	else
		dataAcquisition(wii, wiimote, option);

	cout << "=======================================" << endl;
	cout << "Bye!" << endl;

    return 0;
}
