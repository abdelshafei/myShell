#include <iostream>

using namespace std;

int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

  while(1) {
    cout << "$ ";
    string input;
    getline(cin, input);
    cout << input << ": command not found" << endl;
  }
}
