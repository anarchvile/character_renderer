#ifndef CHARACTER_RENDERER_H
#define CHARACTER_RENDERER_H

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>

class CharacterGeneration
{
public:
	static std::vector<float> charPos;
	static std::vector<float> charColor;
	static std::vector<int> charValuePair;

	std::vector<float> emptyVectorFloat;
	std::vector<int> emptyVectorInt;

	CharacterGeneration() 
	{
		std::vector<std::string> ascii;
		std::string line;
		std::ifstream file("ascii.txt");

		std::vector<int> temp;

		if (file.is_open())
		{
			while (!file.eof())
			{
				std::getline(file, line);
				ascii.push_back(line);
			}
		}

		file.close();

		for (int i = 0; i < ascii.size(); ++i)
			for (char& c : ascii[i])
				if (i % 10 != 0) charValuePair.push_back(c - '0');
	}

	static void InitChar(int key, int xOffset, int yOffset, int scale, bool first)
	{
		if (first)
		{
			for (int j = 0; j < 9; ++j)
			{
				for (int k = 0; k < scale; ++k)
				{
					for (int l = 0; l < scale; ++l)
					{
						charPos.push_back((int)(-scale + l + xOffset));
						charPos.push_back((int)(scale * j + k + yOffset));

						charColor.push_back(0);
						charColor.push_back(0);
						charColor.push_back(0);
					}
				}
			}
		}


		for (int j = 0; j < 9; ++j)
		{
			for (int i = 0; i < 5; ++i)
			{
				for (int k = 0; k < scale; ++k)
				{
					for (int l = 0; l < scale; ++l)
					{
						charPos.push_back(scale * i + l + xOffset);
						charPos.push_back(scale * j + k + yOffset);

						if (charValuePair[i + 5 * j + 45 * (key - 32)] == 1)
						{
							charColor.push_back(1);
							charColor.push_back(1);
							charColor.push_back(1);
						}

						else if (charValuePair[i + 5 * j + 45 * (key - 32)] == 0)
						{
							charColor.push_back(0);
							charColor.push_back(0);
							charColor.push_back(0);
						}
					}
				}
			}
		}

		for (int j = 0; j < 9; ++j)
		{
			for (int k = 0; k < scale; ++k)
			{
				for (int l = 0; l < scale; ++l)
				{
					charPos.push_back((int)(scale * 5 + l + xOffset));
					charPos.push_back((int)(scale * j + k + yOffset));

					charColor.push_back(0);
					charColor.push_back(0);
					charColor.push_back(0);
				}
			}
		}
	}

	static void LoadChar(std::string text, int xOffset, int yOffset, int scale)
	{
		int __charOffset = -6 * scale;
		bool __first = true;

		for (char& c : text)
		{
			__charOffset += 6 * scale;
			if (__charOffset == 0) __first = true;
			else __first = false;
			InitChar((int)c, xOffset + __charOffset, yOffset, scale, __first);
			if (__charOffset > 30 * scale) break;
		}
	}
};

CharacterGeneration charGen;

std::vector<float> CharacterGeneration::charPos = charGen.emptyVectorFloat;
std::vector<float> CharacterGeneration::charColor = charGen.emptyVectorFloat;
std::vector<int> CharacterGeneration::charValuePair = charGen.emptyVectorInt;

#endif