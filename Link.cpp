#include "Link.h"

Link::Link(std::string name, int id, Directory* parent, int size, std::string user, std::string date_created, std::string content)
	: File(name, id, parent, size, user, date_created, content) {}

Link::Link() {}
