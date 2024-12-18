#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> splitArgs(string src) {
  string strBuilder = "";
  vector<string> args;
  for(int i = 0; i <= src.size(); i++) {
    if(i == src.size() || src.at(i) == ' ') {
      args.insert(args.begin(), strBuilder);
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

  while(1) {
    cout << "$ ";
    string input;
    getline(cin, input);
    vector<string> args = splitArgs(input);
    if(args.at(0) == "exit") {
      exitCode = stoi(args.at(1));
      cout << exitCode << endl;
      return exitCode;
    } else {
      cout << input << ": command not found" << endl;
    }
  }
}
