#define TYPE 1
#define NAME 2

/* countLinesInFile
 * Returns number of lines in a file
 */
int countLinesInFile(std::string path){//opens and closes file @path
	std::ifstream file(path);//.c_str());
	if (!file.is_open()) {
		std::cout << "countLinesInFile: Error opening file for reading: \"" << path << "\"" <<std::endl;
		assert(true);
	}
	int nLines = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
	file.close();
	return nLines;
}

std::string getRandomStringFromFile(std::string path){
	std::string str;
	int lines = countLinesInFile(path);
	int j = rndm(0, lines/2);
	std::fstream file(path);//.c_str());
	if (!file.is_open()) {
		std::cout << "getRandomStringFromFile: Error opening file for reading: \"" << path << "\"" <<std::endl;
		assert(true);
	}
	getline(file, str);
	for (int i = 0; i < j; i++) {
		getline(file, str);
		getline(file, str);
	}
	file.close();
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	return str.c_str();
}

std::string getRandomTypeFromFile(std::string path){
	std::string str;
	std::cout << "getRandomTypeFromFile: " << path << ", lines: ";
	std::cout << countLinesInFile(path) << std::endl;
	int j = rndm(0, countLinesInFile(path));
	std::fstream file(path);//.c_str());
	if (!file.is_open()) {
		std::cout << "getRandomTypeFromFile: Error opening file for reading: \"" << path << "\"" <<std::endl;
		assert(true);
	}
	getline(file, str);
	for (int i = 0; i < j; i++) {
		getline(file, str);
	}
	file.close();
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	return str.c_str();
}