/* countLinesInFile
 * Returns number of lines in a file
 */
int countLinesInFile(std::string path){//opens and closes file @path
  std::ifstream file(path);
  if (!file.is_open()) {
    LOGERR("getRandomStringFromFile: Error opening file for reading: \"" << path << "\"", 1);
  }
  int nLines = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
  file.close();
  return nLines;
}

std::string getRandomStringFromFile(std::string path, bool mode = true) {
  std::string str;
  int j, lines = countLinesInFile(path);
  if (mode)
    j = rndm(0, lines/2);
  else
    j = rndm(0, lines);
  std::fstream file(path);
  if (!file.is_open()) {
    LOGERR("getRandomStringFromFile: Error opening file for reading: \"" << path << "\"", 1);
  }
  getline(file, str);
  for (int i = 0; i < j; i++) {
    getline(file, str);
    if (mode)
      getline(file, str);
  }
  file.close();
  return trimCRLF(str);
}