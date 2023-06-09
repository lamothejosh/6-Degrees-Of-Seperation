/**
 ** 6degrees.cpp
 **
 ** Project Two: Six Degrees of Collaboration
 **
 ** Purpose:
 **   Implementation of the 6degrees class
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


#include "sixdegrees.h"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * default constructor
 * purpose: initilize the sixdegree class and open up all necessary files
 *
 * parameters: argument count, and strings containing each file
 * returns:    none
 */
sixdegrees::sixdegrees(int argc, string artist, string commands, string output)
{
    ifstream file;
    file.open(artist);
    if (not file.is_open()){
        cerr << artist << "could not be opened" << endl;
        exit(1);
    }

    artist_reader(file);   
    graph_pop();


    ifstream command_file;
    if (argc >= 3)
    {
        command_file.open(commands);
        if (not command_file.is_open()){
            cerr << commands << "could not be opened" << endl;
            exit(1);
        }
        if (argc == 4)
        {
            filebuf fb;
            fb.open(output, ios::out);
            ostream out_file(&fb);
            command_loop(command_file, out_file);
        }
        command_loop(command_file,cout);
    }

    input_loop(cin, cout);

}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Graph pop
 * purpose: populates the graph using the artist file
 *
 * parameters: none
 * returns:    none
 */
