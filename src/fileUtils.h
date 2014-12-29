 /**
int fcountstrings(std::string path){//opens and closes file @path
	std::ifstream file(path);
	int nLines = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
	file.close();
	return nlines;
}

std::string fgetrandomstring(std::string path){
	int lines=fcountstrings(path);
	std::string str;
	int i, j = rndm(0,fcountstrings(path));
	std::fstream file(path);
	if (!file.is_open()) {
		std::cout
		return "";
	}
	getline(file, str);
	for (i=0;i<j;i++){
		getline(file, str);
	}
	fclose(file);
	i=0;
	while(str[i]!='.'){
		strcpy[i]=str[i];
		i++;
	}
	strcpy[i]='\0';
	return strcpy;
}*/