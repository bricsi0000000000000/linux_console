#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <Windows.h>
#include <vector>

#include "File.h"

class Directory : public File {
public:
	Directory(std::string name, int id, Directory* parent, int size, std::string user, std::string date_created);
	Directory();
	~Directory();

	void addFile(File* file);
	void ls(bool all, std::string parameter, int tab_count = 1);
	void removeElement(int index);

	File* getFile(int id);

	std::vector<File*> getFiles() const;
	std::vector<File*> searchLevel(int search_id);

private:
	std::vector<File*> files;

	HANDLE coloring = GetStdHandle(STD_OUTPUT_HANDLE);

	int cyan = 11;
	int pink = 13;
	int yellow = 14;
	int white = 15;
};



#endif // DIRECTORY_H
