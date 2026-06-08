#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

namespace fs = std::filesystem;
string storagePath = "MiniOS_Storage";

struct HistoryNode
{
    string cmd;
    HistoryNode* next;
};

HistoryNode* historyHead = NULL;
HistoryNode* historyTail = NULL;

string arr[] = { "help", "clear", "exit", "date", "time", "list", "create", "delete", "read", "cat", "mkdir", "rmdir", "where", "pwd", "echo", "find", "history", "calc", "sysinfo",
            "version" };
int n = sizeof(arr) / sizeof(arr[0]);

namespace ui
{
    const string RESET = "\033[0m";
    const string BOLD = "\033[1m";
    const string DIM = "\033[2m";
    const string GREEN = "\033[32m";
    const string BRIGHT_GREEN = "\033[92m";
    const string DIM_GREEN = "\033[2;32m";
    const string CYAN = "\033[36m";
    const string YELLOW = "\033[33m";
    const string RED = "\033[31m";
    const string BG_DARK = "\033[48;2;2;15;10m";
    const string BG_PANEL = "\033[48;2;4;24;15m";
    const string NEON = "\033[38;2;0;255;120m";
    const string SOFT = "\033[38;2;70;160;105m";
    const string FAINT = "\033[38;2;35;85;58m";
    const int WIDTH = 118;

    int historyCount()
    {
        int count = 0;
        HistoryNode* current = historyHead;
        while (current != NULL)
        {
            count++;
            current = current->next;
        }
        return count;
    }

    string repeat(string text, int count)
    {
        string s;
        for (int i = 0; i < count; i++) s += text;
        return s;
    }

    string padRight(string text, int width)
    {
        if ((int)text.length() >= width) return text.substr(0, width);
        return text + repeat(" ", width - (int)text.length());
    }

