#include <iostream>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"

using namespace std;

// TODO: Implement the picture library command-line interpreter

string getbasename(string path) { // ME : FIND MORE APPROPRIATE LOCATION FOR THIS METHOD IF POSSIBLE - TESTED: THIS WORKS
  string basename;
  for (char i : path)
    i == '/' ? (basename = "") : (basename += i);
  return basename;
}

int main(int argc, char ** argv) {

  cout << "------------------------------------------------------" << endl; 
  cout << "        The C++ Picture Processing Interpreter        " << endl;
  cout << "------------------------------------------------------" << endl;

  auto picLibrary = PicLibrary(); // ME : auto OR PicLibrary FOR DECLARING TYPE

  if (argc > 0) {
    for (int i = 1; i < argc; i++)
      picLibrary.loadpicture(argv[i], getbasename(argv[i]));
  }

  cout << "testing picture store" << endl;
  picLibrary.loadpicture("images/test.jpg", "test");
  picLibrary.print_picturestore();
  //picLibrary.loadpicture("images/test1.jpg", "shouldnotwork");

  picLibrary.rotate(90, "test");
  picLibrary.display("test"); // ME : NEED TO IMPLEMENT THE KEYSTROKE THING!

  picLibrary.flipVH('H', "ducks1.jpg");
  picLibrary.display("ducks1.jpg");

  picLibrary.flipVH('V', "ducks2.jpg");
  picLibrary.display("ducks2.jpg");

  picLibrary.blur("ducks3.jpg");
  picLibrary.display("ducks3.jpg");

  // write main IO loop that runs the command-line interpreter interactive shell

  return 0;

}


