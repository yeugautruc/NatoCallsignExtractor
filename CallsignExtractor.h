#pragma once
#include "string"
#include "NatoDecode.h"
#include <map>
#include <vector>
#include <unordered_map>
using namespace std;
class CallsignExtractor
{
private:
    /* data */
    string callSign;
    string callSignWordSeq;
    vector<string> callSigns;
    vector<string> callSignWordSeqs;
    void push_back_String(string &str1, string str2);
    void push_front_String(string &str1, string str2);
    void findAndAddToMap(string stringToFind, unordered_map<string, vector<string>> mapToFind, map<int, string> &mapToAdd);
    void findAndAddToMap(string stringToFind, unordered_map<string, string> mapToFind, map<int, string> &mapToAdd);
    void combineMapToCallSign(map<int, string> combineMap, string &combined);
    void extractCallSignWordSeq(string wordSeq, string &callSignWordSeq);
    NatoDecode nato;

public:
    CallsignExtractor(string wordSeq);
    CallsignExtractor(vector<string> wordSeqs);
    string getCallSign();
    vector<string> getCallSigns();
    string getCallSignWordSeq();
    vector<string> getCallSignWordSeqs();
    ~CallsignExtractor();
};