    void enableANSI()
    {
#ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        SetConsoleTitleA("MiniOS Shell - Modern Terminal UI");

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE)
        {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode))
            {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
#endif
    }

    void setupWindow()
    {
#ifdef _WIN32
        system("mode con: cols=120 lines=40 > nul");
#endif
    }

    void clearScreen()
    {
        cout << "\033[2J\033[H";
    }

    void topChrome()
    {
        cout << BG_DARK << FAINT << "┌" << repeat("─", WIDTH - 2) << "┐" << RESET << endl;

        string left = "  ●  ●  ●";
        string title = "MINIOS SHELL  -  v1.0.0.1";
        string live = "[ + LIVE ]";
        int middleSpace = WIDTH - 2 - (int)left.length() - (int)title.length() - (int)live.length();
        if (middleSpace < 1) middleSpace = 1;

        cout << BG_DARK << FAINT << "│" << RESET
            << BG_DARK << "\033[38;2;255;95;86m" << "  ●" << RESET
            << BG_DARK << "\033[38;2;255;189;46m" << "  ●" << RESET
            << BG_DARK << "\033[38;2;39;201;63m" << "  ●" << RESET
            << BG_DARK << repeat(" ", middleSpace / 2)
            << FAINT << title
            << repeat(" ", middleSpace - middleSpace / 2)
            << "\033[38;2;0;210;95m" << live << FAINT
            << " │" << RESET << endl;

        cout << BG_DARK << FAINT << "├" << repeat("─", WIDTH - 2) << "┤" << RESET << endl;
    }

    void bottomBar()
    {
        string left = "  ● ONLINE   |   OS Project";
        string right = "HIST: " + to_string(historyCount()) + "  ";

        int spaces1 = (WIDTH - 2 - (int)left.length() - (int)right.length()) / 2;
        if (spaces1 < 1) spaces1 = 1;
        int spaces2 = WIDTH - 2 - (int)left.length() - (int)right.length() - spaces1;
        if (spaces2 < 1) spaces2 = 1;

        cout << BG_DARK << FAINT << "├" << repeat("─", WIDTH - 2) << "┤" << RESET << endl;
        cout << BG_DARK << FAINT << "│" << RESET
            << BG_DARK << SOFT << left
            << FAINT << repeat(" ", spaces1)
            << repeat(" ", spaces2)
            << SOFT << right
            << FAINT << "│" << RESET << endl;
        cout << BG_DARK << FAINT << "└" << repeat("─", WIDTH - 2) << "┘" << RESET << endl;
    }

    void statusOK(string left, string right)
    {
        cout << BG_DARK << "  " << FAINT << "[ " << NEON << "OK" << FAINT << " ] "
            << SOFT << padRight(left, 32) << FAINT;
        for (int i = 0; i < 26; i++) cout << ".";
        cout << " " << NEON << right << RESET << endl;
    }

    void bootScreen()
    {
        setupWindow();
        clearScreen();
        topChrome();

        cout << BG_DARK << FAINT << "│" << repeat(" ", WIDTH - 2) << "│" << RESET << endl;
        cout << BG_DARK << FAINT << "│" << repeat(" ", 12)
            << "┌" << repeat("─", 62) << "┐"
            << repeat(" ", WIDTH - 2 - 12 - 64) << "│" << RESET << endl;

        cout << BG_DARK << FAINT << "│" << repeat(" ", 12) << "│ "
            << NEON << BOLD << "M I N I O S S H E L L  -  v 1 . 0 . 0 . 1"
            << FAINT << repeat(" ", 14) << "│"
            << repeat(" ", WIDTH - 2 - 12 - 64) << "│" << RESET << endl;

        cout << BG_DARK << FAINT << "│" << repeat(" ", 12)
            << "└" << repeat("─", 62) << "┘"
            << repeat(" ", WIDTH - 2 - 12 - 64) << "│" << RESET << endl;
        cout << BG_DARK << FAINT << "│" << repeat(" ", WIDTH - 2) << "│" << RESET << endl;

        statusOK("Initializing storage layer", storagePath);
        statusOK("Linked-list history module", "loaded");
        statusOK("Command parser", "ready");

        cout << BG_DARK << FAINT << "│" << repeat(" ", WIDTH - 2) << "│" << RESET << endl;
        cout << BG_DARK << FAINT << "│  " << SOFT << "Type " << NEON << "'help'" << SOFT
            << " to see commands   |   " << NEON << "'exit'" << SOFT << " to quit"
            << repeat(" ", WIDTH - 2 - 2 - 52) << FAINT << "│" << RESET << endl;
        cout << BG_DARK << FAINT << "│" << repeat(" ", WIDTH - 2) << "│" << RESET << endl;
        bottomBar();
        cout << endl;
    }

    void prompt()
    {
        cout << RESET << endl;

#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;

        if (GetConsoleScreenBufferInfo(hOut, &csbi))
        {
            csbi.dwCursorPosition.X = 0;
            SetConsoleCursorPosition(hOut, csbi.dwCursorPosition);
        }
#endif

        cout << NEON << "MiniOS"
            << SOFT << "@Shell"
            << FAINT << ":"
            << CYAN << storagePath
            << NEON << " > "
            << RESET << flush;
    }

    void sectionTitle(string title)
    {
        cout << FAINT << "┌─[ " << NEON << BOLD << title << RESET << FAINT << " ]" << RESET << endl;
        cout << FAINT << "└" << repeat("─", 72) << RESET << endl;
    }

    void success(string msg)
    {
        cout << FAINT << "[ " << NEON << "OK" << FAINT << " ] " << RESET << msg << endl;
    }

    void error(string msg)
    {
        cout << FAINT << "[ " << RED << "ERR" << FAINT << " ] " << RESET << msg << endl;
    }

    void info(string msg)
    {
        cout << FAINT << "[ " << CYAN << "INFO" << FAINT << " ] " << RESET << msg << endl;
    }
}

tm getLocalTimeNow()
{
    time_t now = time(0);
    tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif

    return localTime;
}

void showHelpCommands() {
    ui::sectionTitle("Available Commands");
    for (int i = 0; i < n; i++) {
        cout << ui::BRIGHT_GREEN << "  > " << ui::RESET << arr[i] << endl;
    }
}

void showVersion() {
    cout << ui::BRIGHT_GREEN << "current version: " << ui::RESET << "1.0.0.1" << endl;
}

void handleCalc() {
    float num1;
    string op;
    float num2;
    float result;

    cout << ui::DIM_GREEN << "Enter first number: " << ui::RESET;
    cin >> num1;
    cout << ui::DIM_GREEN << "operation: " << ui::RESET;
    cin >> op;
    cout << ui::DIM_GREEN << "Enter second number: " << ui::RESET;
    cin >> num2;

    if (op == "+") {
        result = num1 + num2;
        cout << ui::BRIGHT_GREEN << "Result: " << ui::RESET << result;
        return;
    }
    else if (op == "*") {
        result = num1 * num2;
        cout << ui::BRIGHT_GREEN << "Result: " << ui::RESET << result;
        return;
    }
    else if (op == "-") {
        result = num1 - num2;
        cout << ui::BRIGHT_GREEN << "Result: " << ui::RESET << result;
        return;
    }
    else if (op == "/") {
        if (num2 == 0) {
            ui::error("cannot divide by zero");
            return;
        }
        else {
            result = num1 / num2;
            cout << ui::BRIGHT_GREEN << "Result: " << ui::RESET << result;
            return;
        }
    }
    else {
        ui::error("Invalid inputs");
        return;
    }
}

