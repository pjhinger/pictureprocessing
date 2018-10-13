#ifndef PICTURE_H
#define PICTURE_H

#include "Colour.hpp"
#include "Utils.hpp"

// The Picture class provided a wrapper for image manipulation via the opencv
// library

class Picture {

  private:
    // opencv representation of an image
    Mat img;
    Utils imgio;

  public:
      
  enum RGB {BLUE, GREEN, RED};
 
  // class constructor, loads an image from a provided file
  Picture(string path);

  // class constructor, creates an image provided the size 
  Picture(int width, int height);  

  // default constructor/destructor
  Picture();
  ~Picture(){};

  // determine the dimensions of the underlying image
  int getwidth();
  int getheight();

  // extract a single pixel from the image as a Colour object
  Colour getpixel(int x, int y);

  // set a single pixel in the  image from a Colour object
  void setpixel(int x, int y, Colour rgb);

  // blurs a single pixel from the image and returns a Colour object
  Colour blurpixel(int x, int y);

  // check if coordinates are within bounds of the stored image
  bool contains(int x, int y);

  // provides the image of the class
  Mat getimage();
  
  // overwrite Picture image with another image
  void setimage(Mat cpyimg);

  /*
   * Returns true if image provided is a .jpg file.
   * .jpg files store the magic numbers (given in hexadecimal) ffd8 at the beginning
   * and ffd9 at the end
   */
  bool isitjpg(string path);
  
 };

#endif
