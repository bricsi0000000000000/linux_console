#include "File.h"

File::File(std::string name, int id, File* parent, int size, std::string user, std::string date_created, std::string content)
	:name(name), id(id), parent(parent), size(size), user(user), date_created(date_created), content(content) {}

File::File() {}

File::~File() {}

void File::setSize(int number) {
	size = number;
}

void File::getAllData() {
	std::cout << user << '\t' << size << '\t' << date_created << '\t';
}

int File::getId() const {
	return id;
}

int File::getSize() const {
	return size;
}

File* File::getParent() const {
	return parent;
}

std::string File::getName() const {
	return name;
}

std::string File::getContent() const {
	return content;
}

std::string File::getUser() const {
	return user;
}

std::string File::getDateCreated() const {
	return date_created;
}