void sixdegrees::graph_pop()
{
    for (unsigned long int i = 0; i < artist_collection.size(); i++)
    {   
        artist_graph.insert_vertex(artist_collection[i]);
    }
    
    for (unsigned long int i = 0; i < artist_collection.size(); i++)
    {
        for (unsigned long int j = 0; j < artist_collection.size(); j++)
        {
            if (i == j)
            {
                continue;
            }
            
            string poss_collab;
            poss_collab = artist_collection[i].get_collaboration(artist_collection[j]);
            if (poss_collab != "")
            {
                artist_graph.insert_edge(artist_collection[i],artist_collection[j], poss_collab);
            }
            
        }
    }  
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * artist reader
 * purpose: places all artist from the file into a vector of artist structs
 *
 * parameters: and artist file
 * returns:    none
 */
void sixdegrees::artist_reader(ifstream &artist)
{
    string line;
    while (artist.peek() != EOF)
    {
        getline(artist, line);
        if (line != "*")
        {
            Artist new_artist(line);

            while (line != "*")
            {
                getline(artist,line);
                if (line != "*")
                {
                    new_artist.add_song(line);
                }
            }
            artist_collection.push_back(new_artist);
        }
    }
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * command loop
 * purpose: loops through the query if there is a command file
 *
 * parameters: command file and output file
 * returns:    none
 */
void sixdegrees::command_loop(ifstream &commands, ostream &out_file)
{
    while (commands.peek() != EOF)
    {
        string curr_command, start_artist, end_artist, temp;
        getline(commands, curr_command);

        if (curr_command == "quit")
        {
            exit(0);
        }
        
        if (curr_command == "dfs" or curr_command == "bfs" or curr_command == "not")
        {
            getline(commands, start_artist);
            Artist start = artist_getter(start_artist);

            getline(commands, end_artist);
            Artist end = artist_getter(end_artist);

            if (start.get_name() == "" and end.get_name() == "")
            {
                out_file <<"\"" << start_artist << "\"" << " was not found in the dataset :(\n";
                out_file <<"\"" << end_artist << "\"" << " was not found in the dataset :(\n";
                goto stop;
            }

            if (start.get_name() == "")
            {
                out_file <<"\"" << start_artist << "\"" << " was not found in the dataset :(\n";
                goto stop;
            }
            if (end.get_name() == "")
            {
                out_file <<"\"" << end_artist << "\"" << " was not found in the dataset :(\n";
                goto stop;
            }

            if(curr_command == "dfs"){
                artist_graph.clear_metadata();
                artist_graph.depthfirst(start, end, out_file);
            }
            if(curr_command == "not"){
                vector<string> names;
                vector<Artist> blocked;
                while (temp != "*")
                {
                    getline(commands, temp);
                    if (temp != "*")
                    {
                        Artist block = artist_getter(temp);
                        if (block.get_name() == "")
                        {
                            cout <<"\"" << temp << "\"" << " was not found in the dataset :(\n";
                            goto stop;
                        }
                        blocked.push_back(block);
                        names.push_back(temp);
                    }
                }
                for (unsigned long int i = 0; i < blocked.size(); i++)
                {
                        if (blocked[i].get_name() == "")
                        {
                            cout <<"\"" << names[i] << "\"" << " was not found in the dataset :(\n";
                            goto stop;
                        }
                }
                artist_graph.clear_metadata();
                artist_graph.no(start, end, blocked, out_file);
            }
            else if(curr_command == "bfs"){
                artist_graph.clear_metadata();
                artist_graph.breadthfirst(start,end, out_file);
            }
        }
        if (curr_command != "quit" and curr_command != "bfs" and curr_command != "dfs" and curr_command != "not" )
        {
            out_file << curr_command << " is not a command. Please try again.\n";
        }

    }
    stop:
        command_loop(commands, out_file);

}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * input loop
 * purpose: loops through the query if there is no command file
 *
 * parameters: cin  and output file
 * returns:    none
 */
void sixdegrees::input_loop(istream &commands, ostream &out_file)
{
    string curr_command, start_artist, end_artist, temp;
    while (curr_command != "quit")
    {
        getline(commands, curr_command);

        if (curr_command == "quit")
        {
            exit(0);
        }
        
        if (curr_command == "dfs" or curr_command == "bfs" or curr_command == "not")
        {
            getline(commands, start_artist);
            Artist start = artist_getter(start_artist);

            getline(commands, end_artist);
            Artist end = artist_getter(end_artist);

            if (start.get_name() == "" and end.get_name() == "")
            {
                out_file <<"\"" << start_artist << "\"" << " was not found in the dataset :(\n";
                out_file <<"\"" << end_artist << "\"" << " was not found in the dataset :(\n";
                goto stop;
            }

            if (start.get_name() == "")
            {
                out_file <<"\"" << start_artist << "\"" << " was not found in the dataset :(\n";
                goto stop;
            }

            if (end.get_name() == "")
            {
                out_file <<"\"" << end_artist << "\"" << " was not found in the dataset :(\n";
                goto stop;
            }

            if(curr_command == "dfs"){
                artist_graph.clear_metadata();
                artist_graph.depthfirst(start, end, out_file);
            }
            if(curr_command == "not"){
                vector<Artist> blocked;
                vector<string> names;
                while (temp != "*")
                {
                    getline(commands, temp);
                    if (temp != "*")
                    {
                        Artist block = artist_getter(temp);
                        blocked.push_back(block);
                        names.push_back(temp);
                    }
                }
                for (unsigned long int i = 0; i < blocked.size(); i++)
                {
                        if (blocked[i].get_name() == "")
                        {
                            cout <<"\"" << names[i] << "\"" << " was not found in the dataset :(\n";
                            goto stop;
                        }
                }
                
                artist_graph.clear_metadata();
                artist_graph.no(start, end, blocked, out_file);
            }
            else if(curr_command == "bfs"){
                artist_graph.clear_metadata();
                artist_graph.breadthfirst(start,end, out_file);
            }
        }
        if (curr_command != "quit" and curr_command != "bfs" and curr_command != "dfs" and curr_command != "not" )
        {
            out_file << curr_command << " is not a command. Please try again.\n";
        }
    }   
     stop:
            input_loop(commands, out_file);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * artist getter
 * purpose: gets an artist (struct) using their name
 *
 * parameters: string (name of artist)
 * returns:    the artist that matches the name (if no one matches returns
 *                          empty artist)
 */
Artist sixdegrees::artist_getter(string artist)
{
    for (unsigned long int i = 0; i < artist_collection.size(); i++)
    {
        if (artist_collection[i].get_name() == artist)
        {
            return artist_collection[i];
        }
    }
    Artist empty;
    return empty;
}