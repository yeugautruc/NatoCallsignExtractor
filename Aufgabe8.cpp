#include <iostream>
#include <utility>
#include <string>
#include <map>
#include "CallsignExtractor.h"
#include "ExpectedValues.h"
#include "./AtcoCommand/ReadAtCoCommand.h"
#include <windows.h>

using namespace std;

const unsigned short BLUE = 9, YELLOW = 14, PINK = 13, RED = 12, WHITE = 15, GREEN = 10, GRAY = 8;

void cout_with_color(unsigned short color, string text)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    cout << text;
    SetConsoleTextAttribute(hConsole, WHITE);
}

bool existsTest(const string &name)
{
    ifstream myFile(name);
    if (myFile.fail())
    {
        cout << "Test file exist                                OK" << endl;
        return false;
    }
    return true;
};

bool ReadUtteranceCheckCallSignTest01()
{
    struct testUtt
    {
        vector<string> expecteds;
        vector<string> wordSeqs;
        bool testResult;
        testUtt(map<string, string> m)
        {
            for (const auto &item : m)
            {
                wordSeqs.push_back(item.first);   // get word sequences from map expected
                expecteds.push_back(item.second); // get expected callsigns  from map expected
            }
            testResult = testCallSignCorrectRead(); // get test Result
        };
        bool testCallSignCorrectRead()
        {
            for (int i = 0; i < expecteds.size(); i++)
            {
                CallsignExtractor temp(wordSeqs[i]);    // extract callsigns from word sequence
                if (temp.getCallSign() != expecteds[i]) // compare extracted callsign and expected callsigns
                {
                    return false;
                }
            }
            return true;
        };
    };
    map<string, string> expectedMap = {
        {"oscar kilo tango november tango praha radar contact karlovy vary radar one one eight decimal eight five zero naslysenou", "OKTNT"},
        {"lufthansa triple nine alfa good morning radar contact kilo is correct", "DLH999A"},
        {"lufthansa double nine alfa good morning radar contact kilo is correct", "DLH99A"},
        {"oscar echo india november kilo direct whisky whisky nine eight five", "OEINK"},
        {"good morning lufthansa one two bravo descend eight zero", "DLH12B"},
        {"gruess gott ryan_air seven seven delta kilo in radar contact", "RYR77DK"},
        {"standby", "NO_CALLSIGN"},
        {"gruess gott lupus one one zero expect ils approach three four", "AYY110"},
        {"gruess gott austrian seven seven seven sierra identified climb flight level two three zero", "AUA777S"}};
    testUtt t1(expectedMap); //run test with expected value is true and get result
    if (!t1.testResult)
    {
        return false;
    }
    return true;
}

bool ReadUtteranceCheckCallSignTest02()
{
    struct testUtt
    {
        vector<string> expecteds;
        vector<string> wordSeqs;
        bool testResult;
        testUtt(map<string, string> m)
        {
            for (const auto &item : m)
            {
                wordSeqs.push_back(item.first);   // get word sequences from map expected
                expecteds.push_back(item.second); // get expected callsigns  from map expected
            }
            testResult = testCallSignCorrectRead(); // get test Result
        };
        bool testCallSignCorrectRead()
        {
            for (int i = 0; i < expecteds.size(); i++)
            {
                CallsignExtractor temp(wordSeqs[i]);    // extract callsigns from word sequence
                if (temp.getCallSign() != expecteds[i]) // compare extracted callsign and expected callsigns
                {
                    return false;
                }
            }
            return true;
        };
    };
    map<string, string> expectedMap = {
        {"oscar kilo tango november tango praha radar contact karlovy vary radar one one eight decimal eight five zero naslysenou", "AAAAS"},
        {"lufthansa triple nine alfa good morning radar contact kilo is correct", "AAAAS"},
        {"lufthansa double nine alfa good morning radar contact kilo is correct", "AAAAS"},
        {"oscar echo india november kilo direct whisky whisky nine eight five", "AAAAS"},
        {"good morning lufthansa one two bravo descend eight zero", "AAAAS"},
        {"gruess gott ryan_air seven seven delta kilo in radar contact", "AAAAS"},
        {"standby", "AAAAS"},
        {"gruess gott lupus one one zero expect ils approach three four", "AAAAS"},
        {"gruess gott austrian seven seven seven sierra identified climb flight level two three zero", "AAAAS"}};
    testUtt t1(expectedMap); //run test with expected value is false and get result
    if (t1.testResult)
    {
        return false;
    }
    return true;
}

