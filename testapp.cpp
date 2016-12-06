#include "stdafx.h"

#include <string>
#include <vector>
#include <algorithm>

struct Letter
{
	char letter;
	int freq;

	Letter() : freq(0) {}
	Letter(char letter, int freq) {
		this->freq = freq;
		this->letter = letter;
	}
};

bool greater(const Letter& a, const Letter& b)
{
	if (a.freq == b.freq) 
	{
		return a.letter < b.letter;
	}
	return a.freq > b.freq;
}

std::ofstream outputStream("output.txt");

std::string extractChecksum(std::string roomText)
{
	auto startIndex = roomText.find_first_of('[') + 1;
	auto endIndex = roomText.find_last_of(']');
	if (startIndex < roomText.length() && endIndex < roomText.length())
	{
		auto checksum = roomText.substr(startIndex, endIndex - startIndex);
		return checksum;
	}
	return "";
}

bool verifyChecksum(std::string roomText) 
{
	auto checksum = extractChecksum(roomText);
	if (checksum.length() != 5) { return false; }

	int letters[26] = { 0 };
	std::vector<Letter> count;
		
	auto startIndex = roomText.find_first_of('[') + 1;
	for (int i = 0; i < startIndex - 1; ++i)
	{
		auto letter = roomText[i];
		if (isalpha(letter))
		{
			auto letterIndex = (int)(tolower(letter) - 'a');
			letters[letterIndex]++;
		}
	}

	for (int i = 0; i < 26; ++i)
	{
		if (letters[i] > 0)
		{
			count.push_back(Letter('a' + i, letters[i]));
		}
	}

	std::sort(count.begin(), count.end(), greater);
	if (count.size() < 5)
	{
		return false;
	}

	auto checksumCorrect = true;
	for (int i = 0; i < 5; ++i)
	{
		if (count[i].letter != checksum[i])
		{
			checksumCorrect = false;
			break;
		}
	}
	return checksumCorrect;
}

bool extractRoomInfo(std::string roomText, std::string& roomName, int& roomId) 
{
	auto startId = roomText.find_last_of('-') + 1;
	auto endId = roomText.find_first_of('[');
	if (startId < roomText.length() && endId < roomText.length())
	{
		auto idString = roomText.substr(startId, endId - startId);
		roomId = atoi(idString.c_str());
		roomName = roomText.substr(0, startId);
		return true;
	}
	return false;
}

// <room name>  - <id>  [  <checksum>   ]
//aaaaa-bbb-z-y-x   -   123 [abxyz]
int processRoom(std::string roomText) 
{
	std::string roomName = "";
	int roomId = 0;

	if (verifyChecksum(roomText))
	{
		extractRoomInfo(roomText, roomName, roomId);

		for (int i = 0; i< roomName.length(); ++i)
		{
			if (roomName[i] == '-')
			{
				roomName[i] = ' ';
			}
			else
			{
				auto val = (int)(roomName[i] - 'a');
				val = (val + roomId) % 26;
				roomName[i] = ('a' + val);
			}
		}

		outputStream << roomName << "  : id: " << roomId << std::endl;
		if (roomName.find("northpole object storage") < roomName.length())
		{
			std::cout << "northpole storage id = " << roomId << std::endl;
		}
	}
	return roomId;
}

void day4() 
{
	std::ifstream fileStream("day4_input.txt");
	int idSum = 0;
	for (std::string line; std::getline(fileStream, line); )
	{
		idSum += processRoom(line);
	}
	std::cout << "final id sum: " << idSum << std::endl;
}

int main()
{
	day4();
    return 0;
}
