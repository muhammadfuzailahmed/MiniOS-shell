#include <iostream>
#include<string>
#include <cstdlib>
#include <ctime>
#include <filesystem>
using namespace std;

namespace fs = std::filesystem;

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

void handleCalc() {
	float num1;
	string op;
	float num2;
	float result;

	cout << "Enter first number: ";
	cin >> num1;
	cout << "operation: ";
	cin >> op;
	cout << "Enter second number: ";
	cin >> num2;

	if (op == "+") {
		result = num1 + num2;
		cout << "Result: " << result;
		return;
	} else if (op == "*") {
		result = num1 * num2;
		cout << "Result: " << result;
		return;
	}else if (op == "-") {
		result = num1 - num2;
		cout << "Result: " << result;
		return;
	}else if (op == "/") {
		if (num2 == 0) {
			cout << "cannot divide by zero" << endl;
			return;
		}
		else {
			result = num1 / num2;
			cout << "Result: " << result;
			return;
		}
	}
	else {
		cout << "Invalid inputs" << endl;
		return;
	}
}

void generateRandomNumber() {
	int num = rand() % 100 + 1;
	cout << "Random Number: " << num << endl;
}

void handlemkdir(string command) {
	string folderName = command.substr(6);

	if (folderName == "") {
		cout << "error: folder name is missing" << endl;
	}
	else if (fs::exists(folderName)) {
		cout << "error: folder already exists with same name." << endl;
	}
	else {
		fs::create_directory(folderName);
		cout << "Folder created successfully!" << endl;
	}
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
	else if (command == "calc") {
		handleCalc();
		cin.ignore();
		cout << endl;
	}
	else if (command == "random") {
		generateRandomNumber();
		cout << endl;
	}
	else if (command.substr(0, 6) == "mkdir ") {
		handlemkdir(command);
		cout << endl;
	}
	else {
		showInvalidCommand(command);
		cout << endl;
	}
}

int main()
{
	srand(time(0));
	cout << "====================================================================================================================================" << endl;
    cout << "\t\t\t\t\t\tMiniOS Shell" << endl;
	cout << "====================================================================================================================================" << endl;
	cout << "Type 'help' to see available commands" << endl;
	cout << "type 'exit' to exit MiniOS shell" << endl;
	cout << endl;
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