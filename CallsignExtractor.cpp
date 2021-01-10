#include "unordered_map"
#include <vector>
#include "CallsignExtractor.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>
#include <string>
#include <map>
#include <iterator>
#include <algorithm>
using namespace std;

CallsignExtractor::CallsignExtractor(string wordSeq)
{
    extractCallSignWordSeq(wordSeq, callSignWordSeq, notCallSignWordSeq);
    findAndAddToMap(callSignWordSeq, callSignElementMap);
    combineMapToCallSign(callSignElementMap, callSign);
    extractNumberFromNotCallSign(notCallSignWordSeq, numberFromNotCallSign);
};

CallsignExtractor::CallsignExtractor(vector<string> wordSeqs)
{
    for (auto &&i : wordSeqs)
    {
        CallsignExtractor temp(i);
        callSigns.push_back(temp.getCallSign());
        callSignWordSeqs.push_back(temp.getCallSignWordSeq());
        notCallSignWordSeqs.push_back(temp.getNotCallSignWordSeq());
        numberFromNotCallSigns.push_back(temp.getNumberFromNotCallSign());
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

void CallsignExtractor::extractNumberFromNotCallSign(string str, vector<string> &toAdd)
{
    NumberExtractor numbExtract(str);
    for (int i = 0; i < numbExtract.getListOfNumberExtracted().size(); i++)
    {
        toAdd.push_back(numbExtract.getListOfNumberExtracted()[i]);
    }
}

void CallsignExtractor::findAndAddToMap(string stringToFind, map<int, string> &mapToAdd)
{
    findAndAddToMap(stringToFind, nato.lettersToNato, mapToAdd);
    findAndAddToMap(stringToFind, nato.numbersToNatto, mapToAdd);
    findAndAddToMap(stringToFind, nato.numbersToNatoMultipleDigits, mapToAdd);
    findAndAddToMap(stringToFind, nato.designators, mapToAdd);
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

    // when meet @triple
    if (combined.find("@@@") != -1)
    {
        combined = combined.substr(0, combined.find("@@@")) + combined.at(combined.find("@@@") + 3) + combined.at(combined.find("@@@") + 3) + combined.substr(combined.find("@@@") + 3, combined.length());
    }
    // when meet @double
    else if (combined.find("@@") != -1)
    {
        combined = combined.substr(0, combined.find("@@")) + combined.at(combined.find("@@") + 2) + combined.substr(combined.find("@@") + 2, combined.length());
    }

    if (combined.length() == 0)
        combined = "NO_CALLSIGN";
};

template <typename OutputIterator>
void getWord(const std::string &s, OutputIterator out)
{
    istringstream wordStream(s);
    string word;
    while (wordStream >> word)
    {
        *out = word;
        ++out;
    }
}

auto getWordVector(std::string const &s)
{
    std::vector<std::string> results;
    getWord(s, back_inserter(results));
    return results;
}

void CallsignExtractor::extractCallSignWordSeq(string wordSeq, string &callSign_WordSeq, string &notCallSign_WordSeq)
{
    std::vector<std::string> wordVector = getWordVector(wordSeq);

    bool notFoundCallSign = true;
    for (int i = 0; i < wordVector.size(); i++)
    {
        if (findWordInNatoWithoutNumber(wordVector[i]))
        {
            notFoundCallSign = false;
        }
        if (i < wordVector.size() - 1)
        {
            if (findWordInNatoWithoutNumber((wordVector[i]) + " " + wordVector[(i) +static_cast < unsigned __int64>(1)]))
            {
                notFoundCallSign = false;
            }
        }
    }
    if (notFoundCallSign)
    {
        notCallSign_WordSeq = wordSeq;
        return;
    }

    std::vector<std::string> notCallsignVector;
    bool endRecordeCallSignWordSeq = false;
    bool startRecordCallSignWordSeq = false;
    for (int i = 0; i < wordVector.size(); i++)
    {
        if (findWordInNato(wordVector[i]))
        {
            startRecordCallSignWordSeq = true;
        }

        if (i < wordVector.size() - 1)
        {
            if (findWordInNato(wordVector[i] + " " + wordVector[i + static_cast <unsigned __int64>(1)]))
            {
                startRecordCallSignWordSeq = true;
                callSign_WordSeq = callSign_WordSeq + " " + wordVector[i] + " " + wordVector[i + static_cast <unsigned __int64>(1)];
                i++;
                continue;
            }
        }

        if (i < wordVector.size() - 4)
        {
            if (findWordInNato(wordVector[i] + " " + wordVector[i + static_cast <unsigned __int64>(1)] + " " + wordVector[i + static_cast <unsigned __int64>(2)] + " " + wordVector[i + static_cast <unsigned __int64>(3)]))
            {
                startRecordCallSignWordSeq = true;
                callSign_WordSeq = callSign_WordSeq + " " + wordVector[i] + " " + wordVector[i + static_cast <unsigned __int64>(1)] + " " + wordVector[i + static_cast <unsigned __int64>(2)] + " " + wordVector[i + static_cast <unsigned __int64>(3)];
                i += 3;
                continue;
            }
        }

        if (wordVector[i] == "correction")
        {
            callSign_WordSeq = "";
            notCallsignVector.push_back(wordVector[i]);
            startRecordCallSignWordSeq = false;
            continue;
        }

        if (startRecordCallSignWordSeq && !findWordInNato(wordVector[i]))
        {
            endRecordeCallSignWordSeq = true;
        }

        if (startRecordCallSignWordSeq && !endRecordeCallSignWordSeq)
        {
            callSign_WordSeq = callSign_WordSeq + " " + wordVector[i];
        }
        else if (!findWordInNatoWithoutNumber(wordVector[i]))
        {
            notCallsignVector.push_back(wordVector[i]);
        }
    }

    // extract not call sign from notCallsignVector
    for (int i = 0; i < notCallsignVector.size(); i++)
    {
        if (i > 1)
        {
            if (notCallsignVector[i] == "correction" && findNumberInNato(notCallsignVector[i - static_cast <unsigned __int64>(1)]))
            {
                for (int j = i - 1; j > 0; j--)
                {
                    if (findNumberInNato(notCallsignVector[j]))
                    {
                        notCallsignVector.erase(notCallsignVector.begin() + j);
                    }
                    else
                        break;
                }
            }
        }
    }

    for (int i = 0; i < notCallsignVector.size(); i++)
    {
        notCallSign_WordSeq = notCallSign_WordSeq + " " + notCallsignVector[i];
    }
};

bool CallsignExtractor::findWordInNato(string word)
{
    for (auto &&i : nato.lettersToNato)
    {
        if (word == i.second)
        {
            return true;
        }
    }
    for (auto &&i : nato.numbersToNatto)
    {
        if (word == i.second)
        {
            return true;
        }
    }
    for (auto &&i : nato.numbersToNatoMultipleDigits)
    {
        if (word == i.second)
        {
            return true;
        }
    }
    for (auto &&i : nato.designators)
    {
        for (int j = 0; j < i.second.size(); j++)
        {
            if (word == i.second[j])
            {
                return true;
            }
        }
    }
    return false;
}

bool CallsignExtractor::findNumberInNato(string word)
{

    for (auto &&i : nato.numbersToNatto)
    {
        if (word == i.second)
        {
            return true;
        }
    }
    for (auto &&i : nato.numbersToNatoMultipleDigits)
    {
        if (word == i.second)
        {
            return true;
        }
    }
    return false;
}

bool CallsignExtractor::findWordInNatoWithoutNumber(string word)
{
    for (auto &&i : nato.lettersToNato)
    {
        if (word == i.second)
        {
            return true;
        }
    }

    for (auto &&i : nato.designators)
    {
        for (int j = 0; j < i.second.size(); j++)
        {
            if (word == i.second[j])
            {
                return true;
            }
        }
    }
    return false;
}

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

string CallsignExtractor::getNotCallSignWordSeq()
{
    return notCallSignWordSeq;
};

vector<string> CallsignExtractor::getCallSignWordSeqs()
{
    return callSignWordSeqs;
};

vector<string> CallsignExtractor::getNotCallSignWordSeqs()
{
    return notCallSignWordSeqs;
};

vector<string> CallsignExtractor::getNumberFromNotCallSign()
{
    return numberFromNotCallSign;
};

vector<vector<string>> CallsignExtractor::getNumberFromNotCallSigns()
{
    return numberFromNotCallSigns;
};

CallsignExtractor::~CallsignExtractor()
{
}