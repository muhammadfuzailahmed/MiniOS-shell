#include <iostream>
#include<string>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
using namespace std;

namespace fs = std::filesystem;
string storagePath = "MiniOS_Storage";

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
	string fullPath = storagePath + "/" + folderName;

	if (folderName == "") {
		cout << "error: folder name is missing" << endl;
	}
	else if (fs::exists(fullPath)) {
		cout << "error: folder already exists with same name." << endl;
	}
	else {
		fs::create_directory(fullPath);
		cout << "Folder created successfully!" << endl;
	}
}

void showFolderAndFiles() {
	for (const auto& item : fs::directory_iterator(storagePath)) {
		if (fs::is_directory(item)) {
			cout << "[FOLDER]: " << item.path().filename().string() << endl;
		}
		else {
			cout << "[FILE]: " << item.path().filename().string() << endl;
		}
	}
}

void handleCreateFileCommand(string fileName) {
	string fullPath = storagePath + "/" + fileName;
	if (fileName == "") {
		cout << "error: file name is missing" << endl;
	}
	else if (fs::exists(fullPath)) {
		cout << "error: file already exists with same name." << endl;
	}
	else {
		ofstream file(fullPath);
		file.close();

		cout << "File created successfully!" << endl;
	}
}

bool findFile(string fileName) {
	string fullPath = storagePath + "/" + fileName;
	if (fs::is_directory(fullPath)) {
		cout << "Folder found, any file does not exist with this name" << endl;
	}
	else if (fs::exists(fullPath)) {
		//cout << "File found" << endl;
		return true;
	}
	else {

		//cout << "File not found" << endl;
		return false;
	}

}

//string getFilePath(string fileName) {
//	if(fs::exists(fileName)){
//		return fileName.path
//}

void clearCommand()
{
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif	
}

void about() {
	cout << R"(=================================
MiniOS Shell
Version: 1.0
=================================

MiniOS Shell is a custom command-line interpreter
developed as an Operating Systems project.

Features:
- Command Interpreter
- File Creation and Deletion
- Folder Creation
- File Search
- Directory Listing
- Calculator Utility

Developed using C++ and Visual Studio 2022.
=================================
)" << endl;
}

void showDate() {
	time_t now = time(0);
	tm localTime;

	localtime_s(&localTime, &now);

	cout << "Date: "
		<< localTime.tm_mday << "/"
		<< localTime.tm_mon + 1 << "/"
		<< localTime.tm_year + 1900
		<< endl;
}

void showTime() {
	time_t now = time(0);
	tm localTime;

	localtime_s(&localTime, &now);

	int hour = localTime.tm_hour;
	string period;

	if (hour >= 12)
	{
		period = "PM";
	}
	else
	{
		period = "AM";
	}

	if (hour == 0)
	{
		hour = 12;
	}
	else if (hour > 12)
	{
		hour = hour - 12;
	}

	cout << "Time: "
		<< hour << ":"
		<< setw(2) << setfill('0') << localTime.tm_min
		<< period << endl;
}

void deleteFile(string fileName) {
	string fullPath = storagePath + "/" + fileName;
	if (fileName == "") {
		cout << "error: file name is missing" << endl;
	}

	else if (fs::exists(fullPath)) {
		fs::remove(fullPath);
		cout << "File deleted successfully!" << endl;
	}
	else {
		cout << "File not found!" << endl;
	}
}

void writeDataToFile(string data, string fileName) {
	string fullPath = storagePath + "/" + fileName;
	ofstream file(fullPath);
	file << data << endl;
	file.close();
}

void appendDataToFile(string data, string fileName) {
	string fullPath = storagePath + "/" + fileName;
	ofstream file(fullPath, ios::app);
	file << data << endl;
	file.close();
}

void handleEchoCommand(string echoCommand) {
	if (echoCommand.find(">>") != string::npos) {
		int firstQuote = echoCommand.find('"');
		int secondQuote = echoCommand.find('"', firstQuote + 1);

		string message = echoCommand.substr(firstQuote + 1, secondQuote - firstQuote - 1);
		int index = echoCommand.find(">>");
		string fileName = echoCommand.substr(index + 3);
		if (findFile(fileName)) {
			appendDataToFile(message, fileName);
		}
		else {
			cout << "File not found" << endl;
		}
	}
	else if (echoCommand.find('>') != string::npos) {
		int firstQuote = echoCommand.find('"');
		int secondQuote = echoCommand.find('"', firstQuote + 1);

		string message = echoCommand.substr(firstQuote + 1, secondQuote - firstQuote - 1);
		int index = echoCommand.find('>');
		string fileName = echoCommand.substr(index + 2);
		if (findFile(fileName)) {
			writeDataToFile(message, fileName);
		}
		else {
			cout << "File not found" << endl;
		}
	}
	else if (echoCommand == "") {
		cout << "error: data is missing" << endl;
	}
	else {
		cout << echoCommand << endl;
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
	else if (command == "list") {
		showFolderAndFiles();
		cout << endl;
	}
	else if (command.substr(0, 7) == "create ") {
		string fileName = command.substr(7);
		handleCreateFileCommand(fileName);
		cout << endl;
	}
	else if (command.substr(0, 5) == "find ") {
		string fileName = command.substr(5);
		bool result = findFile(fileName);
		if (result) {
			cout << "File found" << endl;
		}
		else {
			cout << "File not found" << endl;
			
		}
		cout << endl;
	}
	else if (command == "clear")
	{
		clearCommand();
	}
	else if (command == "about") {
		about();
		cout << endl;
	}
	else if (command == "date") {
		showDate();
	}
	else if (command == "time") {
		showTime();
	}
	else if (command.substr(0, 7) == "delete ") {
		string deleteFileName = command.substr(7);
		deleteFile(deleteFileName);
		cout << endl;
	}
	else if (command.substr(0, 5) == "echo ") {
		string echoString = command.substr(5);
		handleEchoCommand(echoString);
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
	fs::create_directory(storagePath);
	cout << "========================================================================================================================" << endl;
	cout << "\t\t\t\t\t\tMiniOS Shell" << endl;
	cout << "========================================================================================================================" << endl;
	cout << "Type 'help' to see available commands" << endl;
	cout << "type 'exit' to exit MiniOS shell" << endl;
	cout << endl;
	string command;
	do
	{
		cout << "MiniOs> ";
		getline(cin, command);
		if (command == "exit") {
			cout << "Exiting MiniOS shell..." << endl;
			break;
		}
		else {
			checkCommand(command);
		}
	} while (command != "exit");
};