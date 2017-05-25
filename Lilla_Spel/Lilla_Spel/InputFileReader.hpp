//#pragma once
//
//#include <string>
//#include <fstream>
//using namespace std;
//
//namespace InputFileReader {
//	inline string IFR_read() {
//		string r = "", line;
//
//		ifstream inFile("input.txt"); // Open file and fetch content.
//		if (inFile.is_open()) {
//			while (getline(inFile, line)) {
//				if (line == "load") { // Read through content of file.
//					getline(inFile, line);
//					if (line == "BTH") {
//						r += "bth ";
//					}
//					else if (line.substr(line.find_last_of(".") + 1, 3) == "obj") { // Check if file ends with ".obj".
//						r += "obj ";
//					}
//				}
//				if (line == "blur") {
//					r += "blur ";
//				}
//				else if (line == "viewfrustrum") {
//					r += "viewfrustrum ";
//				}
//			}
//		}
//		inFile.close();
//
//		return r;
//	}
//}
