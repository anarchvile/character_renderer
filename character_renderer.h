// Copyright (c) 2020 Lushe Shipkov.
// Permission is hereby granted, free of charge, to any person obtaining a copy 
// of this software and associated documentation files (the "Software"), to deal 
// in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
// of the Software, and to permit persons to whom the Software is furnished to do 
// so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all 
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.


// Description:
// Compact, easy-to-use library for drawing ASCII symbols using OpenGL.

// Usage
// In the OpenGL rendering context, call either the PixelInitialize or LineInitialize function
// to initialize the relevant data, depending on whether the user would like to draw the symbols
// using lines or points. Then, load the character color and position data of the string being
// rendered using LoadPixelData or LoadLineData, whichever applies. Parameters include x- and y-
// position, scale, character spacing, outlining/color options, and cap on max number of characters
// to draw. The character position vector is now ready to be used for rendering.

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
	static std::vector<float> charPixelPos; // Character positions for pixel mode.
	static std::vector<float> charPixelColor; // Character colors for pixel mode.
	static std::vector<float> charLinePos; // Character positions for line mode.
	static std::vector<float> charLineColor; // Character colors for line mode.
	static std::vector<int> pixelData;
	static std::vector<int> lineData;
	static std::map <int, std::vector<int>> charLineMap;

	std::vector<float> emptyVectorFloat;
	std::vector<int> emptyVectorInt;
	std::map <int, std::vector<int>> emptyMap;

	static void PixelInitialize()
	{
		std::vector<std::string> pixelAscii;
		std::string pixel;
		std::ifstream pixelFile("pixelData.txt");

		// read pixel data into vector as strings.
		if (pixelFile.is_open())
		{
			while (!pixelFile.eof())
			{
				std::getline(pixelFile, pixel);
				pixelAscii.push_back(pixel);
			}
		}

		pixelFile.close();

		// convert strings into corresponding int. example: "0" is read as 0, not its ASCII value of 48.
		// each ASCII symbol is represented by a matrix of 0, 1, and 2, denoting how the symbol should be later colored.
		for (size_t i = 0; i < pixelAscii.size(); ++i)
		{
			for (char& c : pixelAscii[i])
				if (i % 10 != 0) pixelData.push_back((int)c - (int)'0');
		}
	}

	static void LineInitialize()
	{
		std::vector<std::string> lineAscii;
		std::string line;
		std::ifstream lineFile("lineData.txt");

		// read line data into vector as strings. Each character has a list of xy-point pairs of the line endpoints used
		// for drawing that specific symbol.
		if (lineFile.is_open()) 
		{
			while (!lineFile.eof())
			{
				std::getline(lineFile, line);
				lineAscii.push_back(line);
			}
		}

		lineFile.close();

		// convert strings into corresponding int. example: "0" is read as 0, not its ASCII value of 48.
		int __count = -1;
		for (size_t i = 1; i < lineAscii.size(); i += 2)
		{
			++__count;
			lineData.clear();
			for (char& c : lineAscii[i])
				lineData.push_back(c - '0');

			charLineMap[__count] = lineData; // maps every ASCII character to an int storing the line endpoints used for drawing.
		}
	}

	static void InitPixelChar(int key, int xOffset, int yOffset, int scale, int outline, bool whiteOnBlack)
	{
		for (int j = 0; j < 9; ++j) // y-component.
		{
			for (int i = 0; i < 7; ++i) // x-component.
			{
				for (int k = 0; k < scale; ++k) // scale factor - expands initial matrix dimensions to account for scaling.
				{
					for (int l = 0; l < scale; ++l)
					{
						int __idx = i + 7 * j + 63 * (key - 32);

						if (outline == 0) // characters only - no outline or background.
						{
							if (pixelData[__idx] == 1)
							{
								charPixelPos.push_back(scale * i + l + xOffset);
								charPixelPos.push_back(scale * j + k + yOffset);

								if (whiteOnBlack) // white characters, black background.
								{
									charPixelColor.push_back(1);
									charPixelColor.push_back(1);
									charPixelColor.push_back(1);
								}

								else
								{
									charPixelColor.push_back(0);
									charPixelColor.push_back(0);
									charPixelColor.push_back(0);
								}
							}
						}

						else if (outline == 1) // characters with outline.
						{
							if (pixelData[__idx] != 0) // skips pixels that are not involved in the symbol's shape or outline.
							{
								charPixelPos.push_back(scale * i + l + xOffset);
								charPixelPos.push_back(scale * j + k + yOffset);
							}

							if (pixelData[__idx] == 1)
							{
								if (whiteOnBlack)
								{
									charPixelColor.push_back(1);
									charPixelColor.push_back(1);
									charPixelColor.push_back(1);
								}

								else
								{
									charPixelColor.push_back(0);
									charPixelColor.push_back(0);
									charPixelColor.push_back(0);
								}
							}

							else if (pixelData[__idx] == 2)
							{
								if (whiteOnBlack)
								{
									charPixelColor.push_back(0);
									charPixelColor.push_back(0);
									charPixelColor.push_back(0);
								}

								else
								{
									charPixelColor.push_back(1);
									charPixelColor.push_back(1);
									charPixelColor.push_back(1);
								}
							}
						}

						else if (outline == 2) // characters with opposite-color solid background.
						{
							charPixelPos.push_back(scale * i + l + xOffset);
							charPixelPos.push_back(scale * j + k + yOffset);

							if (whiteOnBlack)
							{
								if (pixelData[__idx] == 1)
								{
									charPixelColor.push_back(1);
									charPixelColor.push_back(1);
									charPixelColor.push_back(1);
								}

								else
								{
									charPixelColor.push_back(0);
									charPixelColor.push_back(0);
									charPixelColor.push_back(0);
								}
							}

							else
							{
								if (pixelData[__idx] == 1)
								{
									charPixelColor.push_back(0);
									charPixelColor.push_back(0);
									charPixelColor.push_back(0);
								}

								else
								{
									charPixelColor.push_back(1);
									charPixelColor.push_back(1);
									charPixelColor.push_back(1);
								}
							}
						}
					}
				}
			}
		}
	}

	static void InitLineChar(int key, int xOffset, int yOffset, float scale, bool outline, bool whiteOnBlack)
	{
		if (outline)
		{
			// draws opposite-colored character left of the original.
			for (int i = 0; i < (int)(0.5f * charLineMap[key].size()); ++i)
			{
				charLinePos.push_back(scale * charLineMap[key][2 * i] + xOffset - 1);
				charLinePos.push_back(scale * charLineMap[key][2 * i + 1] + yOffset);

				if (whiteOnBlack)
				{
					charLineColor.push_back(0);
					charLineColor.push_back(0);
					charLineColor.push_back(0);
				}

				else
				{
					charLineColor.push_back(1);
					charLineColor.push_back(1);
					charLineColor.push_back(1);
				}
			}

			// draws opposite-colored character right of the original.
			for (int i = 0; i < (int)(0.5f * charLineMap[key].size()); ++i)
			{
				charLinePos.push_back(scale * charLineMap[key][2 * i] + xOffset + 1);
				charLinePos.push_back(scale * charLineMap[key][2 * i + 1] + yOffset);

				if (whiteOnBlack)
				{
					charLineColor.push_back(0);
					charLineColor.push_back(0);
					charLineColor.push_back(0);
				}

				else
				{
					charLineColor.push_back(1);
					charLineColor.push_back(1);
					charLineColor.push_back(1);
				}
			}

			// draws opposite-colored character above the original.
			for (int i = 0; i < (int)(0.5f * charLineMap[key].size()); ++i)
			{
				charLinePos.push_back(scale * charLineMap[key][2 * i] + xOffset);
				charLinePos.push_back(scale * charLineMap[key][2 * i + 1] + yOffset - 1);

				if (whiteOnBlack)
				{
					charLineColor.push_back(0);
					charLineColor.push_back(0);
					charLineColor.push_back(0);
				}

				else
				{
					charLineColor.push_back(1);
					charLineColor.push_back(1);
					charLineColor.push_back(1);
				}
			}

			// draws opposite-colored character below the original. Combined with the above three loops, an outline
			// is created for the character.
			for (int i = 0; i < (int)(0.5f * charLineMap[key].size()); ++i)
			{
				charLinePos.push_back(scale * charLineMap[key][2 * i] + xOffset);
				charLinePos.push_back(scale * charLineMap[key][2 * i + 1] + yOffset + 1);

				if (whiteOnBlack)
				{
					charLineColor.push_back(0);
					charLineColor.push_back(0);
					charLineColor.push_back(0);
				}

				else
				{
					charLineColor.push_back(1);
					charLineColor.push_back(1);
					charLineColor.push_back(1);
				}
			}
		}

		for (int i = 0; i < (int)(0.5f * charLineMap[key].size()); ++i)
		{
			charLinePos.push_back(scale * charLineMap[key][2 * i] + xOffset);
			charLinePos.push_back(scale * charLineMap[key][2 * i + 1] + yOffset);
				
			if (whiteOnBlack)
			{
				charLineColor.push_back(1);
				charLineColor.push_back(1);
				charLineColor.push_back(1);
			}

			else
			{
				charLineColor.push_back(0);
				charLineColor.push_back(0);
				charLineColor.push_back(0);
			}
		}
	}

	static void LoadPixelChar(std::string text, int xOffset, int yOffset, int scale, int spacing, int outline, bool whiteOnBlack, int maxChar)
	{
		int __charOffset = -spacing * scale;

		for (char& c : text)
		{
			__charOffset += spacing * scale;
			InitPixelChar((int)c, xOffset + __charOffset, yOffset, scale, outline, whiteOnBlack);
			if (maxChar != 0 && __charOffset > maxChar * spacing * scale) break; // set limit on number of characters to draw.
		}
	}

	static void LoadLineChar(std::string text, int xOffset, int yOffset, float scale, float spacing, bool outline, bool whiteOnBlack, int maxChar)
	{
		int __charOffset = -spacing * scale;

		for (char& c : text)
		{
			__charOffset += spacing * scale;
			InitLineChar((int)c - (int)' ', xOffset + __charOffset, yOffset, scale, outline, whiteOnBlack);
			if (maxChar != 0 && __charOffset > maxChar * spacing * scale) break; // set limit on number of characters to draw.
		}
	}
};

// initializing static variables.
CharacterGeneration charGen;
std::vector<float> CharacterGeneration::charPixelPos = charGen.emptyVectorFloat;
std::vector<float> CharacterGeneration::charPixelColor = charGen.emptyVectorFloat;
std::vector<float> CharacterGeneration::charLinePos = charGen.emptyVectorFloat;
std::vector<float> CharacterGeneration::charLineColor = charGen.emptyVectorFloat;
std::vector<int> CharacterGeneration::pixelData = charGen.emptyVectorInt;
std::vector<int> CharacterGeneration::lineData = charGen.emptyVectorInt;
std::map <int, std::vector<int>> CharacterGeneration::charLineMap = charGen.emptyMap;

#endif
