#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> splitArgs(string src) {
  string strBuilder = "";
  vector<string> args;
  for(int i = 0; i < src.size(); i++) {
    if(src.at(i) != ' ' || i != src.size() - 1) {
      strBuilder += src.at(i);
    } else {
      cout << strBuilder << endl;
      args.insert(args.begin(), strBuilder);
      strBuilder = "";
      cout << strBuilder << endl;
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
    cout << args.size() << endl;
    if(args.at(0) == "exit") {
      cout << args.at(0) << " " << args.at(1) << endl;
      exitCode = stoi(args.at(1));
      return exitCode;
    } else {
      cout << input << ": command not found" << endl;
    }
  }
}
