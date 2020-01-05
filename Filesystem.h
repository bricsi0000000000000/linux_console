#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "Directory.h"

class Filesystem {
public:
	Filesystem();
	~Filesystem();

	void mkdir(std::string name);
	void ls(std::string name = "");
	void listTree();
	void setActDirectoryId(int number);
	void touch(std::string name, std::string content = "");
	void cat(std::string name);
	void pwd();
	void ln(std::string name, std::string destination);
	void setUser(std::string name);

	static int id;
	int rm(std::string name);
	int cd(std::string name, std::string from, bool ignore_warning = false);
	int getActDirectoryId() const;

	bool isParameterValid(std::string parameter, std::vector<std::string> parameters);

	std::vector<std::string> getSplittedInput(char character, std::string name);
	std::vector<std::string> ls_parameters;

	File* getFile(int id);

	Directory* getRoot() const;

	std::string getUser() const;

	std::string getCurrentDateTime() const;
private:
	int act_file_id;

	Directory* root;

	std::string user;

	HANDLE coloring = GetStdHandle(STD_OUTPUT_HANDLE);

	int cyan = 11;
	int pink = 13;
	int yellow = 14;
	int white = 15;
};

#endif // FILESYSTEM_H
