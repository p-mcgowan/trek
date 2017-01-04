warnings = -W -Wall -Wextra -ansi -Werror
standard = -std=gnu++0x
all: cTrekGame.cpp; rm -f trek trek.exe; g++ -g cTrekGame.cpp $(warnings) -std=gnu++0x -o trek 2>&1 |tee build.log
strict: cTrekGame.cpp; rm -f trek trek.exe; g++ -g cTrekGame.cpp $(warnings) -std=gnu++0x -pedantic -o trek 2>&1 |tee build.log

