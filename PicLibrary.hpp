#ifndef PICLIB_H
#define PICLIB_H

#include <map>
#include "Picture.hpp"
#include "Utils.hpp"

class PicLibrary {

  private:
    map<string, Picture> internalstorage;
    Utils imgio = Utils();
    mutex piclibmutex;

  public:
  // default constructor/destructor
  PicLibrary() = default;;
  ~PicLibrary() = default;;

  // command-line interpreter routines
  void print_picturestore();
  void loadpicture(string path, string filename);
  void unloadpicture(string filename);
  void savepicture(string filename, string path);
  void display(string filename);
  
  // picture transformation routines
  void invert(string filename);
  void grayscale(string filename);
  void rotate(int angle, string filename);
  void flipVH(char plane, string filename);
  void blur(string filename);
};

#endif

