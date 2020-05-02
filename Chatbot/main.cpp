#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <algorithm>

#include <ctime>

std::string sBotName = "Computer";

const std::unordered_map<const char*, std::vector<std::string>> words{
	{"GREETING",      {"hello", "hi", "hey", "greetings", "howdy" }},
	{"FAREWELL",      {"bye", "goodbye"                           }},
	{"VERB:BE",       {"be", "is", "are", "am"                    }},
	{"QUESTION:WHAT", {"what", "wich"                             }},
	{"TIME",          {"time"                                     }},
	{"IT",            {"it"                                       }},
};

const std::vector<std::pair<std::vector<std::string>, int>> phrases{
	{{"GREETING"                               }, 1},
	{{"TIME", "QUESTION:WHAT", "VERB:BE", "IT" }, 2},
	{{"FAREWELL"                               }, 3},
};

const std::vector<void(*)()> responses{
	[] { std::cout << "Sorry, I don't understand what you said.\n"; },
	[] { std::cout << "Hello!\n"; },
	[] {
		std::time_t t = std::time(0);
		std::tm* now = localtime(&t);
		std::cout << "It's " << now->tm_hour << ':' << now->tm_min << ".\n";
	},
	[] {
		std::cout << "Bye!\n";
		exit(0);
	},
};

int main() {
	while(true) {
		std::cout << "You: ";
		std::string line;
		std::string currentWord;
		std::vector<std::string> inputWords;
		std::getline(std::cin, line);
		std::istringstream stream(line);
		while(stream >> currentWord) {
			std::transform(currentWord.begin(), currentWord.end(), currentWord.begin(), ::tolower);
			bool bFoundWordMeaning = false;
			for(auto currentDictWord : words) {
				if(std::find(currentDictWord.second.begin(), currentDictWord.second.end(), currentWord) != currentDictWord.second.end()) {
					inputWords.push_back(currentDictWord.first);
					bFoundWordMeaning = true;
					break;
				}
			}
			if(!bFoundWordMeaning)
				inputWords.push_back("UNKNOWN");
		}

		/* std::cout << "Parsed: "; */
		/* for(auto a : inputWords) */
		/* 	std::cout << a << ' '; */
		/* std::cout << '\n'; */

		float fPhraseMatch = 0;
		int iBestResponse = 0;

		for(auto currentPhrase : phrases) {
			float fCurrentPhraseMatch = 0;
			for(auto currentWord : inputWords) {
				if(std::find(currentPhrase.first.begin(), currentPhrase.first.end(), currentWord) != currentPhrase.first.end())
					fCurrentPhraseMatch++;
			}
			fCurrentPhraseMatch /= (float)currentPhrase.first.size();

			if(fCurrentPhraseMatch > fPhraseMatch) {
				fPhraseMatch = fCurrentPhraseMatch;
				iBestResponse = currentPhrase.second;
			}
		}

		//std::cout << "Selected response: " << iBestResponse << '\n';
		//std::cout << "Accuracy: " << fPhraseMatch << '\n';
		std::cout << sBotName << ": ";
		responses[iBestResponse]();
		std::cout << '\n';
	}

	return 0;
}
