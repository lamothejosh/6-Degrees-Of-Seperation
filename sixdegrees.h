/**
 ** 6degrees.h
 **
 ** Project Two: Six Degrees of Collaboration
 **
 ** Purpose:
 ** Header for the 6Degrees class 
 **
 ** ChangeLog:
 **   13 Nov 2020: rgilk01, jdavid07
 **     Adapted from 'Six Degrees of Kevin Bacon'
 **
 **   17 Nov 2020: zgolds01
 **     Refactored for concision & clarity, updated documentation
 Josh Lamothe 
 August 2022
 **/   
#ifndef sixdegrees_H
#define sixdegrees_H
#include <stdexcept>
#include <iostream>
#include <ctype.h>
#include <vector>
#include <string>
#include "CollabGraph.h"
#include "Artist.h"
using namespace std;

class sixdegrees
{

public:

sixdegrees(int argc, string artist, string commands, string output);

void graph_pop();

void artist_reader(ifstream &artist);
void reader_test();

Artist artist_getter(string artist);
void command_loop(ifstream &commands, ostream &out);
void input_loop(istream &commands, ostream & out);


private:

vector<Artist> artist_collection;



CollabGraph artist_graph;

};

#endif