void generateRandomNumber() {
    int num = rand() % 100 + 1;
    cout << ui::BRIGHT_GREEN << "Random Number: " << ui::RESET << num << endl;
}

void handlemkdir(string command) {
    string folderName = command.substr(6);
    string fullPath = storagePath + "/" + folderName;

    if (folderName == "") {
        ui::error("folder name is missing");
    }
    else if (fs::exists(fullPath)) {
        ui::error("folder already exists with same name.");
    }
    else {
        fs::create_directory(fullPath);
        ui::success("Folder created successfully!");
    }
}

void showFolderAndFiles() {
    ui::sectionTitle("Storage Items");
    bool empty = true;

    for (const auto& item : fs::directory_iterator(storagePath)) {
        empty = false;
        if (fs::is_directory(item)) {
            cout << ui::CYAN << "[FOLDER]: " << ui::RESET << item.path().filename().string() << endl;
        }
        else {
            cout << ui::BRIGHT_GREEN << "[FILE]: " << ui::RESET << item.path().filename().string() << endl;
        }
    }

    if (empty) {
        cout << ui::DIM_GREEN << "  Storage folder is empty." << ui::RESET << endl;
    }
}

void handleCreateFileCommand(string fileName) {
    string fullPath = storagePath + "/" + fileName;
    if (fileName == "") {
        ui::error("file name is missing");
    }
    else if (fs::exists(fullPath)) {
        ui::error("file already exists with same name.");
    }
    else {
        ofstream file(fullPath);
        file.close();

        ui::success("File created successfully!");
    }
}

bool findFile(string fileName) {
    string fullPath = storagePath + "/" + fileName;
    if (fs::is_directory(fullPath)) {
        ui::info("Folder found, any file does not exist with this name");
    }
    else if (fs::exists(fullPath)) {
        return true;
    }
    else {
        return false;
    }

    return false;
}

void clearCommand()
{
    ui::bootScreen();
}

void about() {
    cout << ui::BRIGHT_GREEN << R"(=================================
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

Developed using C++.
=================================
)" << ui::RESET << endl;
}

void showDate() {
    tm localTime = getLocalTimeNow();

    cout << ui::BRIGHT_GREEN << "Date: " << ui::RESET
        << localTime.tm_mday << "/"
        << localTime.tm_mon + 1 << "/"
        << localTime.tm_year + 1900
        << endl;
}

void showTime() {
    tm localTime = getLocalTimeNow();

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

    cout << ui::BRIGHT_GREEN << "Time: " << ui::RESET
        << hour << ":"
        << setw(2) << setfill('0') << localTime.tm_min
        << period << setfill(' ') << endl;
}

void deleteFile(string fileName) {
    string fullPath = storagePath + "/" + fileName;
    if (fileName == "") {
        ui::error("file name is missing");
    }

    else if (fs::exists(fullPath)) {
        fs::remove(fullPath);
        ui::success("File deleted successfully!");
    }
    else {
        ui::error("File not found!");
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
            ui::success("Data appended successfully.");
        }
        else {
            ui::error("File not found");
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
            ui::success("Data written successfully.");
        }
        else {
            ui::error("File not found");
        }
    }
    else if (echoCommand == "") {
        ui::error("data is missing");
    }
    else {
        cout << echoCommand << endl;
    }
}

void showFileData(string fileName) {
    string fullPath = storagePath + "/" + fileName;
    if (fs::exists(fullPath)) {
        ui::sectionTitle("File Content: " + fileName);
        ifstream file(fullPath);
        string line;

        while (getline(file, line))
        {
            cout << line << endl;
        }

        file.close();
    }
    else {
        ui::error("File not found");
    }
}

