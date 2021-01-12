#pragma once
#include "string"
#include <map>
#include <vector>
#include <unordered_map>
using namespace std;

class NatoDecode
{
private:
public:
    const unordered_map<string, string> lettersToNato = {
        {"B", "bravo"},
        {"A", "alfa"},
        {"C", "charlie"},
        {"D", "delta"}, // there is also an airline 'delta', with the three letter code DAL
        {"E", "echo"},
        {"F", "foxtrot"}, // also fox is said
        {"G", "golf"},
        {"H", "hotel"},
        {"I", "india"},
        {"J", "juliett"},
        {"K", "kilo"},
        {"L", "lima"},
        {"M", "mike"},
        {"N", "november"},
        {"O", "oscar"},
        {"P", "papa"},
        {"Q", "quebec"},
        {"R", "romeo"},
        {"S", "sierra"},
        {"T", "tango"},
        {"U", "uniform"},
        {"V", "victor"},
        {"W", "whiskey"},
        {"X", "x-ray"},
        {"Y", "yanhkee"},
        {"Z", "zulu"}};

    const unordered_map<string, string> numbersToNatto = {
        {"1", "one"},
        {"2", "two"},
        {"3", "three"},
        {"4", "four"},
        {"5", "five"},
        {"6", "six"},
        {"7", "seven"},
        {"8", "eight"},
        {"9", "nine"},
        {"0", "zero"},
        {".", "decimal"}};

    const unordered_map<string, string> numbersToNatoMultipleDigits = {
        {"10", "ten"},
        {"11", "eleven"},
        {"12", "twelve"},
        {"13", "thirteen"},
        {"14", "fourteen"},
        {"15", "fifteen"},
        {"16", "sixteen"},
        {"17", "seventeen"},
        {"18", "eighteen"},
        {"19", "nineteen"},

        {"20", "twenty"},
        {"30", "thirty"},
        {"40", "fourty"},
        {"50", "fifty"},
        {"60", "sixty"},
        {"70", "seventy"},
        {"80", "eighty"},
        {"90", "ninety"},
        {"100", "hundred"},
        {"1000", "thousand"}};

    const unordered_map<string, vector<string>> designators = {
        {"A", {"alpha"}},
        {"C", {"charly"}},
        {"W", {"whisky"}},
        {"Z", {"zoulou"}},
        {"X", {"xray"}},
        {"F", {"fox"}},
        {"ABP", {"b_air"}},
        {"ACA", {"canada"}},
        {"AFR", {"air_france"}},
        {"AEG", {"airest", "eastern", "east air", "east"}},
        {"AYY", {"lupus"}},
        {"BER", {"air_berlin", "berlin"}},
        {"BAW", {"speed bird", "speed_bird", "speedbird"}},
        {"CCA", {"air_china", "china"}},
        {"CSA", {"csa", "czech", "CSA"}},
        {"CTN", {"croatia"}},
        {"EZY", {"easy jet", "easy_jet"}},
        {"GTW", {"united states of america"}},
        {"LDM", {"lauda_motion", "lauda", "motion"}},
        {"LOT", {"lot"}},
        {"SWR", {"swiss air", "swiss_air"}},
        {"TVL", {"travel air", "travel service"}},
        {"TVS", {"sky_travel", "sky travel"}},
        {"WZZ", {"wizzair", "wizz_air", "wizz air"}},
        {"DLH", {"lufthansa", "hansa"}},
        {"GEC", {"lufthansa cargo"}},
        {"LCI", {"lufthansa india"}},
        {"LHT", {"lufthansa technik"}},
        {"DAL", {"delta"}},
        {"DAT", {"deltair", "delta air"}},
        {"CPD", {"capital delta"}},
        {"KLH", {"klm heli"}},
        {"KLM", {"klm", "KLM"}},
        {"ROT", {"tarom"}},
        {"ENT", {"enter", "enter air"}},
        {"RYR", {"ryan air", "ryan_air", "ryanair"}},
        {"SAS", {"scandinavian"}},
        {"AUA", {"austrian", "austrian airlines"}},
        {"MHV", {"snowcap", "snow cap"}},
        {"NLY", {"flyniki", "fly niki", "fly_niki", "niki"}},
        {"@@@", {"triple"}},
        {"@@", {"double"}}};
};
