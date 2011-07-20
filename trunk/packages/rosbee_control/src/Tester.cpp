/********************************************************************
       program: Tester
       Author: Michiel van Osch
       Date: 11-7-2001
       Description: Tester is a test program for testing the PlatformAPI
                    for Keila's Unicycle. With this program I was able
                    to communicate with the platform and read sensor
                    values, and either set PCcontrol or motion enabled.
                    However when I set control and motion after eachother
                    the program gets stuck. Probably because you are
                    not allowed to do this (and need to query in between
                    or because some serial buffer does nog get flushed
                    properly between commands.
                    
                    If I interleave with get_Rwheelpositions() (which
                    now return ultrasoundsensorvalues I beleive
                    the Tester works properly but reading sensor values
                    disables the motion or control enabled.

                    Therefore, the platform did NOT move yet using this
                    program.
*********************************************************************/

#include "rosbee_control/Platform.h"
#include <string>
#include <iostream>
#include "ros/ros.h"

using namespace std;

Platform* itsPlatform;

int main(int argc, char** argv)
{
	//init ros stuff
	ros::init(argc,argv,"tester");
	ros::NodeHandle n;

	itsPlatform = Platform::getInstance();

	itsPlatform->connect("/dev/ttyUSB0");

}


