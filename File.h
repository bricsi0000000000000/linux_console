#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <ctime>

class File {
public:
	File(std::string name, int id, File* parent, int size, std::string user, std::string date_created, std::string content = "");
	File();
	virtual ~File();

	void setSize(int number);
	void getAllData();

	const int MAX_FILES_COUNT = 100;
	int getId() const;
	int getSize() const;

	File* getParent() const;

	std::string getName() const;
	std::string getContent() const;
	std::string getUser() const;
	std::string getDateCreated() const;
private:
	int id;
	int size;

	File* parent;

	std::string name;
	std::string content;
	std::string user;

	std::string date_created;
};


#endif // FILE_H