bool ReadUtteranceCheckCallSign(string fileName, bool printOut)
{

    ReadAtCoCommand data(fileName);

    if (data.runAllTests() != 1)
    {
        cout << "Read atcoCommand ocurred error!";
        return false;
    }
    if (!ReadUtteranceCheckCallSignTest01())
    {
        cout << "ReadUtterance test 1 failed!";
        return false;
    }
    if (!ReadUtteranceCheckCallSignTest02())
    {
        cout << "ReadUtterance test 2 failed!";
        return false;
    }

    // get wordSeqs to extract call sign
    vector<string> wordSeqs = {};
    for (int i = 0; i < data.getOutputAtcoCommand().getDynAtcoCommands().getSize(); i++)
    {
        if (data.getOutputAtcoCommand().getDynAtcoCommands().getElementArray(i).getWordSequence().length() > 1)
        {
            wordSeqs.push_back(data.getOutputAtcoCommand().getDynAtcoCommands().getElementArray(i).getWordSequence());
        }
    }

    // input wordSeqs to extract
    CallsignExtractor callSignEx(wordSeqs);

    if (printOut)
    {
        for (int i = 0; i < callSignEx.getCallSigns().size(); i++)
        {
            cout << "Keyword Seq:";
            cout_with_color(GREEN, wordSeqs[i]);
            cout << "\nCallsign Seq:";
            cout_with_color(PINK, callSignEx.getCallSignWordSeqs()[i]);
            cout << "\nCallsign extracted:   ";
            cout_with_color(YELLOW, callSignEx.getCallSigns()[i] + "\n");
            cout << endl;
        }
    }

    return true;
}

bool ReadUtteranceExtractNumbersTest01()
{
    struct testUtt
    {
        vector<string> wordSeqs;
        vector<vector<ExpectedValues>> numberSeqs;
        bool testResult;
        testUtt(map<string, vector<ExpectedValues>> m)
        {
            for (const auto &item : m)
            {
                wordSeqs.push_back(item.first);    // get word sequences from map expected
                numberSeqs.push_back(item.second); // get expected numbers  from map expected
            }
            testResult = testNumberCorrectRead(m); // get test Result
        };
        bool testNumberCorrectRead(map<string, vector<ExpectedValues>> m)
        {
            for (int i = 0; i < numberSeqs.size(); i++)
            {
                CallsignExtractor temp(wordSeqs[i]); // extract numbers from word sequence
                if (temp.getNumberFromNotCallSign().size() != m.at(wordSeqs[i]).size())
                    return false; // if amount of number extracted from each word sequence is not the same as expected
                for (int j = 0; j < temp.getNumberFromNotCallSign().size(); j++)
                {
                    if (temp.getNumberFromNotCallSign()[j].find(".") != -1) // if number extracted is double
                    {
                        if (to_string(m.at(wordSeqs[i])[j].getExpectedDouble()).find(temp.getNumberFromNotCallSign()[j]) == -1)
                            return false; // compare if expected as the same with extracted
                    }                     // if number extracted is integer then compare if expected as the same with extracted
                    else if (to_string(m.at(wordSeqs[i])[j].getExpectedInt()) != temp.getNumberFromNotCallSign()[j])
                        return false;
                }
            }
            return true;
        };
    };

    map<string, vector<ExpectedValues>> expectedMap;
    expectedMap.emplace("dobry den sky_travel five eight juliett ruzyne radar radar contact on present"
                        " heading descend four thousand feet qnh one zero two two",
                        vector<ExpectedValues>{ExpectedValues(4000), ExpectedValues(1022)});
    expectedMap.emplace("oscar kilo victor india kilo roger descend three thousand five hundred feet"
                        " squawk seven thousand",
                        vector<ExpectedValues>{ExpectedValues(3500), ExpectedValues(7000)});
    expectedMap.emplace("snow cap two hundred one descend eight thousand feet",
                        vector<ExpectedValues>{ExpectedValues(8000)});
    expectedMap.emplace("austrian three nine two papa descend altitude one zero thousand"
                        " qnh one zero zero three",
                        vector<ExpectedValues>{ExpectedValues(10000), ExpectedValues(1003)});
    expectedMap.emplace("fly_niki six hundred zulu contact tower now"
                        " one two three point eight servus",
                        vector<ExpectedValues>{ExpectedValues(123.8)});
    expectedMap.emplace("negative sir temperature two three instead of two one "
                        "and dew point one three instead of one two",
                        vector<ExpectedValues>{ExpectedValues(23), ExpectedValues(21), ExpectedValues(13), ExpectedValues(12)});
    expectedMap.emplace("contact director one one nine eight two five goodbye",
                        vector<ExpectedValues>{ExpectedValues(119825)});
    testUtt t1(expectedMap); //run test with expected value is true and get result
    if (!t1.testResult)
    {
        return false;
    }
    return true;
}

