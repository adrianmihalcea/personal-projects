// @Copyright 2021 Mihalcea Adrian - 334CA

#include <string>

using namespace std;

// returns the capitalized consonnant if input is consonnant
char Is_Consonnant(char c) {
	if (c < 'a') {
		c += 'a' - 'A';
	}

	if (c > 'a' && c <= 'z' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
		return c;
	}

	return 0;
}

void Change_Comedy(string& line) {
	int last_delimiter = -1;
	for (int i = 0; i < line.length(); ++i) {
		if (line[i] == ' ') {
			last_delimiter = i;
			continue;
		}
		if ((i - last_delimiter) % 2 == 0) {
			// even-positioned leter
			if (line[i] >= 'a' && line[i] <= 'z') {
				line[i] -= 'a' - 'A';
			}
		}
	}
}

void Change_Horror(string& line) {
	string result;
	for (int i = 0; i < line.length(); ++i) {
		result += line[i];
		char c = Is_Consonnant(line[i]);
		if (c != 0) {
			result += c;
		}
	}
	line = string(result);
}

void Change_SF(string& line) {
	int words = 1;

	for (int i = 0; i < line.length(); ++i) {
		if (words == 7) {
			words = 0;
			string word = "";
			int j = i;
			while (line[j] != ' ' && line[j] != '\n' && line[j] != '\0') {
				word += line[j++];
			}
			for (int k = 0; k < word.length(); ++k) {
				line[k + i] = word[word.length() - k - 1];
			}
			continue;
		}
		if (line[i] == ' ') {
			++words;
		}
	}
}

void Change_Fantasy(string& line) {
	int last_delimiter = -1;
	for (int i = 0; i < line.length(); ++i) {
		if (line[i] == ' ') {
			last_delimiter = i;
			continue;
		}
		if (i == last_delimiter + 1) {
			if (line[i] >= 'a' && line[i] <= 'z') {
				line[i] -= 'a' - 'A';
			}
		}
	}
}