/*
 *	logger.cpp
 *
 *	This file is part of WiiC, written by:
 *		Gabriele Randelli
 *		Email: randelli@dis.uniroma1.it
 *
 *	Copyright 2010
 *
 *	This file is based on WiiuseCpp, written By:
 *		James Thomas
 *		Email: jt@missioncognition.net
 *
 *	Copyright 2009
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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiicpp/wiicpp.h>

using namespace std;

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
			training->addSample(x,y,z);	
		  	enbAcq = 2;		
		  	usleep(10000);
			wii.Poll();					
		}
		
		if(enbAcq == 2) {
			dataset->addTraining(training);
			count++;
			cout << "Training acquired" << endl;
			cout << "  - Press PLUS button to acquire the training" << endl;
			cout << "  - Release PLUS button to end the training acquisition" << endl;
			cout << "  - Press HOME button to go back to the main menu" << endl;
			enbAcq = 1;		
		}

		if(enbAcq == 1 && wiimote.Buttons.isJustPressed(CButtons::BUTTON_HOME)) {
			dataset->save(logfile.c_str());
			enbAcq = 0;
		}
	}
}


int main(int argc, char** argv)
{
    CWii wii; // Defaults to 4 remotes
    std::vector<CWiimote>::iterator i;
    int reloadWiimotes = 0;
    int numFound;
    int index;
    bool help = false;

	string option;
	if(argc == 2) {
		option = string(argv[1]);
		if(option == "help") {
			cout << endl << "wiic-logger is a utility to collect accelerometer data in a log." << endl;
			cout << "wiic-logger is part of WiiC (http://wiic.sf.net)" << endl << endl;
			cout << "Usage: wiic-logger <log_filename>" << endl;
			cout << "   <log_filename>: full pathname of the output log file" << endl << endl;
		
			return 1;
		}
	}
	else {
		cout << endl << "Bad Syntax: wiic-logger <log_filename>" << endl;
		cout << "   <log_filename>: full pathname of the output log file" << endl;
		cout << "Type wiic-logger help for a brief documentation." << endl << endl;
		
		return 1;
	}

    cout << "Searching for wiimotes... Turn them on!" << endl;

    //Find the wiimote
    numFound = wii.Find(1);

    // Search for up to five seconds;

    cout << "Found " << numFound << " wiimotes" << endl;
    cout << "Connecting to wiimotes..." << endl;

    // Connect to the wiimote
    std::vector<CWiimote>& wiimotes = wii.Connect();

	cout << "Connected to " << (unsigned int)wiimotes.size() << " wiimotes" << endl;

	if(wiimotes.size() == 0) {
		cout << "Error: no connected Wiimote" << endl;
		return 1;
	}
	
    // Use a reference to make working with the iterator handy.
    CWiimote & wiimote = wiimotes[0];

    //Rumble for 0.2 seconds as a connection ack
    wiimote.SetRumbleMode(CWiimote::ON);
    usleep(200000);
    wiimote.SetRumbleMode(CWiimote::OFF);
	wiimote.SetMotionSensingMode(CWiimote::ON);

	dataAcquisition(wii, wiimote, option);

    return 0;
}
