#include "unordered_map"
#include "vector"
#include "CallsignExtractor.h"
#include <fstream>
#include <iostream>
#include <utility>
#include <string>
#include <map>
#include <iterator>
#include <algorithm>
using namespace std;

CallsignExtractor::CallsignExtractor(string wordSeq)
{
    extractCallSignWordSeq(wordSeq, callSignWordSeq);

    callSign = "";

    map<int, string> callSignElementMap;
    findAndAddToMap(callSignWordSeq, nato.lettersToNato, callSignElementMap);
    findAndAddToMap(callSignWordSeq, nato.numbersToNatto, callSignElementMap);
    findAndAddToMap(callSignWordSeq, nato.numbersToNatoMultipleDigits, callSignElementMap);
    findAndAddToMap(callSignWordSeq, nato.designators, callSignElementMap);

    combineMapToCallSign(callSignElementMap, callSign);
    if (callSign.length() == 0)
    {
        callSign = "NO_CALLSIGN";
    }
};

CallsignExtractor::CallsignExtractor(vector<string> wordSeqs)
{
    for (auto &&i : wordSeqs)
    {
        CallsignExtractor temp(i);
        callSigns.push_back(temp.getCallSign());
        callSignWordSeqs.push_back(temp.getCallSignWordSeq());
    }
};

void CallsignExtractor::push_back_String(string &str1, string str2)
{
    for (int i = 0; str2[i] != '\0'; i++)
    {
        str1.push_back(str2[i]);
    }
}

void CallsignExtractor::push_front_String(string &str1, string str2)
{
    for (int i = 0; str1[i] != '\0'; i++)
    {
        str2.push_back(str1[i]);
    }
    str1 = str2;
}

void CallsignExtractor::findAndAddToMap(string stringToFind, unordered_map<string, vector<string>> mapToFind, map<int, string> &mapToAdd)
{
    for (auto &&i : mapToFind)
    {
        for (int j = 0; j < i.second.size(); j++)
        {
            vector<size_t> positions;
            size_t pos = stringToFind.find(i.second[j], 0);
            while (pos != string::npos)
            {
                positions.push_back(pos);
                pos = stringToFind.find(i.second[j], pos + 1);
            }

            if (positions.size() != 0)
            {
                for (int k = 0; k < positions.size(); k++)
                {
                    mapToAdd.emplace(static_cast<int>(positions[k]), i.first);
                }
                break;
            };
        }
    }
}

void CallsignExtractor::findAndAddToMap(string stringToFind, unordered_map<string, string> mapToFind, map<int, string> &mapToAdd)
{
    for (auto &&i : mapToFind)
    {
        vector<size_t> positions;
        size_t pos = stringToFind.find(i.second, 0);
        while (pos != string::npos)
        {
            positions.push_back(pos);
            pos = stringToFind.find(i.second, pos + 1);
        }

        if (positions.size() != 0)
        {
            for (int k = 0; k < positions.size(); k++)
            {
                mapToAdd.emplace(static_cast<int>(positions[k]), i.first);
            }
        };
    }
}

void CallsignExtractor::combineMapToCallSign(map<int, string> combineMap, string &combined)
{
    int temp = 0;
    for (auto &&i : combineMap)
    {
        if (i.first >= temp)
        {
            push_back_String(combined, i.second);
        }
        else
        {
            push_front_String(combined, i.second);
        }
    }

    if (combined.find("@") != -1)
    {
        combined = combined.substr(0, combined.find("@@@")) + combined.at(combined.find("@@@") + 3) + combined.at(combined.find("@@@") + 3) + combined.substr(combined.find("@@@") + 3, combined.length());
    }
};

void CallsignExtractor::extractCallSignWordSeq(string wordSeq, string &callSignWordSeq)
{
    for (auto &&i : nato.devider)
    {
        vector<size_t> positions;
        size_t pos = wordSeq.find(i.second, 0);
        while (pos != string::npos)
        {
            positions.push_back(pos);
            pos = wordSeq.find(i.second, pos + 1);
        }

        if (positions.size() != 0)
        {
            auto it = min_element(std::begin(positions), std::end(positions));
            callSignWordSeq = wordSeq.substr(0, static_cast<int>(*it));
            break;
        };
        callSignWordSeq = wordSeq;
    }
};

string CallsignExtractor::getCallSign()
{
    return callSign;
};

vector<string> CallsignExtractor::getCallSigns()
{
    return callSigns;
};

string CallsignExtractor::getCallSignWordSeq()
{
    return callSignWordSeq;
};

vector<string> CallsignExtractor::getCallSignWordSeqs()
{
    return callSignWordSeqs;
};

CallsignExtractor::~CallsignExtractor()
{
}