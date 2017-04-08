#pragma once



#include "Vertex.h"
using namespace std;

namespace OBJLoader {
	inline void OL_loadVertex(Vertex**& vtxA, int*& vertexCount, int& objCount, float*& objCoords) {
		string line;
		ifstream inFile("input.txt"); // Open file and fetch content.
		if (inFile.is_open()) {
			while (getline(inFile, line)) {
				if (line == "load") {
					//while (getline(inFile, line) && line.substr(line.find_last_of(".") + 1, 3) == "obj") {
					for (int k = 0; k < objCount; k++) {
						getline(inFile, line);
						string address = line.substr(0, line.find_last_of("."));
						int vCount = 0, vtCount = 0, fCount = 0;
						ifstream inFile2(address + ".obj"); // Open file and fetch content.
						if (inFile2.is_open()) {
							while (getline(inFile2, line)) {
								if (line.substr(0, 2) == "v ") {
									vCount++;
								}
								else if (line.substr(0, 3) == "vt ") {
									vtCount++;
								}
								else if (line.substr(0, 2) == "f ") {
									fCount++;
								}
							}
						}
						inFile2.close();

						string *v = new string[vCount], *vt = new string[vtCount], *f = new string[fCount];
						int vC = 0, vtC = 0, fC = 0;
						inFile2.open(address + ".obj"); // Open file and fetch content.
						if (inFile2.is_open()) {
							string line;
							while (getline(inFile2, line)) {
								if (line.substr(0, 2) == "v ") {
									v[vC] = line.substr(2); vC++;
								}
								else if (line.substr(0, 3) == "vt ") {
									vt[vtC] = line.substr(3); vtC++;
								}
								else if (line.substr(0, 2) == "f ") {
									f[fC] = line.substr(2); fC++;
								}
							}
						}
						inFile2.close();

						vtxA[k] = new Vertex[fC * 3];
						vertexCount[k] = fC * 3;
						for (int i = 0; i < fC; i++) {
							int lastPos = 0;
							for (int j = 2; j > -1; j--) {
								int slashPos = f[i].find('/', lastPos);
								int spacePos = f[i].find(' ', lastPos);
								int vI = stoi(f[i].substr(lastPos, slashPos - lastPos)) - 1;
								int vtI = stoi(f[i].substr(slashPos + 1, spacePos - slashPos)) - 1;

								int vFirstSpacePos = v[vI].find(' ');
								int vSecondSpacePos = v[vI].find(' ', vFirstSpacePos + 1);
								int vtSpacePos = vt[vtI].find(' ');

								vtxA[k][i * 3 + j] = { -(stof(v[vI].substr(0, vFirstSpacePos))),
									stof(v[vI].substr(vFirstSpacePos + 1, vSecondSpacePos - vFirstSpacePos)),
									stof(v[vI].substr(vSecondSpacePos + 1)),
									stof(vt[vtI].substr(0, vtSpacePos)),
									stof(vt[vtI].substr(vtSpacePos + 1))
								};

								lastPos = spacePos + 1;
							}
						}
						delete[] v; delete[] vt; delete[] f;
					}
					getline(inFile, line);
					objCoords = new float[objCount * 3];
					if (line == "position") {
						for (int k = 0; k < objCount; k++) {
							getline(inFile, line);
							float x = stof(line.substr(0, line.find_first_of(' ')));
							float y = stof(line.substr(line.find_first_of(' ') + 1, line.find_last_of(' ')));
							float z = stof(line.substr(line.find_last_of(' ') + 1));
							objCoords[k * 3] = x; objCoords[k * 3 + 1] = y; objCoords[k * 3 + 2] = z;
						}
					}
					else {
						for (int k = 0; k < objCount; k++) {
							objCoords[k * 3] = 0; objCoords[k * 3 + 1] = 0; objCoords[k * 3 + 2] = 0;
						}
					}
				}
			}
		}
		inFile.close();
	}

	inline string OL_getTextureFile(int objN, string map) {
		string line, r = "";

		ifstream file("input.txt");
		if (file.is_open()) {
			while (getline(file, line)) {
				if (line == "load") {
					for (int i = 0; i < objN + 1; i++) {
						getline(file, line);
					}
					break;
				}
			}
		}
		file.close();

		string address = line;
		file.open(line.substr(0, line.find_last_of(".")) + ".mtl");
		if (file.is_open()) {
			while (getline(file, line)) {
				if (map == "diffuse" && line.substr(0, 7) == "map_Kd ") {
					r = address.substr(0, address.find_last_of("\\") + 1) + line.substr(7);
					break;
				}
				else if (map == "normal" && line.substr(0, 5) == "bump ") {
					r = address.substr(0, address.find_last_of("\\") + 1) + line.substr(5, line.find(' ', line.find_last_of('.')) - 5);
					break;
				}
			}
		}
		file.close();

		return r;
	}

	inline int OL_getOBJCount() {
		int r = 0;
		string line;
		ifstream inFile("input.txt"); // Open file and fetch content.
		if (inFile.is_open()) {
			while (getline(inFile, line)) {
				if (line == "load") {
					while (getline(inFile, line) && line.substr(line.find_last_of(".") + 1, 3) == "obj") {
						r++;
					}
				}
			}
		}
		return r;
	}
}
