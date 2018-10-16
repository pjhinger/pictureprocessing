#include <iostream>
#include <sstream>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"

using namespace std;

string const lookuptable[11] = { "liststore",
                                 "load",
                                 "unload",
                                 "save",
                                 "display",
                                 "invert",
                                 "grayscale",
                                 "rotate",
                                 "flip",
                                 "blur",
                                 "view" };

/* Used for switch statement's int expression (in which the cases are linked
 * to each option of the Picture Library) in main() */
int lookup(const string &command) {
  for (int i = 0; i < sizeof(lookuptable) / sizeof(string) ; i++) { // ME : SHORTER EXPRESSION FOR STOPPING CONDITION?
    if (lookuptable[i] == command) {
      return i;
    }
  }
  return -1;
}

string getbasename(string path) { // ME : FIND MORE APPROPRIATE LOCATION FOR THIS METHOD IF POSSIBLE - TESTED: THIS WORKS
  string basename;
  for (char i : path) {
    i == '/' ? (basename = "") : (basename += i);
  }
  return basename; // ME : TRY REMOVING THE .JPG POSTFIX FROM IT
}

vector<string> tokenise(const string &line) { // ME : CHANGE TO AN ARRAY?
  vector<string> tokens;
  string token;
  stringstream check1(line);
  while (getline(check1, token, ' ')) {
    tokens.push_back(token);
  }
  return tokens;
}

