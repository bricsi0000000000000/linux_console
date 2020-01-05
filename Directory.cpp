#include "Directory.h"
#include "Link.h"

Directory::Directory(std::string name, int id, Directory* parent, int size, std::string user, std::string date_created)
	: File(name, id, parent, size, user, date_created)
{
}

Directory::Directory() {}

Directory::~Directory() {
	for (const auto& file : files) {
		delete file;
	}
}

void Directory::addFile(File* file) {
	files.push_back(file);
}

void Directory::ls(bool all, std::string parameter, int tab_count) {

	if (files.size() < MAX_FILES_COUNT) {
		for (const auto& file : files) {

			SetConsoleTextAttribute(coloring, white);

			if (all) {
				for (int i = 0; i <= tab_count; i++) {
					std::cout << "\t";
				}
			}

			if (parameter == "-l") {
				file->getAllData();
			}

			if (dynamic_cast<Link*>(file)) {
				SetConsoleTextAttribute(coloring, pink);
			}
			else if (dynamic_cast<Directory*>(file)) {
				SetConsoleTextAttribute(coloring, cyan);
			}
			else {
				SetConsoleTextAttribute(coloring, white);
			}

			std::cout << file->getName();

			if (dynamic_cast<Directory*>(file)) {
				std::cout << '/';
			}
			std::cout << '\n';
			if (all) {
				static_cast<Directory*>(file)->ls(true, parameter, tab_count + 1);
			}
		}
	}
}

void Directory::removeElement(int index) {
	files.erase(files.begin() + index);
}

File* Directory::getFile(int id) {
	File* new_file = nullptr;
	if (files.size() < MAX_FILES_COUNT) {
		for (const auto& file : files) {
			if (file->getId() == id) {
				return file;
			}
			else {
				new_file = static_cast<Directory*>(file)->getFile(id);
				if (new_file != nullptr) {
					return new_file;
				}
			}
		}
	}

	return new_file;
}

std::vector<File*> Directory::getFiles() const {
	return files;
}

std::vector<File*> Directory::searchLevel(int search_id) {
	std::vector<File*> path;

	int s_id = search_id;

	bool stop = false;

	while (!stop)
	{
		File* file = getFile(s_id);

		if (file != nullptr) {
			if (file->getName() != "") {
				path.push_back(file);
			}
			s_id = file->getParent()->getId();
		}
		else {
			stop = true;
		}
	}

	return path;
}
