#define TYPE 1
#define NAME 2

/* countLinesInFile
 * Returns number of lines in a file
 */
int countLinesInFile(std::string path){//opens and closes file @path
	std::ifstream file(path);//.c_str());
	if (!file.is_open()) {
		LOGERR("getRandomStringFromFile: Error opening file for reading: \"" << path << "\"", 1);
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
		LOGERR("getRandomStringFromFile: Error opening file for reading: \"" << path << "\"", 1);
	}
	getline(file, str);
	for (int i = 0; i < j; i++) {
		getline(file, str);
		getline(file, str);
	}
	file.close();
	return trimCRLF(str);
}

std::string getRandomTypeFromFile(std::string path){
	std::string str;
	//LOGD("getRandomTypeFromFile: " << path << ", lines: " << countLinesInFile(path));
	int j = rndm(0, countLinesInFile(path));
	std::fstream file(path);//.c_str());
	if (!file.is_open()) {
		LOGERR("getRandomStringFromFile: Error opening file for reading: \"" << path << "\"", 1);
	}
	getline(file, str);
	for (int i = 0; i < j; i++) {
		getline(file, str);
	}
	file.close();
	return trimCRLF(str);
}