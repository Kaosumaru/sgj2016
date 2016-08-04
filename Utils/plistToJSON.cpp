// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Entry
{
	string name;
	unsigned x,y;
	unsigned w,h;

	unsigned colorX,colorY;
	unsigned colorW,colorH;
	unsigned origW,origH;

};

std::string betweenStrings(const string &input, const string &s1, const string &s2)
{
	auto f1 = input.find(s1);
	if (f1 == string::npos || f1 + s1.size() >= input.size())
		return "";
	f1 += s1.size();
	auto f2 = input.find(s2,f1);
	return input.substr(f1, f2 - f1);
}


bool LoadEntries(const string& filename, vector<Entry> &entries)
{
	ifstream infile;
	infile.open(filename);

	bool found_entry = false;
	string line;
	while (getline(infile, line))
	{
		if (!found_entry)
		{
			if (line.find("--FRAME") == string::npos)
				continue;
			found_entry = true;
			Entry e;
			e.name = betweenStrings(line, " \"", "\":");
			entries.push_back(e);
			continue;
		}

		char a = 0;
		auto pos = line.find_first_not_of(" \t");
		if (pos != string::npos)
			a = line[pos];
		unsigned *pointer = nullptr;

		if (a == 'x')
			pointer = &(entries.back().x);
		else if (a == 'y')
			pointer = &(entries.back().y);
		else if (a == 'w')
			pointer = &(entries.back().w);
		else if (a == 'h')
			pointer = &(entries.back().h);
		else if (a == '}')
		{
			found_entry = false;
			continue;
		}
		else
			continue;

		if (pointer)
		{
			auto value = betweenStrings(line, "= ", ",");
			*pointer = stod(value);
		}

	}

	infile.close();
	return true;
}

bool SaveEntries(const string& filename, vector<Entry> &entries)
{
	ofstream file;
	file.open(filename);

	file << "{" << endl;
	file << "\t\"" << "Resources" << "\":" << endl;
	file << "\t{" << endl;

	file << "\t\t\"" << "Frames" << "\":" << endl;
	file << "\t\t[" << endl;

	for (auto &entry : entries)
	{
		stringstream ss;
		ss << "[\"" << entry.name << "\"," << entry.x << "," << entry.y << "," << entry.w << "," << entry.h << "],"; 

		string entry;
		file << "\t\t\t" << ss.str() << endl;
	}

	file << "\t\t]" << endl;
	file << "\t}" << endl;
	file << "}" << endl;



	file.close();
	return true;
}

bool LoadEntriesPlist(const string& filename, vector<Entry> &entries)
{
	ifstream infile;
	infile.open(filename);

	bool found_entry = false;
	string line;

	while (getline(infile, line))
		if (line.find("<key>frames</key>") != string::npos)
			break;
	

	while (getline(infile, line))
	{
		if (!found_entry)
		{
			if (line.find("<key>") == string::npos)
				continue;
			found_entry = true;
			Entry e;
			e.name = betweenStrings(line, "<key>", "</key>");
			entries.push_back(e);
			
		}

		if (line.find("</dict>") != string::npos)
		{
			found_entry = false;
			continue;
		}

		auto &entry = entries.back();
		string line2;
		getline(infile, line2);

		auto property = betweenStrings(line, "<key>", "</key>");
		auto value = betweenStrings(line2, "<string>", "</string>");

		if (property == "spriteColorRect")
		{
			sscanf_s(value.c_str(), "{{%d, %d}, {%d, %d}}", &entry.colorX, &entry.colorY, &entry.colorW, &entry.colorH);
		}
		else if (property == "spriteSourceSize")
		{
			sscanf_s(value.c_str(), "{%d, %d}", &entry.origW, &entry.origH);
		}
		else if (property == "textureRect")
		{
			sscanf_s(value.c_str(), "{{%d, %d}, {%d, %d}}", &entry.x, &entry.y, &entry.w, &entry.h);
		}

	}

	infile.close();
	return true;
}

bool SaveEntriesPlist(const string& filename, vector<Entry> &entries)
{
	ofstream file;
	file.open(filename);

	file << "{" << endl;
	file << "\t\"" << "Resources" << "\":" << endl;
	file << "\t{" << endl;

	file << "\t\t\"" << "Frames" << "\":" << endl;
	file << "\t\t[" << endl;

	for (auto &entry : entries)
	{
		string tab1 = "\t\t\t";
		string tab = "\t\t\t\t";


	
		file << tab1 << "{" << endl;

		file << tab << "\"Name\": \"" << entry.name << "\"," <<  endl;
		file << tab << "\"Rect\": [" << entry.x << ", " << entry.y << ", " << entry.w << ", " << entry.h << "]," <<  endl;
		file << tab << "\"OriginalRect\": [" << entry.colorX << ", " << entry.colorY << ", " << entry.colorW << ", " << entry.colorH << "]," <<  endl;
		file << tab << "\"OriginalSize\": [" << entry.origW << ", " << entry.origH << "]," <<  endl;


		file << tab1 << "}," << endl;

	}

	file << "\t\t]" << endl;
	file << "\t}" << endl;
	file << "}" << endl;



	file.close();
	return true;
}


int _tmain(int argc, _TCHAR* argv[])
{
	/*
	if (argc < 2)
	{
		cout << "Too little arguments" << std::endl;
		return 0;
	}
	*/
	vector<Entry> entries;

	string fileName = argv[1];

	fileName = "D:/GitHub/Repository/ProjectCrimson/ProjectCrimson/Res/Enemies/Bug2/Bug2_UntitledSheet-ipadhd.plist";
	LoadEntriesPlist(fileName, entries);
	SaveEntriesPlist(fileName + ".json", entries);


	return 0;
}

