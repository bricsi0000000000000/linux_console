#ifndef CONSOLE_H
#define CONSOLE_H


#include "Filesystem.h"

class Console {
public:
	Console(Filesystem* filesystem, Directory* root);

	void startConsole();
	void nano(std::string name);
	void help(std::string command);

	bool isCommandInCommands(std::string input) const;

	std::string getUser() const;
	std::string actFolder();
private:
	Filesystem* filesystem;
	Directory* root;

	const std::string EXIT_NANO_COMMAND = ":exit";
	const std::string EXIT_CONSOLE_COMMAND = ":end";
	std::string cd_help_txt;
	std::string mkdir_help_txt;
	std::string ls_help_txt;
	std::string nano_help_txt;
	std::string cat_help_txt;
	std::string pwd_help_txt;
	std::string rm_help_txt;
	std::string ln_help_txt;
	std::string help_help_txt;
	std::string clear_help_txt;
	std::string computer;

	std::vector<std::string> commands;

	HANDLE coloring = GetStdHandle(STD_OUTPUT_HANDLE);

	int cyan = 11;
	int pink = 13;
	int yellow = 14;
	int white = 15;
};

#endif // CONSOLE_H
