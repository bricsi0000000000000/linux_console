#ifndef LINK_H
#define LINK_H

#include "Directory.h"

class Link : public File {
public:
	Link(std::string name, int id, Directory* parent, int size, std::string user, std::string date_created, std::string content = "");
	Link();
private:
};

#endif // LINK_H
