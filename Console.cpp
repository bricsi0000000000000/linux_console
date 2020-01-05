#include <sstream>

#include "Console.h"

Console::Console(Filesystem* filesystem, Directory* root)
	:filesystem(filesystem), root(root)
{
	filesystem->setUser("root");
	computer = "debian";
	commands = { "cd", "mkdir", "ls", "pwd", "rm", "cat", "nano", "ln", "help", ":end", "clear" };

	cd_help_txt = "Usage: cd [DIR]\n";
	cd_help_txt += "Change the shell working directory.\n";
	cd_help_txt += "Change the current directory to DIR.\n";
	cd_help_txt += "The default DIR is the root directory.";

	mkdir_help_txt = "Usage: mkdir DIRECTORY...\n";
	mkdir_help_txt += "Create the DIRECTORY(ies), if they do not already exists.";

	ls_help_txt = "Usage: ls [OPTION] DIRECTORY...\n";
	ls_help_txt += "Create the DIRECTORY(ies), if they do not already exists.\n";
	ls_help_txt += "\t-t, --tree\tlist elements in a tree form.\n";
	ls_help_txt += "\t-l,\t\tuse a long listing format.";

	nano_help_txt = "Usage: nano [FILE]\n";
	nano_help_txt += "Text editor.";

	cat_help_txt = "Usage: cat [FILE]\n";
	cat_help_txt += "Show the content of FILE.";

	pwd_help_txt = "Usage: pwd\n";
	pwd_help_txt += "Print the name of the current working directory.";

	rm_help_txt = "Usage: rm [FILE]...\n";
	rm_help_txt += "Remove the FILES(s).";

	ln_help_txt = "Usage: ln LINK_NAME TARGET\n";
	ln_help_txt += "Create a shortcut to TARGET.";

	help_help_txt = "Usage: help\n";
	help_help_txt += "Display all commands.";

	clear_help_txt = "Usage: clear\n";
	clear_help_txt += "Clear the screen.";

	filesystem->mkdir("bin");
	filesystem->mkdir("boot");
	filesystem->mkdir("cdrom");
	filesystem->mkdir("dev");
	filesystem->mkdir("etc");
	filesystem->mkdir("home");
	filesystem->cd("home", "");
	filesystem->ln("info", "usr/debian/info");
	filesystem->cd("", "");
	filesystem->mkdir("lib");
	filesystem->mkdir("lost+found");
	filesystem->mkdir("media");
	filesystem->mkdir("mnt");
	filesystem->mkdir("opt");
	filesystem->mkdir("proc");
	filesystem->mkdir("root");
	filesystem->mkdir("run");
	filesystem->mkdir("sbin");
	filesystem->mkdir("selinux");
	filesystem->mkdir("srv");
	filesystem->mkdir("sys");
	filesystem->mkdir("tmp");
	filesystem->mkdir("usr");
	filesystem->cd("usr","");
	filesystem->mkdir("debian");
	filesystem->cd("debian", "");
	filesystem->touch("info", "Made by Bolya Richard\nrichard.bolya.eu");
	filesystem->cd("", "");

	filesystem->mkdir("var");

	startConsole();
}

