#include <iostream>
#include <sstream>
#include <thread>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"

using namespace std;

vector<string> const lookuptable{"liststore",
                                 "load",
                                 "unload",
                                 "save",
                                 "display",
                                 "invert",
                                 "grayscale",
                                 "rotate",
                                 "flip",
                                 "blur",
                                 "view"};

/* Used for switch statement's int expression (in which the cases are linked
 * to each option of the Picture Library) in main() */
int lookup(const string &command) {
  for (int i = 0; i < lookuptable.size(); i++) {
    if (lookuptable[i] == command) {
      return i;
    }
  }
  return -1;
}

string getbasename(string path) {
  string basename;
  for (char i : path) {
    i == '/' ? (basename = "") : (basename += i);
  }
  return basename;
}

vector<string> tokenise(const string &line) {
  vector<string> tokens;
  string token;
  stringstream check1(line);
  while (getline(check1, token, ' ')) {
    tokens.push_back(token);
  }
  return tokens;
}

int main(int argc, char **argv) {

  cout << "------------------------------------------------------" << endl;
  cout << "        The C++ Picture Processing Interpreter        " << endl;
  cout << "------------------------------------------------------" << endl;

  cout << "before initialising library" << endl;
  auto picLibrary = PicLibrary();
  cout << "library initialised" << endl;

  /* Pre-loads into picLibrary any images from the image filepaths (if any)
   * that the program is invoked with on the command-line. */
  if (argc > 0) {
    for (int i = 1; i < argc; i++) {
      picLibrary.loadpicture(argv[i], getbasename(argv[i]));
    }
  }

  vector<thread> workerthreads;

  cout << "type \"view\" to see a list of options." << endl;

  string line;
  cout << "cpp prompt > ";
  while (getline(cin, line)) {
    vector<string> tokens = tokenise(line);
    switch (lookup(tokens[0])) {
      case 0 : { // liststore
        if (tokens.size() == 1) {
          workerthreads.emplace_back(&PicLibrary::print_picturestore,
                                     &picLibrary);
        } else {
          cerr << "incorrect number of arguments. command failed. type "
                  "\"view\" to see a list of options." << endl;
        }
        break;
      }
      case 1 : { /* load <file_path> <file_name> */
        if (tokens.size() == 3) {
          thread t = thread(&PicLibrary::loadpicture, &picLibrary, tokens[1],
                            tokens[2]);
          t.join();
        } else {
          cerr
              << "incorrect number of arguments. command failed. type \"view\" to see a list of options."
              << endl;
        }
        break;
      }
      case 2 : { /* unload <file_name> */
        if (tokens.size() == 2) {
          workerthreads.emplace_back(&PicLibrary::unloadpicture, &picLibrary,
                                     tokens[1]);
        } else {
          cerr
              << "incorrect number of arguments. command failed. type \"view\" to see a list of options."
              << endl;
        }
        break;
      }
      case 3 : { /* save <file_name> <file_path> */
        if (tokens.size() == 3) {
          thread t = thread(&PicLibrary::savepicture, &picLibrary, tokens[1],
                            tokens[2]);
          t.join();
        } else {
          cerr
              << "incorrect number of arguments. command failed. type \"view\" to see a list of options."
              << endl;
        }
        break;
      }
      case 4 : { /* display <file_name> */
        if (tokens.size() == 2) {
          workerthreads.emplace_back(&PicLibrary::display, &picLibrary,
                                     tokens[1]);
        } else {
          cerr
              << "incorrect number of arguments. command failed. type \"view\" to see a list of options."
              << endl;
        }
        break;
      }
      case 5 : { /* invert <file_name> */
        if (tokens.size() == 2) {
          workerthreads.emplace_back(&PicLibrary::invert, &picLibrary,
                                     tokens[1]);
        } else {
          cerr
              << "incorrect number of arguments. command failed. type \"view\" to see a list of options."
              << endl;
        }
        break;
      }
      case 6 : { /* grayscale <file_name> */
        if (tokens.size() == 2) {
          workerthreads.emplace_back(&PicLibrary::grayscale, &picLibrary,
                                     tokens[1]);
        } else {
          cerr
              << "incorrect number of arguments. command failed. type \"view\" to see a list of options."
              << endl;
        }
        break;
      }
      case 7 : { /* rotate [90|180|270] <file_name> */
        if (tokens.size() == 3) {
          workerthreads.emplace_back(&PicLibrary::rotate, &picLibrary,
                                     stoi(tokens[1]), tokens[2]);
        } else {
          cerr
              << "incorrect number of arguments. command failed. type \"view\" to see a list of options."
              << endl;
        }
        break;
      }
      case 8 : { /* flip [H|V] <file_name> */
        if (tokens.size() == 3) {
          workerthreads.emplace_back(&PicLibrary::flipVH, &picLibrary,
                                     tokens[1][0], tokens[2]);
        } else {
          cerr
              << "incorrect number of arguments. command failed. type \"view\" to see a list of options."
              << endl;
        }
        break;
      }
      case 9 : { /* blur <file_name> */
        if (tokens.size() == 2) {
          workerthreads.emplace_back(&PicLibrary::rowblur, &picLibrary,
              tokens[1]);
        } else {
          cerr
              << "incorrect number of arguments. command failed. type \"view\" to see a list of options."
              << endl;
        }
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
      default : { /* tokens[0] is not recognised as a supported command */
        if (tokens[0] != "exit") {
          cerr << "error: " + tokens[0] + " is not a supported command."
               << endl;
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

  /* joins all threads to ensure all have completed before exiting */
  for (auto &t : workerthreads) {
    t.join();
  }

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

  /*picLibrary.loadpicture("images/joker.jpg", "sectorblurjoker4");
  picLibrary.sectorblur("sectorblurjoker4", 4);*/

  /*picLibrary.loadpicture("images/joker.jpg", "sectorblurjoker8");
  picLibrary.sectorblur("sectorblurjoker8", 8);*/

  return 0;

}