bool ReadUtteranceExtractNumbersTest02()
{
    struct testUtt
    {
        vector<string> wordSeqs;
        vector<vector<ExpectedValues>> numberSeqs;
        bool testResult;
        testUtt(map<string, vector<ExpectedValues>> m)
        {
            for (const auto &item : m)
            {
                wordSeqs.push_back(item.first);    // get word sequences from map expected
                numberSeqs.push_back(item.second); // get expected numbers  from map expected
            }
            testResult = testNumberCorrectRead(m); // get test Result
        };
        bool testNumberCorrectRead(map<string, vector<ExpectedValues>> m)
        {
            for (int i = 0; i < numberSeqs.size(); i++)
            {
                CallsignExtractor temp(wordSeqs[i]); // extract numbers from word sequence
                if (temp.getNumberFromNotCallSign().size() != m.at(wordSeqs[i]).size())
                    return false; // if amount of number extracted from each word sequence is not the same as expected
                for (int j = 0; j < temp.getNumberFromNotCallSign().size(); j++)
                {
                    if (temp.getNumberFromNotCallSign()[j].find(".") != -1) // if number extracted is double
                    {
                        if (to_string(m.at(wordSeqs[i])[j].getExpectedDouble()).find(temp.getNumberFromNotCallSign()[j]) == -1)
                            return false; // compare if expected as the same with extracted
                    }                     // if number extracted is integer then compare if expected as the same with extracted
                    else if (to_string(m.at(wordSeqs[i])[j].getExpectedInt()) != temp.getNumberFromNotCallSign()[j])
                        return false;
                }
            }
            return true;
        };
    };

    map<string, vector<ExpectedValues>> expectedMap;
    expectedMap.emplace("dobry den sky_travel five eight juliett ruzyne radar radar contact on present"
                        " heading descend four thousand feet qnh one zero two two",
                        vector<ExpectedValues>{ExpectedValues(1000), ExpectedValues(1000)});
    expectedMap.emplace("oscar kilo victor india kilo roger descend three thousand five hundred feet"
                        " squawk seven thousand",
                        vector<ExpectedValues>{ExpectedValues(1000), ExpectedValues(1000)});
    expectedMap.emplace("snow cap two hundred one descend eight thousand feet",
                        vector<ExpectedValues>{ExpectedValues(1000)});
    expectedMap.emplace("austrian three nine two papa descend altitude one zero thousand"
                        " qnh one zero zero three",
                        vector<ExpectedValues>{ExpectedValues(1000), ExpectedValues(1000)});
    expectedMap.emplace("fly_niki six hundred zulu contact tower now"
                        " one two three point eight servus",
                        vector<ExpectedValues>{ExpectedValues(100.10)});
    expectedMap.emplace("negative sir temperature two three instead of two one "
                        "and dew point one three instead of one two",
                        vector<ExpectedValues>{ExpectedValues(1000), ExpectedValues(1000)});
    expectedMap.emplace("contact director one one nine eight two five goodbye",
                        vector<ExpectedValues>{ExpectedValues(1000)});
    testUtt t1(expectedMap); //run test with expected value is false and get result
    if (t1.testResult)
    {
        return false;
    }
    return true;
}

