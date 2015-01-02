/* countLinesInFile
 * Returns number of lines in a file
 */
int countLinesInFile(std::string path){//opens and closes file @path
	std::ifstream file(path.c_str());
	int nLines = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
	file.close();
	return nLines;
}

std::string getRandomStringFromFile(std::string path){
	int lines = countLinesInFile(path);
	std::string str;
	int i, j = rndm(0, lines);
	std::fstream file(path.c_str());
	if (!file.is_open()) {
		std::cout << "Error opening file for reading: " << path;
		return "";
	}
	getline(file, str);
	for (i = 0; i < j; i++) {
		getline(file, str);
	}
	file.close();
	return std::string(str.c_str());
}
