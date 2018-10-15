#include <thread>
#include "PicLibrary.hpp"
#include "Colour.hpp"

using namespace std; // ME : DO I PUT THE NAMESPACE HERE? - YES BECAUSE I DO ALL MY ERROR MESSAGES HERE AS OPPOSED TO Main.cpp
// ME " ALMOST ALL OF THESE USE AN IF STATEMENT, COULD I REFACTOR THIS?
// error: MESSAGES FOR NON-EXISTENT FILENAMES ARE REDUNDANT

void PicLibrary::print_picturestore() {
  cout << "internal picture storage:" << endl;
  //piclibmutex.lock();
  for (auto &i : internalstorage) { // ME : map<string, Picture>::iterator (USE auto WHEN DECLARING ITERATORS)
    cout << i.first << endl;
  }
  //piclibmutex.unlock();
}

void PicLibrary::loadpicture(string path, string filename) { // ME : COME BACK TO TEST THIS
  Picture pic = Picture(path);
  //piclibmutex.lock(); // ME : READ ABOUT read-and-write locks IF YOU NEED TO READ AND WRITE
  if (internalstorage.count(filename) == 0) {
    if (imgio.isitjpg(path)) {
      //piclibmutex.lock(); // ME : SHOULD THIS NOT BE ABOVE THE IF CONDITION? BECAUSE WE ARE ACCESSING internalstorage FOR THE CONDITION?
      internalstorage.insert(pair<string, Picture>(filename, pic));
      //piclibmutex.unlock(); // ME : IF I LOCK THE MUTEX OUTSIDE IF CONDITION THEN MUST UNLOCK IT OUTSIDE TOO?
      cout << filename + " has been loaded into the internal picture storage." << endl;
    } else {
      cerr << "error: could not load image into internal picture storage because the path " + path + " does not point to a .jpg file." << endl;
    }
  } else {
    cerr << "error: could not load image into internal picture storage because " + filename + " already exists." << endl;
  }
  //piclibmutex.unlock(); // ME : DON'T REALLY WANT TO UNLOCK IT AFTER THIS MANY LINES (LOOKS PRETTY SEQUENTIAL STILL)
}

