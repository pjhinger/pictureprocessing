#include <fstream>
#include "Utils.hpp"

Utils::Utils() {}

Mat Utils::createimage(int width, int height) {
  Mat img;
  try {
    cout << "creating colour image" << endl;
    img.create(height, width, CV_8UC3);
  } catch (runtime_error &ex) {
    fprintf(stderr, "Exception creating color image: %s\n", ex.what());
    img.release();
  }
  return img;
}

Mat Utils::copyimage(Mat img) {
  return img.clone();
}

bool Utils::saveimage(Mat img, string filename) {
  vector<int> compression_params;

  compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
  compression_params.push_back(95); //default value in opencv

  try {
    imwrite(filename, img, compression_params);
  } catch (runtime_error &ex) {
    fprintf(stderr, "Exception converting image to JPEG format: %s\n", ex.what());
    return false;
  }
  return true;
}

Mat Utils::loadimage(string path) {
  Mat img;
  try {
    img = imread(path, CV_LOAD_IMAGE_COLOR);
    if (!img.data) {
      cout << "Could not load the image" << endl;
    }
  } catch (runtime_error &ex) {
    fprintf(stderr, " %s\n", ex.what());
    img.release();
  }
  return img;
}

void Utils::displayimage(Mat img) {
  namedWindow("Picture processing", WINDOW_AUTOSIZE);
  imshow("Picture processing", img);
  waitKey(0);
  destroyWindow("Picture processing");
}

bool Utils::isitjpg(string path) {
  /*uint16_t magicstart = 0xffd8; // ME : CAN I MOVE THESE MAGIC NUMBERS TO THE HEADER USING #define
  // uint16_t magicend = 0xffd9; // ME : DO THE SAME FOR THE END OF THE FILE

  fstream file(path, ios::in | ios::out | ios::binary);

  uint16_t &prefix;
  // uint16_t postfix;

  cout << *prefix << endl;

  file.read(reinterpret_cast<char *>(prefix), 2);

  cout << prefix << endl;

  // cout << magicstart << endl;
  // cout << *prefix << endl;

  return prefix == magicstart; // ME : && postfix == magicend;*/

  string extension;

  for (int i = 0; i < path.size(); i++) {
    path[i] == '.' ? extension = "." : extension += path[i];
  }

  return extension == ".jpg";
}