#include <iostream>
#include <string>
#include <vector>

using namespace std;

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

int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

  int exitCode;

  for(;;) {
    cout << "$ ";
    string input;
    getline(cin, input);
    vector<string> args = splitArgs(input);
    if(args.at(0) == "exit") {
      exitCode = stoi(args.at(1));
      return exitCode;
    } else if(args.at(0) == "echo") {
      for(int i = 1; i < args.size(); i++) {
        cout << args.at(i) << " ";
      }
      cout << endl;
    } else if(args.at(0) == "type") {
      if(isValidCommand(args.at(1)))
        cout << args.at(1) << " is a shell builtin" << endl;
      else 
        cout << args.at(1) << ": not found" << endl;
    } else {
      cout << input << ": command not found" << endl;
    }
  }
}
