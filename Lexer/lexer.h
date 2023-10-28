#pragma once
#include <algorithm>
#include <string>
#include <vector>

#define PRESORTED_VECTORS                                                      \
    false // Enable if vectors are already sorted by descending length. Improves
          // performance.

std::vector<std::string>
lex(const std::string &input,
#if PRESORTED_VECTORS
    const std::vector<std::string> &tokens = {},
    const std::vector<std::string> &delimiters = {},
    const std::vector<std::string> &splitters = {},
#else
    std::vector<std::string> tokens = {},
    std::vector<std::string> delimiters = {},
    std::vector<std::string> splitters = {},
#endif
    const std::string &toggler = "\0") {

    std::vector<std::string> output;
    size_t currentChar = 0;
    std::string currentString;
    bool stringMode = false;

    auto foundToken = [&](const std::string &token, const size_t start) {
        return input.find(token, start) == start;
    };
    auto addCurrentString = [&]() {
        if (currentString.length() > 0) {
            output.push_back(currentString);
            currentString = "";
        }
    };

#if !PRESORTED_VECTORS
    auto vectorSortMethod = [](const std::string &string1,
                               const std::string &string2) {
        return string1.length() > string2.length();
    };

    std::sort(tokens.begin(), tokens.end(), vectorSortMethod);
    std::sort(delimiters.begin(), delimiters.end(), vectorSortMethod);
    std::sort(splitters.begin(), splitters.end(), vectorSortMethod);
#endif

Start:
    while (currentChar < input.length()) {
        if (toggler != "\0" && foundToken(toggler, currentChar)) {
            if (stringMode) {
                output.push_back(currentString + toggler);
                currentString = "";
            } else {
                if (currentString.length() > 0)
                    output.push_back(currentString);
                currentString = toggler;
            }
            stringMode ^= true;
            currentChar += toggler.length();
        } else if (stringMode) {
            currentString += input[currentChar++];
        } else {
            for (auto searchToken : splitters) {
                if (foundToken(searchToken, currentChar)) {
                    addCurrentString();
                    currentChar += searchToken.length();
                    goto Start;
                }
            }
            for (auto searchToken : delimiters) {
                if (foundToken(searchToken, currentChar)) {
                    addCurrentString();
                    output.push_back(searchToken);
                    currentChar += searchToken.length();
                    goto Start;
                }
            }
            for (auto searchToken : tokens) {
                if (foundToken(searchToken, currentChar)) {
                    if (currentChar + searchToken.length() > input.length() ||
                        foundToken(
                            toggler, currentChar + searchToken.length()
                        )) {
                        output.push_back(searchToken);
                        currentChar += searchToken.length();
                        goto Start;
                    }
                    for (auto searchToken2 : splitters) {
                        if (foundToken(
                                searchToken2, currentChar + searchToken.length()
                            )) {
                            output.push_back(searchToken);
                            currentChar += searchToken.length();
                            goto Start;
                        }
                    }
                    for (auto searchToken2 : delimiters) {
                        if (foundToken(
                                searchToken2, currentChar + searchToken.length()
                            )) {
                            output.push_back(searchToken);
                            currentChar += searchToken.length();
                            goto Start;
                        }
                    }
                }
            }
            currentString += input[currentChar++];
        }
    }
    addCurrentString();
    return output;
}