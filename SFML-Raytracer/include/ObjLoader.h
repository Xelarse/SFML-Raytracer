#pragma once
#include "Utilities.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace ObjLoader
{
	static bool LoadObj(std::string filePath, AA::Model& modelOut)
	{
		//Check that the path is actually an obj
		if (filePath.substr(filePath.size() - 4, 4) != ".obj")
		{
			return false;
		}

		//Open the file 
		std::ifstream file(filePath);

		if (!file.is_open())
		{
			return false;
		}

		//Init some empty vectors to store the processed results in
		std::vector<AA::Vec3> vPos;
		std::vector<AA::Vec3> vNorm;
		std::vector<AA::Tri> modelTris;

		//jump through each line of the file and process
		//THIS IS ORDER DEPENDANT v and vn should be populated by the time f is hit

		std::string currentLine;
		while (std::getline(file, currentLine))
		{
			//Get all the vertex positions
			if (FirstToken(currentLine) == 'v')
			{
				//Drop the first two chars and split the string into the 3 co ords
				std::vector<std::string> ords = SplitString(currentLine.substr(2, currentLine.size()), ' ');
				if(ords.size() != 3) { return false; } //Didnt get the expected co ords

				//Create a Vec3 from the resultant 3 strings
				AA::Vec3 res(StringToDouble(ords[0]), StringToDouble(ords[1]), StringToDouble(ords[2]));

				//Add the result to vPos
				vPos.push_back(res);
			}

			//Get all the vertex normals
			if (FirstToken(currentLine) == 'vn')
			{
				//Drop the first two chars and split the string into the 3 co ords
				std::vector<std::string> ords = SplitString(currentLine.substr(3, currentLine.size()), ' ');
				if(ords.size() != 3) { return false; } //Didnt get the expected co ords

				//Create a Vec3 from the resultant 3 strings
				AA::Vec3 res(StringToDouble(ords[0]), StringToDouble(ords[1]), StringToDouble(ords[2]));

				//Add the result to vPos
				vNorm.push_back(res);
			}

			//Create the model Tris based off the faces from the obj
			if (FirstToken(currentLine) == 'f')
			{
				std::vector<AA::Vertex> triVerts;
				triVerts.reserve(3);

				//First split the string into the 3 tri components aka 3x 1/1/1
				std::vector<std::string> rawTriInds = SplitString(currentLine.substr(2, currentLine.size()), ' ');
				if(rawTriInds.size() != 3) { return false; } //Didnt get the expected values to build a tri

				//Process each out to just contain an index of a pos and norm and craft 3 vertex's from this
				for (auto& triInds : rawTriInds)
				{
					//Split each one out into its individual values, Moar stringssssss
					std::vector<std::string> threeVals = SplitString(triInds, '/');
					if(threeVals.size() != 3) { return false; } //Didnt get the expected values to build a vertex

					//Only process the first and last as we currently dont need the Tex cord TODO potentially add later, Remember f starts at 1, need to -1 from every index
					int posInd = StringToInt(threeVals[0]) - 1;
					int normInd = StringToInt(threeVals[2]) - 1;

					AA::Vertex vert(vPos[posInd], vNorm[normInd]);
					triVerts.push_back(vert);
				}

				//Create the tri and push it onto the vector to store it
				if(triVerts.size() != 3) { return false; }
				modelTris.push_back(AA::Tri( { triVerts[0], triVerts[1], triVerts[2] } ));
			}
		}

		//At this point all Tris should be created to make a model and move it to the unique ptr
		modelOut = AA::Model(modelTris);
		return true;
	}

	namespace
	{
		//Utilities related to the obj loading go here

		//Returns the first character of an inputted string
		static char FirstToken(const std::string& in)
		{
			if (!in.empty())
			{
				//TODO clean this up later just returning a char?
				char ref = in.front();
				return in.front();
			}
			return ' ';
		}

		//Splits a string based on the inputted delimiter character
		static std::vector<std::string> SplitString(const std::string& in, char delim)
		{
			std::vector<std::string> out;
			std::string token;
			std::istringstream streamToSplit(in);
			while (std::getline(streamToSplit, token, delim))
			{
				out.push_back(token);
			}
			return out;
		}

		double StringToDouble(const std::string& in)
		{
			std::istringstream i(in);
			double x;
			if (!(i >> x))
			{
				std::cout << "String to double failed on: " << in << std::endl;
				return 0;
			}
			return x;
		}

		int StringToInt(const std::string& in)
		{
			std::istringstream i(in);
			int x;
			if (!(i >> x))
			{
				std::cout << "String to Int failed on: " << in << std::endl;
				return 0;
			}
			return x;
		}
	}
}