void deleteDirectory(string folderName) {
    string fullPath = storagePath + "/" + folderName;
    if (folderName == "") {
        ui::error("file name is missing");
    }
    if (fs::is_directory(fullPath)) {
        fs::remove(fullPath);
        ui::success("folder deleted successfully!");
    }
    else {
        ui::error("folder not found");
    }
}

void showSysInfo()
{
    ui::sectionTitle("System Information");
    cout << ui::BRIGHT_GREEN << "Shell Name      : " << ui::RESET << "MiniOS Shell" << endl;
    cout << ui::BRIGHT_GREEN << "Version         : " << ui::RESET << "1.0" << endl;
    cout << ui::BRIGHT_GREEN << "Project Type    : " << ui::RESET << "Operating Systems Mini Project" << endl;
    cout << ui::BRIGHT_GREEN << "Language        : " << ui::RESET << "C++" << endl;
    cout << ui::BRIGHT_GREEN << "UI Type         : " << ui::RESET << "Pure C++ ANSI Terminal UI" << endl;

#ifdef _WIN32
    cout << ui::BRIGHT_GREEN << "Platform        : " << ui::RESET << "Windows" << endl;
#else
    cout << ui::BRIGHT_GREEN << "Platform        : " << ui::RESET << "Linux/Mac" << endl;
#endif

    cout << ui::BRIGHT_GREEN << "Storage Folder  : " << ui::RESET << storagePath << endl;
}

void handleHistory(string user_command)
{
    if (user_command == "")
    {
        return;
    }

    HistoryNode* new_cmd = new HistoryNode();
    new_cmd->cmd = user_command;
    new_cmd->next = NULL;

    if (historyHead == NULL)
    {
        historyHead = new_cmd;
        historyTail = new_cmd;
    }

    else
    {
        historyTail->next = new_cmd;
        historyTail = new_cmd;
    }
}

void printHistory() {
    ui::sectionTitle("Command History");
    if (historyHead == NULL)
    {
        cout << ui::DIM_GREEN << "History is empty." << ui::RESET << endl;
    }

    else if (historyHead == historyTail && historyHead->cmd == "history")
    {
        cout << ui::BRIGHT_GREEN << "1 " << ui::RESET << historyHead->cmd << endl;
    }
    else
    {
        HistoryNode* current = historyHead;
        int counter = 1;
        while (current != NULL)
        {
            cout << ui::BRIGHT_GREEN << counter << " " << ui::RESET << current->cmd << endl;
            current = current->next;
            counter++;
        }
    }
}

void showCurrentDirectory()
{
    cout << ui::BRIGHT_GREEN << "Current Directory: " << ui::RESET << storagePath << endl;
}

void pwd() {
    cout << storagePath << endl;
}

void whereFile(string whereString) {
    string fullpath = storagePath + "/" + whereString;
    if (fs::exists(fullpath)) {
        cout << storagePath << endl;
    }
    else {
        ui::error("file not found!");

    }
}

void showInvalidCommand(string command) {
    cout << ui::RED << "'" << command << "'" << ui::RESET << " is not recognized as an internal or external command" << endl;
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
            ui::success("File found");
        }
        else {
            ui::error("File not found");

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
        cout << endl;
    }
    else if (command == "time") {
        showTime();
        cout << endl;
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
    else if (command.substr(0, 4) == "cat ") {
        string fileName = command.substr(4);
        showFileData(fileName);
        cout << endl;
    }
    else if (command.substr(0, 6) == "rmdir ") {
        string folderName = command.substr(6);
        deleteDirectory(folderName);
        cout << endl;
    }
    else if (command == "sysinfo") {
        showSysInfo();
        cout << endl;
    }
    else if (command == "history")
    {
        printHistory();
        cout << endl;
    }
    else if (command == "pwd") {
        showCurrentDirectory();
        cout << endl;
    }
    else if (command.substr(0, 6) == "where ") {
        string whereString = command.substr(6);
        whereFile(whereString);
        cout << endl;
    }
    else if (command == "") {
        cout << endl;
    }
    else {
        showInvalidCommand(command);
        cout << endl;
    }
}

int main()
{
    ui::enableANSI();
    srand(time(0));
    fs::create_directory(storagePath);

    ui::bootScreen();

    string command;
    do
    {
        ui::prompt();
        getline(cin, command);
        handleHistory(command);
        if (command == "exit") {
            cout << ui::DIM_GREEN << "Exiting MiniOS shell..." << ui::RESET << endl;
            break;
        }
        else {
            checkCommand(command);
        }
    } while (command != "exit");
};


