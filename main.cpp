/*
 * main.cpp
 *
 * COMP 15 Project 2: Six Degrees of Collaboration
 * 
 * by  August 17 2022
        Josh Lamothe
 */



#include <iostream>
#include <string>
#include "Artist.h"
#include "CollabGraph.h"
#include "sixdegrees.h"

using namespace std;


int main(int argc, char *argv[])
{
    if(argc > 4 or argc < 1){
        cerr <<" Usage: ./ SixDegrees dataFile [commandFile] [outputFile]";
        cerr << endl;
        return 1;
    }

    string artist = argv[1];
    string commands = "cin";
    string output = "output";

    if (argc >= 3)
    {
        commands = argv[2];
    }

    else{
        output = argv[3];
    }
    
    sixdegrees files(argc, artist, commands, output);
    
    return 0;
}
