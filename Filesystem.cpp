#include "Filesystem.h"
#include "Link.h"

Filesystem::Filesystem() {
	root = new Directory("/", 0, nullptr, 0, getUser(), getCurrentDateTime());
	act_file_id = 0;
	id = 1;

	ls_parameters = { "-l", "-t", "--tree" };
}

Filesystem::~Filesystem() {
	delete root;
}

void Filesystem::mkdir(std::string name) {
	std::vector<std::string> all_input = getSplittedInput(' ', name);

	for (const auto& act_input : all_input) {
		std::vector<std::string> input = getSplittedInput('/', act_input);

		int prev_file_id = act_file_id;
		for (const auto& search_file : input) {
			for (const auto& file : static_cast<Directory*>(getFile(act_file_id))->getFiles()) {
				if (file->getName() == search_file) {
					act_file_id = file->getId();
				}
			}
		}

		Directory* dir = new Directory(input.back(), id, static_cast<Directory*>(getFile(act_file_id)), 0, getUser(), getCurrentDateTime());
		static_cast<Directory*>(getFile(act_file_id))->addFile(dir);

		act_file_id = prev_file_id;

		id++;
	}
}

void Filesystem::ls(std::string name) {
	if (name == "") {
		for (const auto& file : static_cast<Directory*>(getFile(act_file_id))->getFiles()) {

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
		}
	}
	else {
		std::vector<std::string> parameters = getSplittedInput(' ', name);
		if (parameters.size() == 1) {
			if (parameters[0] == "-t" || parameters[0] == "--tree") {
				listTree();
			}
			else {
				std::vector<std::string> path = getSplittedInput('/', parameters[0]);
				int prev_act_id = act_file_id;
				for (const auto& dir : path) {
					if (parameters[0] == "-l") {
						cd(dir, "ls", true);
					}
					else {
						cd(dir, "ls");
					}
				}
				static_cast<Directory*>(getFile(act_file_id))->ls(false, parameters[0]);
				act_file_id = prev_act_id;
			}
		}
	}
}

void Filesystem::listTree() {
	SetConsoleTextAttribute(coloring, cyan);
	std::cout << getFile(act_file_id)->getName() << '\n';

	for (const auto& file : static_cast<Directory*>(getFile(act_file_id))->getFiles()) {
		std::cout << '\t';

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

		static_cast<Directory*>(file)->ls(true, "");
	}
}

void Filesystem::setActDirectoryId(int number) {
	act_file_id = number;
}

void Filesystem::touch(std::string name, std::string content) {
	std::vector<std::string> input = getSplittedInput('/', name);

	File* file = new File(input.back(), id, getFile(act_file_id), 0, getUser(), getCurrentDateTime(), content);

	int prev_file_id = act_file_id;
	for (const auto& search_file : input) {
		for (const auto& file : static_cast<Directory*>(getFile(act_file_id))->getFiles()) {
			if (file->getName() == search_file) {
				act_file_id = file->getId();
			}
		}
	}

	file->setSize(content.length());
	static_cast<Directory*>(getFile(act_file_id))->addFile(file);

	act_file_id = prev_file_id;

	id++;
}

void Filesystem::cat(std::string name) {
	std::vector<std::string> path = getSplittedInput('/', name);
	int prev_act_id = act_file_id;

	for (unsigned int i = 0; i < path.size() - 1; i++) {
		cd(path[i], "cat");
	}

	bool valid = false;
	for (const auto& file : static_cast<Directory*>(getFile(act_file_id))->getFiles()) {
		if (file->getName() == path.back()) {
			valid = true;
			if (dynamic_cast<Link*>(file)) {
				std::cout << file->getName() << ": " << file->getContent() << '\n';

				std::vector<std::string> path = getSplittedInput('/', file->getContent());
				int prev_act_id = act_file_id;

				act_file_id = 0;
				for (unsigned int i = 0; i < path.size() - 1; i++) {
					cd(path[i], "cat");
				}

				for (const auto& act_file : static_cast<Directory*>(getFile(act_file_id))->getFiles()) {
					if (act_file->getName() == path.back()) {
						std::cout << act_file->getName() << ": " << act_file->getContent() << '\n';
					}
				}
			}
			else if (!dynamic_cast<Directory*>(file)) {
				std::cout << file->getContent() << '\n';
			}
		}
	}
	act_file_id = prev_act_id;

	if (!valid) {
		std::cout << "cat: " << name << ": No such file or directory\n";
	}
}

