#ifndef PICLIB_H
#define PICLIB_H

#include "Picture.hpp"
#include "Utils.hpp"
#include "LockablePicNodeList.hpp"

/*
 * PicLibrary's thread-safety implementation:
 * My internal picture storage has been implemented as a singly-linked list
 * so that any methods that require iterating through internalstorage
 * required locking onto the two nodes that are currently being accessed. I
 * implemented the PicNode class (every node has one mutex) and the
 * LockablePicNodeList class which is implemented as a chain of PicNode and
 * uses two dummy nodes for the head and tail with the name "\n" so that they
 * can never be moved or accessed. Accessing nodes will return a pair of
 * nodes (can be considered the position) so that addition, removal or editing
 * of nodes can occur, then unlocking will involve unlocking both of their
 * mutexes directly.
 *
 */

class PicLibrary {

  private:
    LockablePicNodeList internalstorage;
    Utils imgio = Utils();

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
  void rowblur(string filename);
  void columnblur(string filename);
  void sectorblur(string filename, int sectorsize);
  void pixelblur(string filename);
};

#endif

