#pragma once
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <string_view>

#define PRESORTED_VECTORS false //Enable if vectors are already sorted by descending length. Improves performance.

template<size_t S1, size_t S2, size_t S3>
std::vector<std::string> Lex(const std::string& input,
#if PRESORTED_VECTORS
							 const std::array<std::string_view, S1>& tokens = {},
							 const std::array<std::string_view, S2>& delimiters = {},
							 const std::array<std::string_view, S3>& splitters = {},
#else
							 std::array<std::string_view, S1> tokens = {},
							 std::array<std::string_view, S2> delimiters = {},
							 std::array<std::string_view, S3> splitters = {},
#endif
							 std::string_view toggler = "\0") {

	std::vector<std::string> output;
	size_t currentChar = 0;
	std::string currentString;
	bool stringMode = false;

	auto foundToken = [&](const std::string_view & token, const size_t start) {
		return input.find(token, start) == start;
	};
	auto addCurrentString = [&]() {
		if(currentString.length() > 0) {
			output.push_back(currentString);
			currentString = "";
		}
	};

#if !PRESORTED_VECTORS
	auto vectorSortMethod = [](const std::string_view & string1, const std::string_view & string2) {
		return string1.length() > string2.length();
	};

	std::sort(tokens.begin(), tokens.end(), vectorSortMethod);
	std::sort(delimiters.begin(), delimiters.end(), vectorSortMethod);
	std::sort(splitters.begin(), splitters.end(), vectorSortMethod);
#endif

Start:
	while(currentChar < input.length()) {
		if(toggler != "\0" && foundToken(toggler, currentChar)) {
			if(stringMode) {
				output.push_back(currentString + std::string(toggler));
				currentString = "";
			} else {
				if(currentString.length() > 0)
					output.push_back(currentString);
				currentString = toggler;
			}
			stringMode ^= true;
			currentChar += toggler.length();
		} else if(stringMode) {
			currentString += input[currentChar++];
		} else {
			for(auto searchToken : splitters) {
				if(foundToken(searchToken, currentChar)) {
					addCurrentString();
					currentChar += searchToken.length();
					goto Start;
				}
			}
			for(auto searchToken : delimiters) {
				if(foundToken(searchToken, currentChar)) {
					addCurrentString();
					output.push_back(std::string(searchToken));
					currentChar += searchToken.length();
					goto Start;
				}
			}
			for(auto searchToken : tokens) {
				if(foundToken(searchToken, currentChar)) {
					if(currentChar + searchToken.length() > input.length() || foundToken(toggler, currentChar + searchToken.length())) {
						output.push_back(std::string(searchToken));
						currentChar += searchToken.length();
						goto Start;
					}
					for(auto searchToken2 : splitters) {
						if(foundToken(searchToken2, currentChar + searchToken.length())) {
							output.push_back(std::string(searchToken));
							currentChar += searchToken.length();
							goto Start;
						}
					}
					for(auto searchToken2 : delimiters) {
						if(foundToken(searchToken2, currentChar + searchToken.length())) {
							output.push_back(std::string(searchToken));
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