#include "Picture.hpp"

using namespace std;
using namespace cv;

Picture::Picture(string path) {
  img = imgio.loadimage(path);
}

Picture::Picture(int width, int height) {
  img = imgio.createimage(width, height);
}

Picture::Picture() {}

int Picture::getwidth() {
  return img.size().width;
}

int Picture::getheight() {
  return img.size().height;
}

Colour Picture::getpixel(int x, int y) {
  // Beware: opencv pixels are stored in a (y,x) vector, not an (x,y) vector!
  cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);
  // RGB format
  Colour rgb(pixel[RED], pixel[GREEN], pixel[BLUE]);
  return rgb;
}

void Picture::setpixel(int x, int y, Colour rgb) {
  // Beware: opencv pixels are stored in a (y,x) vector, not an (x,y) vector!
  img.at<cv::Vec3b>(y, x)[BLUE] = rgb.getblue();
  img.at<cv::Vec3b>(y, x)[GREEN] = rgb.getgreen();
  img.at<cv::Vec3b>(y, x)[RED] = rgb.getred();
}

/* returns the Colour of a single pixel that's blurred if it is not a
 * boundary pixel */
Colour Picture::blurpixel(int x, int y) {
  int redblur = 0;
  int greenblur = 0;
  int blueblur = 0;

  if (x > 0 && x < getwidth() - 1 && y > 0 && y < getheight() - 1) {
    for (int i = x - 1; i <= x + 1; i++) {
      for (int j = y - 1; j <= y + 1; j++) {
        Colour pixel = getpixel(i, j);
        redblur += pixel.getred();
        greenblur += pixel.getgreen();
        blueblur += pixel.getblue();
      }
    }
    redblur /= 9;
    greenblur /= 9;
    blueblur /= 9;
    return {redblur, greenblur, blueblur};
  } else {
    return getpixel(x, y);
  }
}

bool Picture::contains(int x, int y) {
  return x >= 0 && x < img.size().width && y >= 0 && y < img.size().height;
}

Mat Picture::getimage() {
  return img;
}

void Picture::setimage(Mat cpyimg) {
  img = imgio.copyimage(cpyimg);
}