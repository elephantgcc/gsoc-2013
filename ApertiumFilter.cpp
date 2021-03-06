/**
 * Coding challenge for Apertium GSOC2013 
 * of idea "Sliding-window part-of-speech tagger":
 *
 * Write a filter that reads in the output of Apertium 
 * morphological analyser and writes out either a random 
 * one (-r) or the first one (-f) of the lexical form for
 * each surface form in a new format, respecting superblanks.
 * http://wiki.apertium.org/wiki/Ideas_for_Google_Summer_of_Code/Sliding-window_part-of-speech_tagger
 *
 * ======================================================================================================
 * Example input line:
 * [<em>]^I/I<num><mf><sg>/I<prn><subj><p1><mf><sg>$ ^have/have<vbhaver><inf>/have<vbhaver><pres>$[</em>]
 * output:
 * I.prn.subj.p1.mf.sg have.vbhaver.inf 
 * ======================================================================================================
 * 2013-04-21 by Gang Chen
 */

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdio>
using namespace std;

//mode = true, output the first one
//mode = false, output a random one
void filt(bool mode) {
    srand((unsigned)time(NULL));
    string line;
    size_t begin = 0, end = 0; // index for paired '^' and '$'
    string attr_info;          // e.g. I.num.mf.sg
    vector<string> attr_info_list; // e.g. I.num.mf.sg I.prn.subj.p1.mf.sg
    while (getline(cin, line)) {
        begin = 0, end = 0;
        while (true) {
        
            // locate begin('^') and end('$')
            begin = line.find('^', end); // skiped superblanks
            if (begin == string::npos) {
                break; // break when no further '^' is found
            }
            end = line.find('$', begin); // skiped superblanks
            if (end == string::npos) {
                cerr << "error: input format error!\n";
                exit(1);
            }
            
            //collecting attr_info(s) into list
            attr_info.clear();
            attr_info_list.clear();
            for ( size_t i = line.find('/', begin) + 1; i <= end; ++i) {
                if (line[i] == '/' || line[i] == '$') {
                    attr_info_list.push_back(attr_info);
                    attr_info.clear();
                } else if (line[i] == '<') {
                    attr_info.push_back('.');
                } else if (line[i] == '>') {
                    continue;
                } else {// normal morph content
                    attr_info.push_back(line[i]);
                }
            }
            
            //output, true for first, false for random
            if (mode == true) {
                cout << attr_info_list[0] << " ";
            } else {
                cout << attr_info_list[rand() % attr_info_list.size()] << " ";
            }
        }
        cout << endl;
    }
}

int main(int argc, char * argv[]) {
    if (argc == 2 && strcmp(argv[1], "-f") == 0) {
        filt(true);
    } else if (argc == 2 && strcmp(argv[1], "-r") == 0) {
        filt(false);
    } else if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        cout << "usage: exe -f, output the first morphological analysis for each word.\n"
             << "       exe -r, output a random morphological analysis for each word.\n";
    } else {
        cerr << "error: invalid cmd/option!\n"
             << "try `exe -h or --help' for more information.\n";
        exit(1);
    }
}
