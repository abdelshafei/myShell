#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
    if(input == "exit 0") {
      cout << args.at(1) << endl;
      return 0;
    }
    if(args.at(0) == "exit") {
      exitCode = stoi(args.at(1));
      cout << args.size() <<  endl;
      cout << args.at(0) << endl;
      return exitCode;
    } else {
      cout << input << ": command not found" << endl;
    }
  }
}
