#pragma once
#include "string"
#include "NatoDecode.h"
#include <map>
#include <vector>
#include <unordered_map>
#include "./NumberExtractor/NumberExtractor.h"
using namespace std;
class CallsignExtractor
{
private:
    /* data */
    string callSign;
    string callSignWordSeq;
    string notCallSignWordSeq;
    vector<string> callSigns;
    vector<string> callSignWordSeqs;
    vector<string> notCallSignWordSeqs;
    vector<string> numberFromNotCallSign;
    vector<vector<string>> numberFromNotCallSigns;
    map<int, string> callSignElementMap;
    void push_back_String(string &str1, string str2);
    void push_front_String(string &str1, string str2);
    void extractNumberFromNotCallSign(string str, vector<string> &numberFromNotCallSign);
    void findAndAddToMap(string stringToFind, map<int, string> &mapToAdd);
    void findAndAddToMap(string stringToFind, unordered_map<string, vector<string>> mapToFind, map<int, string> &mapToAdd);
    void findAndAddToMap(string stringToFind, unordered_map<string, string> mapToFind, map<int, string> &mapToAdd);
    void combineMapToCallSign(map<int, string> combineMap, string &combined);
    void extractCallSignWordSeq(string wordSeq, string &callSignWordSeq, string &notCallSignWordSeq);
    NatoDecode nato;
    bool findWordInNato(string word);
    bool findNumberInNato(string word);
    bool findWordInNatoWithoutNumber(string word);

public:
    CallsignExtractor(string wordSeq);
    CallsignExtractor(vector<string> wordSeqs);
    string getCallSign();
    vector<string> getCallSigns();
    string getCallSignWordSeq();
    string getNotCallSignWordSeq();
    vector<string> getCallSignWordSeqs();
    vector<string> getNotCallSignWordSeqs();
    vector<string> getNumberFromNotCallSign();
    vector<vector<string>> getNumberFromNotCallSigns();
    ~CallsignExtractor();
};