bool ReadUtteranceExtractNumbers(string fileName, vector<vector<ExpectedValues>> container, bool printOut)
{
    ReadAtCoCommand data(fileName);
    if (data.runAllTests() != 1)
    {
        cout << "Read atcoCommand ocurred error!";
        return false;
    }

    if (!ReadUtteranceExtractNumbersTest01())
    {
        cout << "ReadUtteranceExtractNumbersTest01 ocurred error!";
        return false;
    }

    if (!ReadUtteranceExtractNumbersTest01())
    {
        cout << "ReadUtteranceExtractNumbersTest02 ocurred error!";
        return false;
    }

    // get wordSeqs to extract call sign
    vector<string> wordSeqs = {};
    for (int i = 0; i < data.getOutputAtcoCommand().getDynAtcoCommands().getSize(); i++)
    {
        if (data.getOutputAtcoCommand().getDynAtcoCommands().getElementArray(i).getWordSequence().length() > 1)
        {
            wordSeqs.push_back(data.getOutputAtcoCommand().getDynAtcoCommands().getElementArray(i).getWordSequence());
        }
    }

    // input wordSeqs to extract
    CallsignExtractor callSignEx(wordSeqs);

    if (printOut)
    {
        for (int i = 0; i < callSignEx.getCallSigns().size(); i++)
        {
            cout << "Keyword Seq:";
            cout_with_color(GREEN, wordSeqs[i]);
            cout << "\nNumber extracted:   ";
            if (callSignEx.getNumberFromNotCallSigns()[i].size() == 0)
            {
                cout_with_color(YELLOW, "No number extracted   ");
            }
            for (int j = 0; j < callSignEx.getNumberFromNotCallSigns()[i].size(); j++)
            {
                cout_with_color(YELLOW, callSignEx.getNumberFromNotCallSigns()[i][j]);
                if (j != callSignEx.getNumberFromNotCallSigns()[i].size() - 1)
                    cout << ",  ";
            }

            cout << "     Expected: ";
            if (container[i].size() == 0)
            {
                cout_with_color(PINK, "No number expected");
            }
            for (int k = 0; k < container[i].size(); k++)
            {
                cout_with_color(PINK, container[i][k].toString());
                if (k != container[i].size() - 1)
                    cout << ",  ";
            }

            cout << endl;
            cout << endl;
        }
    }
    return true;
};

int main()
{
    string fileName = "NumbersWithCallSignsEx1.txt";
    cout << "       ReadUtteranceCheckCallSign file NumbersWithCallSignsEx1.txt: \n";
    ReadUtteranceCheckCallSign(fileName, true);
    cout << "\n\n\n";

    cout << "       ReadUtteranceExtractNumbers file NumbersWithCallSignsEx1.txt: \n";
    ReadUtteranceExtractNumbers(fileName,
                                vector<vector<ExpectedValues>>{vector<ExpectedValues>{ExpectedValues(985)},
                                                               vector<ExpectedValues>{ExpectedValues(80)},
                                                               vector<ExpectedValues>{},
                                                               vector<ExpectedValues>{},
                                                               vector<ExpectedValues>{ExpectedValues(34)},
                                                               vector<ExpectedValues>{ExpectedValues(230)}},
                                true);
    cout << "\n\n\n";

    cout << "       ReadUtteranceCheckCallSign file CallSignCorrection.txt: \n";
    ReadUtteranceCheckCallSign("CallSignCorrection.txt", true);
    cout << "\n\n\n";

    cout << "       ReadUtteranceExtractNumbers file NumberCorrection.txt: \n";
    ReadUtteranceExtractNumbers("NumberCorrection.txt",
                                vector<vector<ExpectedValues>>{vector<ExpectedValues>{ExpectedValues(4000), ExpectedValues(1012)},
                                                               vector<ExpectedValues>{ExpectedValues(4000), ExpectedValues(10), ExpectedValues(7000)}},
                                true);
}