void PicLibrary::unloadpicture(string filename) {
  //piclibmutex.lock();
  if (internalstorage.count(filename) != 0) {
    internalstorage.erase(filename);
    cout << filename + " has been unloaded from the internal picture storage." << endl;
  } else {
    cerr << "error: could not unload image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
  //piclibmutex.unlock();
}

void PicLibrary::savepicture(string filename, string path) { // ME : WHAT TO DO IF PATH DOESN'T EXIST - DOES savepicture NEED A MUTEX? - YES BECAUSE YOU'RE READING internalstorage USING .count(filename)
  //piclibmutex.lock();
  if (internalstorage.count(filename) != 0) {
      Picture pic = internalstorage[filename];
      imgio.saveimage(pic.getimage(), path);
      cout << filename + " has been saved to the path " + path + "." << endl;
  } else {
    cerr << "error: could not save image to internal picture storage because " + filename + " doesn't exist." << endl;
  }
  //piclibmutex.unlock();
}

void PicLibrary::display(string filename) { // ME : DOES display NEED A MUTEX? YES BECAUSE YOU'RE READING internalstorage USING .count(filename)
  // ME : char keystroke;
  if (internalstorage.count(filename) != 0) {
    imgio.displayimage(internalstorage[filename].getimage());
    cout << filename + " is currently displayed." << endl;
  } else {
    cerr << "error: could not display image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
}

void PicLibrary::invert(string filename) { // ME : DO ALL THE PICTURE TRANSFORMATIONS NEED MUTEXES? REFER TO QUESTIONS IN NOTES APP ON LAPTOP
  if (internalstorage.count(filename) != 0) {
    Picture pic = internalstorage[filename];
    for (int x = 0; x < pic.getwidth(); x++) {
      for (int y = 0; y < pic.getheight(); y++) {
        Colour pixel = pic.getpixel(x, y); // ME : USE auto OR typeOfVariable CONSISTENTLY? CHOOSE WHEN AUTO APPROPRIATE?
        pic.setpixel(x, y, Colour(255 - pixel.getred(), 255 - pixel.getgreen(), 255 - pixel.getblue()));
      }
    }
    cout << filename + " has been inverted." << endl;
  } else {
    cerr << "error: could not invert image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
}

void PicLibrary::grayscale(string filename) {
  if (internalstorage.count(filename) != 0) {
    Picture pic = internalstorage[filename];
    for (int x = 0; x < pic.getwidth(); x++) {
      for (int y = 0; y < pic.getheight(); y++) {
        Colour pixel = pic.getpixel(x, y);
        int avg = (pixel.getred() + pixel.getgreen() + pixel.getblue()) / 3; // ME : IS THIS SUFFICIENT FOR INTEGER DIVISION?
        pic.setpixel(x, y, Colour(avg, avg, avg));
      }
    }
    cout << filename + " has been grayscaled." << endl;
  } else {
    cerr << "error: could not grayscale image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
}

void PicLibrary::rotate(int angle, string filename) {
  if (internalstorage.count(filename) != 0) {
    Picture pic = internalstorage[filename];
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic;
    switch (angle) { // ME : IS THERE A WAY TO COMBINE THE STARTING OPERATIONS OF 90 AND 270? YES - PUT THE FOR LOOPS ON THE OUTSIDE AND SWITCH INSIDE IT
      case 90 : {
        newpic = Picture(picheight, picwidth);
        for (int x = 0; x < picwidth; x++) {
          for (int y = 0; y < picheight; y++) {
            Colour pixel = pic.getpixel(x, y); // ME : DEFINE A MACRO FOR THIS IN THE HEADER OF Picture.hpp?
            newpic.setpixel(newpic.getwidth() - y - 1, x, pixel); // ME : CONSISTENCY WITH DIFFERENT USES OF e.g. picwidth AND pic.getwidth AND newPic.getheight etc.
          }
        }
        break;
      }
      case 180 : {
        newpic = Picture(picwidth, picheight);
        for (int x = 0; x < picwidth; x++) {
          for (int y = 0; y < picheight; y++) {
            Colour pixel = pic.getpixel(x, y);
            newpic.setpixel(picwidth - x - 1, picheight - y - 1, pixel);
          }
        }
        break;
      }
      case 270 : {
        newpic = Picture(picheight, picwidth);
        for (int x = 0; x < picwidth; x++) {
          for (int y = 0; y < picheight; y++) {
            Colour pixel = pic.getpixel(x, y);
            newpic.setpixel(y, newpic.getheight() - x - 1, pixel);
          }
        }
        break;
      }
      default : {
        cerr << "error: can only rotate images by 90, 180 or 270 degrees clockwise." << endl;
        break;
      }
    }
    internalstorage[filename] = newpic;
    cout << filename + " has been rotated by " << angle << " degrees." << endl;
  } else {
    cerr << "error: could not rotate image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
}

void PicLibrary::flipVH(char plane, string filename) {
  if (internalstorage.count(filename) != 0) {
    Picture pic = internalstorage[filename];
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);
    if (plane == 'H') { // ME : IS THERE A GOOD WAY I COULD SIMPLIFY THE USE OF TWO NESTED FOR LOOPS? YES - PUT THE FOR LOOPS OUTSIDE AND THE IF-ELSE INSIDE - BUT THAT SUGGESTS THAT EACH PIXEL COULD HAVE DIFFERENT FLIPS WHICH IS NOT POSSIBLE SO NO
      for (int x = 0; x < picwidth; x++) {
        for (int y = 0; y < picheight; y++) {
          newpic.setpixel(picwidth - x - 1, y, pic.getpixel(x, y));
        }
      }
    } else if (plane == 'V') {
      for (int x = 0; x < picwidth; x++) {
        for (int y = 0; y < picheight; y++) {
          newpic.setpixel(x, picheight - y - 1, pic.getpixel(x, y));
        }
      }
    } else {
      cerr << "error: can only flip images vertically (V) or horizontally (H)." << endl;
    }
    internalstorage[filename] = newpic;
    cout << filename + " has been flipped ";
    if (plane == 'H') {
      cout << "horizontally." << endl;
    } else if (plane == 'V') {
      cout << "vertically." << endl;
    }
  } else {
    cerr << "error: could not flip image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
}

// ME : REMOVE THIS IF YOU'RE USING THE CONCURRENT IMPLEMENTATION
void PicLibrary::blur(string filename) {
  if (internalstorage.count(filename) != 0) {
    Picture pic = internalstorage[filename];
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);
    for (int x = 0; x < picwidth; x++) {
      for (int y = 0; y < picheight; y++) {
        newpic.setpixel(x, y, pic.blurpixel(x, y));
      }
    }
    internalstorage[filename] = newpic;
    cout << filename + " has been blurred." << endl;
  } else {
    cerr << "error: could not blur image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
}

// ME : FOR ALL BELOW, ASSERT PIC SIZE HAS SAME DIMENSIONS AS NEW PIC
// ME : ADD THESE METHODS TO PicLibrary.hpp PUBLIC MEMBER FUNCTIONS LIST
// ME : ORDER THESE PROPERLY
void rowblurthread(Picture pic, Picture newpic, int row) {
  for (int i = 0; i < pic.getwidth(); i++) {
    newpic.setpixel(i, row, pic.blurpixel(i, row));
  }
}

void PicLibrary::rowblur(string filename) {
  if (internalstorage.count(filename) != 0) {
    Picture pic = internalstorage[filename];
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);
    vector<thread> rowthreads;
    for (int i = 0; i < picheight; i++) {
      rowthreads.emplace_back(rowblurthread, pic, newpic, i);
    }
    for (int i = 0; i < picheight; i++) {
      rowthreads[i].join();
    }
    internalstorage[filename] = newpic;
    cout << filename + " has been blurred." << endl;
  } else {
    cerr << "error: could not blur image from internal picture storage because " + filename + " doesn't exist." << endl;
  }

}

void columnblurthread(Picture pic, Picture newpic, int column) {
  for (int i = 0; i < pic.getheight(); i++) {
    newpic.setpixel(column, i, pic.blurpixel(column, i));
  }
}

void PicLibrary::columnblur(string filename) {
  if (internalstorage.count(filename) != 0) {
    Picture pic = internalstorage[filename];
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);
    vector<thread> columnthreads;
    for (int i = 0; i < picwidth; i++) {
      columnthreads.emplace_back(columnblurthread, pic, newpic, i);
    }
    for (int i = 0; i < picwidth; i++) {
      columnthreads[i].join();
    }
    internalstorage[filename] = newpic;
    cout << filename + " has been blurred." << endl;
  } else {
    cerr << "error: could not blur image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
}

// ME : ASSERT sectorsize IS OF FORM 2^2n AND n > 0 (OTHERWISE IF n = 0 THEN THIS WILL HAVE THE SAME EFFECT AS pixelblurthread
void sectorblurthread(Picture pic, Picture newpic, int sectorsize, int xstart, int ystart) {
  int xend = xstart + (pic.getwidth() - 1) / sectorsize;
  int yend = ystart + (pic.getheight() - 1) / sectorsize;
  for (int i = xstart; i < xend; i++) {
    for (int j = ystart; j < xend; j++) {
      newpic.setpixel(i, j, pic.blurpixel(i, j));
    }
  }
}

void PicLibrary::sectorblur(string filename, int sectorsize) {
  if (internalstorage.count(filename) != 0) {
    Picture pic = internalstorage[filename];
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);
    vector<thread> sectorthreads;
    cout << "1here" << endl;
    for (int i = 0; i < (sectorsize ^ 2); i++) {
      for (int j = 0; j < picwidth; j += (picwidth - 1) / sectorsize) {
        for (int k = 0; k < picheight; k += (picheight - 1) / sectorsize) {
          cout << "2here" << endl;
          sectorthreads.emplace_back(sectorblurthread, pic, newpic, sectorsize, j, k);
          cout << "3here" << endl;
        }
      }
    }
    cout << "4here" << endl;
    for (int i = 0; i < (sectorsize ^ 2); i++) {
      sectorthreads[i].join();
    }
    internalstorage[filename] = newpic;
    cout << filename + " has been blurred." << endl;
  } else {
    cerr << "error: could not blur image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
}

void pixelblurthread(Picture pic, Picture newpic, int x, int y) {
  newpic.setpixel(x, y, pic.blurpixel(x, y));
}

void PicLibrary::pixelblur(string filename) {
  if (internalstorage.count(filename) != 0) {
    Picture pic = internalstorage[filename];
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);
    vector<thread> pixelthreads;
    for (int i = 0; i < picwidth * picheight; i++) {
      for (int j = 0; j < picwidth; j++) {
        for (int k = 0; k < picheight; k++) {
          pixelthreads.emplace_back(pixelblurthread, pic, newpic, j, k);
        }
      }
    }
    for (int i = 0; i < picwidth * picheight; i++) {
      pixelthreads[i].join();
    }
    internalstorage[filename] = newpic;
    cout << filename + " has been blurred." << endl;
  } else {
    cerr << "error: could not blur image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
}