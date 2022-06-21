#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <fstream>

#include "RDSGraph.h"
#include "utils/TimeFuncs.h"

using std::vector;
using std::pair;
using std::string;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::cout;
using std::endl;

vector<vector<string> > readSequencesFromFile(const string &filename)
{
    vector<vector<string> > sequences;
    vector<string> tokens;
    string token;

    ifstream in(filename.c_str(), ios::in);
    if(!in.is_open())
    {
        cout << "Unable to open file: " << filename << endl;
        exit(1);
    }

    while(!in.eof())
    {
        string line;
        getline(in, line);

        if(line.size() > 0)
        {
            stringstream ss(line);
            while(!ss.eof())
            {
                ss >> token;

                if(token == "*")
                    tokens.clear();
                else if(token == "#")
                {
                    sequences.push_back(tokens);
                    break;
                }
                else
                    tokens.push_back(token);
            }
        }
    }
    in.close();

    return sequences;
}

int main(int argc, char *argv[])
{
    if(argc < 6)
    {
        cout << "Usage:" << endl;
        cout << "ModifiedADIOS <filename> <eta> <alpha> <context_size> <coverage> ---OPTIONAL--- <number_of_new_sequences>" << endl;
        exit(1);
    }

    cout << "BEGIN CORPUS ----------" << endl;
    vector<vector<string> > sequences = readSequencesFromFile(argv[1]);
    for(unsigned int i = 0; i < sequences.size(); i++)
    {
        for(unsigned int j = 0; j < sequences[i].size(); j++)
            cout << sequences[i][j] << " ";
        cout << endl;
    }
    cout << "END CORPUS ----------" << endl << endl << endl;

    RDSGraph testGraph(sequences);
    cout << testGraph << endl;
    double startTime = getTime();
    testGraph.distill(ADIOSParams(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atof(argv[5])));
    double endTime = getTime();
    cout << testGraph << endl << endl;

    std::cout << endl << "Time elapsed: " << endTime - startTime << " seconds" << endl << endl << endl << endl;

    // Print grammar to terminal
    // testGraph.convert2PCFG(std::cout);
    
    // Store the input filename as a string of a variable
    std::string grammar_filename = argv[1];

    // Create a new output filename based on input filename
    // Assumption is that input filename begins with 
    // string "training_corpus".
    // Replace those 15 characters from 0th index with "grammar"
    // to create the new, custom output filename.
    grammar_filename.replace(0, 15, "grammar");
    // Print the output filename for verification
    cout << grammar_filename << endl;

    // Save grammar to a .txt file saved with the new name
    // just created
    ofstream myfile (grammar_filename); //"grammar.txt"
    testGraph.convert2nltkPCFG(myfile);
    myfile.close();

/*
    startTime = getTime();
    testGraph.distill(ADIOSParams(atof(argv[2]), atof(argv[3])*10, atoi(argv[4])-2, atof(argv[5])));
    endTime = getTime();
    cout << testGraph << endl << endl;

    std::cout << endl << "Time elapsed: " << endTime - startTime << " seconds" << endl << endl << endl << endl;*/
/*
    vector<string> testString(sequences[10].begin(), sequences[10].end());
    for(unsigned int i = 0; i < testString.size() - 1; i++)
        std::cout << testString[i] << " ";
    std::cout << testString.back() << endl;
    SearchPath newPath = testGraph.encode(testString);
    std::cout << newPath << endl;
    testGraph.predict(newPath);

    if(argc > 6)
        for(unsigned int i = 0; i < static_cast<unsigned int>(atoi(argv[6])); i++)
        {
            vector<string> sequence = testGraph.generate();
            for(unsigned int j = 0; j < sequence.size(); j++)
                std::cout << sequence[j] << " ";
            std::cout << endl;
        }*/
}
