#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>

using namespace std;

string path = std::getenv("PATH");

vector<string> validCommands = {"exit", "echo", "type"};
bool isValidCommand(string cmd) {
  for(int i = 0; i < validCommands.size(); i++) {
    if(validCommands.at(i) == cmd) {
      return true;
    }
  }

  return false;
}

vector<string> splitArgs(string src) {
  string strBuilder = "";
  vector<string> args;
  for(int i = 0; i <= src.size(); i++) {
    if(i == src.size() || src.at(i) == ' ') {
      args.insert(args.end(), strBuilder);
      strBuilder = "";
    } else {
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

    /* splits the command into individual argumnets */
    vector<string> args = splitArgs(input);

    /* excutes commands */
    if(args.at(0) == "exit") { 
      exitCode = stoi(args.at(1));
      return exitCode;
    } else if(args.at(0) == "echo") {
      for(int i = 1; i < args.size(); i++) {
        cout << args.at(i) << " ";
      }
      cout << endl;
    } else if(args.at(0) == "type") {
      if(path == "") {
        if(isValidCommand(args.at(1)))
          cout << args.at(1) << " is a shell builtin" << endl;
        else {
          if(searchPath(args.at(1)) == "")
            cout << args.at(1) << ": not found" << endl;
          else 
            cout <<args.at(1) << " is " << searchPath(args.at(1)) << endl;
        }
      }
    } else {
      cout << input << ": command not found" << endl;
    }
  }
}
