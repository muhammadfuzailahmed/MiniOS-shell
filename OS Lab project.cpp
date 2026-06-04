#include <iostream>
#include<string>
using namespace std;

string arr[] = { "help", "clear", "exit", "date", "time", "list", "create", "delete", "read", "write", "mkdir", "rmdir", "cd", "pwd", "echo", "find", "history", "calc", "sysinfo"
			"version" };
int n = sizeof(arr) / sizeof(arr[0]);

void showHelpCommands() {
	for (int i = 0; i < n; i++) {
		cout << arr[i] << endl;
	}
}

void showVersion() {
	cout << "current version: " << "1.0.0.1" << endl;
}

void showInvalidCommand(string command) {
	cout << "'" << command << "'" << " is not recognized as an internal or external command" << endl;
}


void checkCommand(string command) {
	if (command == "help") {
		showHelpCommands();
		cout << endl;
	}
	else if (command == "version") {
		showVersion();
		cout << endl;
	}
	else {
		showInvalidCommand(command);
		cout << endl;
	}
}

int main()
{
	cout << "====================================================================================================================================" << endl;
    cout << "\t\t\t\t\t\tMiniOS Shell" << endl;
	cout << "====================================================================================================================================" << endl;
	cout << endl;
	cout << "Type 'help' to see available commands" << endl;
	cout << "type 'exit' to exit MiniOS shell" << endl;
	string command;
	do
	{
		cout << "MiniOs> ";
		getline(cin, command);
		if (command == "exit") {
			break;
		}
		else {
			checkCommand(command);
		}
	} while (command != "exit");
}