int main(int argc, char ** argv) {

  cout << "------------------------------------------------------" << endl; 
  cout << "        The C++ Picture Processing Interpreter        " << endl;
  cout << "------------------------------------------------------" << endl;

  // ME : SHOULD PROBABLY OUTPUT ALL POSSIBLE COMMAND LINE OPTIONS FROM THE SPEC'S BNF
  // ME : REMEMBER TO FREE ANYTHING THAT NEEDED MEMORY FROM HEAP!

  auto picLibrary = PicLibrary(); // ME : auto OR PicLibrary FOR DECLARING TYPE

  /* Pre-loads into picLibrary any images from the image filepaths (if any)
   * that the program is invoked with on the command-line. */
  if (argc > 0) {
    for (int i = 1; i < argc; i++) {
      picLibrary.loadpicture(argv[i], getbasename(argv[i]));
    }
  }

  cout << "type \"view\" to see a list of options." << endl;

  string line;
  cout << "cpp prompt > ";
  while (getline(cin, line)) {
    vector<string> tokens = tokenise(line);
    switch (lookup(tokens[0])) { // ME : ADD ASSERTIONS FOR NUMBER OF TOKENS GIVEN IN EACH COMMAND IF TOO FEW/MANY ARGS GIVEN
      case 0 : { // liststore
        picLibrary.print_picturestore();
        break;
      }
      case 1 : { // load <file_path> <file_name>
        picLibrary.loadpicture(tokens[1], tokens[2]);
        break;
      }
      case 2 : { // unload <file_name>
        picLibrary.unloadpicture(tokens[1]);
        break;
      }
      case 3 : { // save <file_name> <file_path>
        picLibrary.savepicture(tokens[1], tokens[2]);
        break;
      }
      case 4 : { // display <file_name>
        picLibrary.display(tokens[1]); // ME : NEED A WAY TO CLOSE THE GUI WINDOW BY A KEY STROKE
        string anything;
        do {
          getline(cin, anything);
        } while (getline(cin, anything));
        break;
      }
      case 5 : { // invert <file_name>
        picLibrary.invert(tokens[1]);
        break;
      }
      case 6 : { // grayscale <file_name>
        picLibrary.grayscale(tokens[1]);
        break;
      }
      case 7 : { // rotate [90|180|270] <file_name>
        //int angle = stoi(tokens[1]);
        picLibrary.rotate(stoi(tokens[1]), tokens[2]);
        break;
      }
      case 8 : { // flip [H|V] <file_name>
        picLibrary.flipVH(tokens[1][0], tokens[2]); // ME : VERY HACKY TO ACCESS 0TH INDEX OF SINGLE CHARACTER STRING, FIND AN APPROPRIATE FUNCTION
        break;
      }
      case 9 : { // blur <file_name>
        picLibrary.blur(tokens[1]);
        break;
      }
      case 10 : {
        cout << endl;
        cout << "picture library options:" << endl;
        cout << "* liststore" << endl;
        cout << "* load <file_path> <file_name>" << endl;
        cout << "* unload <file_name>" << endl;
        cout << "* save <file_name> <file_path>" << endl;
        cout << "* exit" << endl;
        cout << "* display <file_name>" << endl;
        cout << endl;
        cout << "picture transformation options:" << endl;
        cout << "* invert <file_name>" << endl;
        cout << "* grayscale <file_name>" << endl;
        cout << "* rotate [90|180|270] <file_name>" << endl;
        cout << "* flip [H|V] <file_name>" << endl;
        cout << "* blur <file_name>" << endl;
        cout << endl;
        break;
      }
      default : { // tokens[0] is not recognised as a supported command
        if (tokens[0] != "exit") {
          cerr << "error: " + tokens[0] + " is not a supported command." << endl;
        }
        break;
      }
    }
    if (tokens[0] == "exit") {
      break;
    } else {
      cout << "cpp prompt > ";
    }
  }

  /*
   * Only once the standard input stream is closed, should your program exit.
   *
   * A simple command-line interpreter will display a prompt, accept a command line typed by the
   * user terminated by the Enter key, then execute the specified command and provide a textual display
   * of the results or error messages generated.
   *
   * exit command should stop the command-line interpreter, wait for any outstanding worker threads to
   * complete and then free all of the resources used by this program before terminating with a successful exit code of 0.
   *
   * */

  return 0;

  /* CODE USED INSIDE main() IN Main.cpp FOR BLUR EXPERIMENT */

  /*picLibrary.loadpicture("images/joker.jpg", "blurjoker");
  picLibrary.blur("blurjoker");*/

  /*picLibrary.loadpicture("images/joker.jpg", "rowblurjoker");
  picLibrary.rowblur("rowblurjoker");*/

  /*picLibrary.loadpicture("images/joker.jpg", "columnblurjoker");
  picLibrary.columnblur("columnblurjoker");*/

  /*picLibrary.loadpicture("images/joker.jpg", "pixelblurjoker");
  picLibrary.pixelblur("pixelblurjoker");*/

  /*picLibrary.loadpicture("images/joker.jpg", "sectorblurjoker1");
  picLibrary.sectorblur("sectorblurjoker1", 1);*/

  /*picLibrary.loadpicture("images/joker.jpg", "sectorblurjoker2");
  picLibrary.sectorblur("sectorblurjoker2", 2);*/

  // ME : SEGMENTATION FAULT
  /*picLibrary.loadpicture("images/joker.jpg", "sectorblurjoker3");
  picLibrary.sectorblur("sectorblurjoker3", 3);*/

  /*picLibrary.loadpicture("images/joker.jpg", "sectorblurjoker4");
  picLibrary.sectorblur("sectorblurjoker4", 4);*/

  /*picLibrary.loadpicture("images/joker.jpg", "sectorblurjoker5");
  picLibrary.sectorblur("sectorblurjoker5", 5);*/

  // ME : SEGMENTATION FAULT
  /*picLibrary.loadpicture("images/joker.jpg", "sectorblurjoker6");
  picLibrary.sectorblur("sectorblurjoker6", 6);*/

  // ME : SEGMENTATION FAULT
  /*picLibrary.loadpicture("images/joker.jpg", "sectorblurjoker7");
  picLibrary.sectorblur("sectorblurjoker7", 7);*/

  /*picLibrary.loadpicture("images/joker.jpg", "sectorblurjoker8");
  picLibrary.sectorblur("sectorblurjoker8", 8);*/

  // ME : SEGMENTATION FAULT
  /*picLibrary.loadpicture("images/joker.jpg", "sectorblurjoker9");
  picLibrary.sectorblur("sectorblurjoker9", 9);*/

  /*picLibrary.loadpicture("images/joker.jpg", "sectorblurjoker10");
  picLibrary.sectorblur("sectorblurjoker10", 10);*/

}