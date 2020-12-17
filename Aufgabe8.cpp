#include <iostream>
#include <utility>
#include <string>
#include <map>
#include "CallsignExtractor.h"
#include "./AtcoCommand/ReadAtCoCommand.h"

int main()
{
    ReadAtCoCommand testAtco("./AtcoCommand/BigWordSeqPlusCmdsFile.txt");
    vector<string> testWordSeqs = {};
    vector<string> testWordCommands = {};
    for (int i = 0; i < testAtco.getOutputAtcoCommand().getDynAtcoCommands().getSize(); i++)
    {
        if (testAtco.getOutputAtcoCommand().getDynAtcoCommands().getElementArray(i).getWordSequence().length() > 1)
        {
            testWordSeqs.push_back(testAtco.getOutputAtcoCommand().getDynAtcoCommands().getElementArray(i).getWordSequence());
            testWordCommands.push_back(testAtco.getOutputAtcoCommand().getDynAtcoCommands().getElementArray(i).getDynCommands().getElementArray(0).getCommands());
        }
    }

    CallsignExtractor c1(testWordSeqs);

    for (int t = 0; t < c1.getCallSigns().size(); t++)
    {
        cout << "Full Seq:" << testWordSeqs[t] << ".\n";
        cout << "Extracted Seq:" << c1.getCallSignWordSeqs()[t] << ".\n";
        cout << "Call sign: " << c1.getCallSigns()[t] << ".\n";
        cout << "Command:" << testWordCommands[t] << ".\n\n";
    }
}