void Filesystem::pwd() {
	if (act_file_id == 0) {
		std::cout << '/';
	}
	else {
		std::string path = "";
		for (const auto& file : root->searchLevel(act_file_id)) {
			path = '/' + file->getName() + path;
		}
		std::cout << path;
	}
	std::cout << '\n';
}

void Filesystem::ln(std::string name, std::string destination) {
	std::vector<std::string> path = getSplittedInput('/', name);
	int prev_prev_act_id = act_file_id;
	for (unsigned int i = 0; i < path.size() - 1; i++) {
		cd(path[i], "ln");
	}

	Link* link = new Link(path.back(), id, static_cast<Directory*>(getFile(act_file_id)), destination.length(), getUser(), getCurrentDateTime(), destination);
	static_cast<Directory*>(getFile(act_file_id))->addFile(link);
	id++;

	act_file_id = prev_prev_act_id;
}

int Filesystem::id = 0;

int Filesystem::rm(std::string name) {
	std::vector<File*> files = static_cast<Directory*>(getFile(act_file_id))->getFiles();
	for (unsigned int i = 0; i < files.size(); i++) {
		if (name == files[i]->getName()) {
			static_cast<Directory*>(getFile(act_file_id))->removeElement(i);

			return 0;
		}
	}

	return 1;
}

int Filesystem::cd(std::string name, std::string from, bool ignore_warning) {
	std::vector<std::string> input = getSplittedInput('/', name);

	if (input.size() == 1) {
		if (input[0] == "") {
			int diff = root->searchLevel(act_file_id).size();
			act_file_id = 0;

			return 0;
		}
		else if (input[0] == "..") {
			act_file_id = getFile(act_file_id)->getParent()->getId();

			return 0;
		}
		else {
			for (const auto& file : static_cast<Directory*>(getFile(act_file_id))->getFiles()) {
				if (file->getName() == input[0]) {
					if (dynamic_cast<Directory*>(file)) {
						act_file_id = file->getId();
					}
					else if (dynamic_cast<Link*>(file)) {
						cd(file->getContent(), "cd");
					}

					return 0;
				}
			}
		}
	}
	else if (input.size() > 1) {
		bool file_exists = false;
		int prev_file_id = act_file_id;
		for (const auto& search_file : input) {
			file_exists = false;
			for (const auto& file : static_cast<Directory*>(getFile(act_file_id))->getFiles()) {
				if (file->getName() == search_file) {
					if (dynamic_cast<Directory*>(file)) {
						act_file_id = file->getId();
						file_exists = true;
					}
					else if (dynamic_cast<Link*>(file)) {
						cd(file->getContent(), "cd");
					}
				}
			}
		}
		if (file_exists) {
			return 0;
		}
		else {
			act_file_id = prev_file_id;
		}
	}

	if (!ignore_warning) {
		std::cout << from << ": " << input.back() << ": No such file or directroy\n";
	}

	return 1;
}

int Filesystem::getActDirectoryId() const {
	return act_file_id;
}

std::vector<std::string> Filesystem::getSplittedInput(char character, std::string name) {
	std::vector<std::string> input;

	std::string word = "";
	for (unsigned int i = 0; i < name.length(); i++) {
		if (name[i] == character) {
			input.push_back(word);
			word = "";
		}
		else {
			word += name[i];
		}
	}
	input.push_back(word);

	return input;
}

File* Filesystem::getFile(int id) {
	if (id == 0) {
		return root;
	}
	else {
		return root->getFile(id);
	}
}

Directory* Filesystem::getRoot() const {
	return root;
}

bool Filesystem::isParameterValid(std::string parameter, std::vector<std::string> parameters) {
	for (const auto& param : parameters) {
		if (param == parameter) {
			return true;
		}
	}

	return false;
}

std::string Filesystem::getUser() const {
	return user;
}

void Filesystem::setUser(std::string name) {
	user = name;
}

std::string Filesystem::getCurrentDateTime() const {
#pragma warning(disable:4996);
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}
