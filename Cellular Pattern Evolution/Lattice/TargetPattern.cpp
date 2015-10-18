#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "Headers/TargetPattern.h"

namespace lattice
{
	/**
		Constructor fills the inner structure based on a file
	*/
	target_pattern::target_pattern(
		unsigned int desired_width, unsigned int desired_height, string filepath
	)
	{
		ifstream infile;
		infile.open(filepath.c_str(), ios::in);

		if (!infile)
			throw invalid_argument("Could not open target pattern file: " + filepath);

		unsigned int width, height;
		infile >> width >> height;
		if (width != desired_width || height != desired_height)
			throw invalid_argument("Mismatch between target pattern size and CA size!");

		string line, token;
		istringstream istline, isttoken;
		unsigned int r, g, b;

		getline(infile, line);
		while (getline(infile, line))
		{
			_pattern.push_back(vector<rgb>());

			// format (r,g,b) with delimeter ';'
			replace(line.begin(), line.end(), ';', ' ');
			istline.str(line);
			while (istline >> token)
			{
				token = string(token.begin() + 1, token.end() - 1);
				replace(token.begin(), token.end(), ',', ' ');
				isttoken.str(token);
				
				isttoken >> r >> g >> b;
				_pattern.back().push_back(rgb { r, g, b });
				isttoken.clear();
			}
			istline.clear();
		}

		infile.close();
	}
	
	/**
		Returns the element on the given position
	*/
	rgb target_pattern::at(unsigned int x, unsigned int y)
	{
		if (x < 0 || x >= _pattern.size() || y < 0 || y >= _pattern.size())
			throw invalid_argument("Out of range at target pattern!");

		return _pattern[y][x];
	}
}