void Console::startConsole() {
	std::string command_input = "";
	while (command_input != EXIT_CONSOLE_COMMAND) {

		SetConsoleTextAttribute(coloring, yellow);

		std::cout << getUser();

		//SetConsoleTextAttribute(coloring, cyan);

		std::cout << actFolder();

		SetConsoleTextAttribute(coloring, white);

		std::cout << "# ";


		command_input = "";
		std::getline(std::cin, command_input);

		std::vector<std::string> input;

		std::string act_command = "";
		for (unsigned int i = 0; i < command_input.length(); i++) {
			if (command_input[i] == ' ') {
				input.push_back(act_command);
				act_command = "";
			}
			else {
				act_command += command_input[i];
			}
		}
		input.push_back(act_command);

		bool isParameterHelp = false;

		if (isCommandInCommands(input[0])) {
			if (input.size() > 1) {
				if (input[1] == "--help" || input[1] == "-h") {
					help(input[0]);

					isParameterHelp = true;
				}
			}
			if (!isParameterHelp) {
				if (input[0] == "cd") {
					if (input.size() == 1) {
						filesystem->cd("", "cd");
					}
					else if (input.size() == 2) {
						filesystem->cd(input[1], "cd");
					}
					else {
						std::cout << "cd: too many arguments\n";
					}
				}
				else if (input[0] == "mkdir") {
					if (input.size() == 1) {
						std::cout << "mkdir: missing operand\n";
						std::cout << "Try 'mkdir --help' for more information.\n";
					}
					else {
						std::string mkdir_input = "";
						for (unsigned int i = 1; i < input.size(); i++) {
							mkdir_input += input[i] + ' ';
						}
						mkdir_input = mkdir_input.substr(0, mkdir_input.size() - 1);
						filesystem->mkdir(mkdir_input);
					}
				}
				else if (input[0] == "ls") {
					std::string path = "";
					for (unsigned int i = 1; i < input.size(); i++) {
						path += input[i];
						if (i + 1 < input.size()) {
							path += ' ';
						}
					}
					filesystem->ls(path);
				}
				else if (input[0] == "nano") {
					if (input.size() == 1)
					{
						std::cout << "nano: missing operand\n";
						std::cout << "Try 'nano --help' for more information.\n";
					}
					else if (input.size() == 2) {
						nano(input[1]);
					}
					else {
						std::cout << "nano: too many arguments\n";
					}
				}
				else if (input[0] == "cat") {
					if (input.size() == 1)
					{
						std::cout << "cat: missing operand\n";
						std::cout << "Try 'cat --help' for more information.\n";
					}
					else if (input.size() == 2) {
						filesystem->cat(input[1]);
					}
					else {
						std::cout << "cat: too many arguments\n";
					}
				}
				else if (input[0] == "pwd") {
					filesystem->pwd();
				}
				else if (input[0] == "rm") {
					if (input.size() == 1) {
						std::cout << "rm: missing operand\n";
						std::cout << "Try 'rm --help' for more information.\n";
					}
					else if (input.size() == 2) {
						filesystem->rm(input[1]);
					}
					else {
						for (unsigned int i = 1; i < input.size(); i++) {
							if (filesystem->rm(input[i]) == 1) {
								std::cout << "rm: cannot remove '" << input[i] << "': No such file or directory\n";
							}
						}
					}
				}
				else if (input[0] == "ln") {
					if (input.size() < 3) {
						std::cout << "ln: missing file operand\n";
						std::cout << "Try 'ln --help' for more information.\n";
					}
					else if (input.size() == 3)
					{
						filesystem->ln(input[1], input[2]);
					}
					else {
						std::cout << "ln: too many arguments\n";
					}
				}
				else if (input[0] == "help") {
					std::cout << "commands: ";
					for (const auto& command : commands) {
						std::cout << command << ", ";
					}

					std::cout << "exit from console: ':end'";

					std::cout << '\n';
				}
				else if (input[0] == "clear") {
					system("CLS");
				}
			}
		}
		else {
			if (input.size() > 0 && input[0] != "") {
				std::cout << "Command '" << input[0] << "' not found\n";
			}
		}
	}
}

void Console::nano(std::string name) {
	if (name == "") {
		std::cout << "nano: name is empty\n";
	}
	else {
		std::vector<std::string> input = filesystem->getSplittedInput('/', name);
		std::cout << "nano\t" << input.back() << '\n';

		std::string exit_string = "";
		std::string rows = "";

		while (exit_string == "") {
			std::string input = "";
			std::getline(std::cin, input);

			if (input == EXIT_NANO_COMMAND) {
				exit_string = input;
			}
			else if (input.length() > 0) {
				rows += input + '\n';
			}
		}

		if (rows != "") {
			if (rows.back() == '\n') {
				rows = rows.substr(0, rows.length() - 1);
			}
		}

		filesystem->touch(name, rows);
	}
}

bool Console::isCommandInCommands(std::string input) const {
	for (const auto& command : commands) {
		if (input == command) {
			return true;
		}
	}

	return false;
}

std::string Console::getUser() const {
	return filesystem->getUser() + '@' + computer + ':';
}

std::string Console::actFolder() {
	std::string path = "";
	if (filesystem->getActDirectoryId() == 0) {
		path = '/';
	}
	else {
		for (const auto& file : root->searchLevel(filesystem->getActDirectoryId())) {
			path = '/' + file->getName() + path;
		}
	}

	return path;
}

void Console::help(std::string command) {
	if (command == "cd") {
		std::cout << cd_help_txt << '\n';
	}
	else if (command == "mkdir") {
		std::cout << mkdir_help_txt << '\n';
	}
	else if (command == "ls") {
		std::cout << ls_help_txt << '\n';
	}
	else if (command == "nano") {
		std::cout << nano_help_txt << '\n';
	}
	else if (command == "cat") {
		std::cout << cat_help_txt << '\n';
	}
	else if (command == "pwd") {
		std::cout << pwd_help_txt << '\n';
	}
	else if (command == "rm") {
		std::cout << rm_help_txt << '\n';
	}
	else if (command == "ln") {
		std::cout << ln_help_txt << '\n';
	}
	else if (command == "help") {
		std::cout << help_help_txt << '\n';
	}
	else if (command == "clear") {
		std::cout << clear_help_txt << '\n';
	}
}
