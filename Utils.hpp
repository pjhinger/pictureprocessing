#ifndef UTILS_H
#define UTILS_H

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;
using namespace cv;

class Utils{
    
 public:
   Utils();
  
  /**
   * Create a new instance of a Picture object of the specified width and
   * height, using the RGB colour model.
   * 
   * @param width
   *          width of new Picture
   * @param height
   *          height of new Picture
   * 
   * @return a new instance of a Mat object of the specified size.
   *         In an error case the Mat's data and reference counter pointers are set to NULL.  
   */
  Mat createimage(int width, int height);
  
  /**
   * Saves the given image format (jpg) in the given destination.
   * 
   * @param picture
   *          the picture to save to disk
   * @param destination
   *          where to save the Picture
   * @return true if the file was saved successfully, false otherwise
   */
  bool saveimage(Mat img, string filename);

  /**
   * Create a Picture object from the the image at the specified location (or local file).
   * 
   * @param location
   *          a String representing the location of the image to be loaded. This
   *          can either be a URL, or a filesystem location.
   * @return a Picture representing the image at the specified URL, or null if
   *         loading failed for any reason.
   *         In an error case the Mat's data and reference counter pointers are set to NULL.
   */
  Mat loadimage(string path);
  
  /**
   * Displays the image provided as argument on a GUI window
   */
  void displayimage(Mat img);
  
  /*clones the image provided as argument
   */
  Mat copyimage(Mat img);

  /**
    * Returns true if image provided is a .jpg file.
    * checking to see if ".jpg" is appended to the end of the filename is not reliable
    * as some files don't have to use this suffix, however, .jpg files store the magic
    * numbers (given in hexadecimal) ffd8 at the beginning and ffd9 at the end.
    * NOTE: .jpg files are stored in big endian format so values are converted to
    * little endian format.
    */
  bool isitjpg(string path);
  
};

#endif

