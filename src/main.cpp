#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <sys/wait.h>
#include <unistd.h>


using namespace std;

vector<string> validCommands = {"exit", "echo", "type", "pwd", "cd", "cat", "history"};
bool isValidCommand(string cmd);

vector<string> splitArgs(string src);
string singleQuoteParsing(string src, int* startIndex);
string doubleQuoteParsing(string src, int* startIndex);

vector<string> getDirectories(string p);
string searchPath(string cmd);
void excuteProgram(string path, vector<string> args); 
void excuteProgramWCat(string fPtah);

int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

  int exitCode;

  for(;;) {
    /* User enters command */
    cout << "$ ";
    string input;
    getline(cin, input);

    /* splits the command into individual arguments */
    vector<string> args = splitArgs(input);

    /* excutes commands */
    if(args.at(0) == "exit") { // exit command
      exitCode = stoi(args.at(1));
      return exitCode;
    } else if(args.at(0) == "echo") {
      for(int i = 1; i < args.size(); i++) {
        cout << args.at(i) << " ";
      }
      cout << endl;
    } else if(args.at(0) == "type") { // type command
      if(isValidCommand(args.at(1)))
        cout << args.at(1) << " is a shell builtin" << endl;
      else {
        if(searchPath(args.at(1)) == "")
          cout << args.at(1) << ": not found" << endl;
        else 
          cout <<args.at(1) << " is " << searchPath(args.at(1)) << endl;
      }
    } else if(args.at(0) == "pwd") { // pwd command
      cout << filesystem::current_path().string() << endl;
    } else if(args.at(0) == "cd") { // cd command
      if(args.at(1) != "~") { // Not home directory
        if(filesystem::exists(args.at(1)))
          filesystem::current_path(args.at(1));
        else 
          cout << "cd: " << args.at(1) << ": No such file or directory" <<  endl;
      } else { // Change to home directory
        string path = getenv("HOME");
        filesystem::current_path(path);
      }
    } else if(args.at(0) == "cat") {
      for(int i = 1; i < args.size(); i++) {
        excuteProgramWCat(args.at(i));
      }
    } else {
      if(searchPath(args.at(0)) == "")
        cout << input << ": command not found" << endl;
      else 
        excuteProgram(searchPath(args.at(0)), args);
    }
  }
}

bool isValidCommand(string cmd) {
  for(int i = 0; i < validCommands.size(); i++) {
    if(validCommands.at(i) == cmd) {
      return true;
    }
  }

  return false;
}

string singleQuoteParsing(string src, int* startIndex) {
  string strBuilder = "";

  for(int i = *startIndex + 1; i < src.size(); i++) {
    if(src.at(i) == '\'') {
      *startIndex = i;
      break;
    }

    strBuilder += src.at(i);
  }

  return strBuilder;
}

string doubleQuoteParsing(string src, int* startIndex) {
  string strBuilder = "";

  for(int i = *startIndex + 1; i < src.size(); i++) {
    if(src.at(i) == '"') {
      *startIndex = i;
      break;
    }

    strBuilder += src.at(i);
  }

  return strBuilder;
}

//add individual arguments into one if they are encapsulated with a single/double qoutes and also removing those qoutes from them
vector<string> splitArgs(string src) {
  string strBuilder = "";
  vector<string> args;

  for(int i = 0; i <= src.size(); i++) {
    if(i == src.size() || src.at(i) == ' ') {
      if(!strBuilder.empty()) 
        args.push_back(strBuilder);
      strBuilder = "";
    } else {
      if(src.at(i) == '\'') {
        strBuilder = singleQuoteParsing(src, &i);
      }
      else if(src.at(i) == '"') {
        strBuilder = doubleQuoteParsing(src, &i);
      }
      else 
        strBuilder += src.at(i);
    }

  }

  return args;
}

vector<string> getDirectories(string p) {
  string strBuilder = "";
  vector<string> dirs;
  for(int i = 0; i <= p.size(); i++) {
    if(i == p.size() || p.at(i) == ':') {
      dirs.insert(dirs.end(), strBuilder);
      strBuilder = "";
    } else {
      strBuilder += p.at(i);
    }
  }

  return dirs;
}

string searchPath(string cmd) {
  string path = getenv("PATH");
  vector<string> dirs = getDirectories(path);

  for(int i = 0; i < dirs.size(); i++) {
    string pathBuilder = dirs.at(i);
    pathBuilder += "/";
    pathBuilder += cmd;
    if(filesystem::exists(pathBuilder))
      return pathBuilder;
  }

  return "";
}

void excuteProgram(string path, vector<string> args) {
  pid_t pid = fork();

  if(pid == 0) { //Child process (excutes the excutable)
    vector<char*> c_args;

    for(int i = 0; i < args.size(); i++) {
      c_args.push_back(const_cast<char*>(args.at(i).c_str()));
    } 
    c_args.push_back(nullptr); //acts as terminator to prevent execvp from reading out of bounds of the data

    execvp(path.c_str(), c_args.data());
    perror("execvp");  // If exec fails
    exit(1);
  } else if(pid > 0 ) { //Parent process (excutes the actual program)
    int status;
    waitpid(pid, &status, 0); // waits until the child process has excuted the file and exits
  } 
}

void excuteProgramWCat(string fPath) {
    pid_t pid = fork();

  if(pid == 0) { //Child process (excutes the excutable)

    cout << filesystem::exists(fPath) <<  endl;
    cout << fPath.c_str() << endl;
    const char* args[] = {fPath.c_str(), nullptr};
    execvp(fPath.c_str(), const_cast<char* const*>(args));
    perror("execvp");  // If exec fails
    exit(1);
  } else if(pid > 0 ) { //Parent process (excutes the actual program)
    int status;
    waitpid(pid, &status, 0); // waits until the child process has excuted the file and exits
  } 
}
