#include <algorithm>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std::literals;

constexpr auto sBotName = "Computer"sv;

const std::unordered_map<std::string_view, const std::vector<std::string_view>>
    words{
        {"GREETING", {"hello", "hi", "hey", "greetings", "howdy"}},
        {"FAREWELL", {"bye", "goodbye"}},
        {"VERB:BE", {"be", "is", "are", "am"}},
        {"QUESTION:WHAT", {"what", "wich"}},
        {"TIME", {"time"}},
        {"IT", {"it"}},
    };

const std::vector<std::pair<const std::vector<std::string_view>, const int>>
    phrases{
        {{"GREETING"}, 1},
        {{"TIME", "QUESTION:WHAT", "VERB:BE", "IT"}, 2},
        {{"FAREWELL"}, 3},
    };

const std::vector<void (*)()> responses{
    [] { std::cout << "Sorry, I don't understand what you said.\n"; },
    [] { std::cout << "Hello!\n"; },
    [] {
        std::time_t t = std::time(0);
        std::tm *now = localtime(&t);
        std::cout << "It's " << now->tm_hour << ':' << now->tm_min << ".\n";
    },
    [] {
        std::cout << "Bye!\n";
        exit(0);
    },
};

int main() {
    while (!std::cin.eof()) {
        std::cout << "You: ";
        std::string line;
        std::string currentWord;
        std::vector<std::string_view> inputWords;
        std::getline(std::cin, line);
        std::istringstream stream(line);
        while (stream >> currentWord) {
            std::transform(
                currentWord.begin(), currentWord.end(), currentWord.begin(),
                ::tolower
            );
            bool bFoundWordMeaning = false;
            for (auto currentDictWord : words) {
                if (std::find(
                        currentDictWord.second.begin(),
                        currentDictWord.second.end(), currentWord
                    ) != currentDictWord.second.end()) {
                    inputWords.push_back(currentDictWord.first);
                    bFoundWordMeaning = true;
                    break;
                }
            }
            if (!bFoundWordMeaning) {
                inputWords.push_back("UNKNOWN");
            }
        }

        float fPhraseMatch = 0;
        int iBestResponse = 0;

        for (auto currentPhrase : phrases) {
            float fCurrentPhraseMatch = 0;
            for (auto currentWord : inputWords) {
                if (std::find(
                        currentPhrase.first.begin(), currentPhrase.first.end(),
                        currentWord
                    ) != currentPhrase.first.end()) {
                    fCurrentPhraseMatch++;
                }
            }
            fCurrentPhraseMatch /= (float)currentPhrase.first.size();

            if (fCurrentPhraseMatch > fPhraseMatch) {
                fPhraseMatch = fCurrentPhraseMatch;
                iBestResponse = currentPhrase.second;
            }
        }

        std::cout << sBotName << ": ";
        responses[iBestResponse]();
        std::cout << '\n';
    }

    return 0;
}
