#include <thread>
#include "PicLibrary.hpp"
#include "Colour.hpp"

using namespace std;
using namespace chrono;
// ME : ALMOST ALL OF THESE USE AN IF STATEMENT, COULD I REFACTOR THIS?
// error: MESSAGES FOR NON-EXISTENT FILENAMES ARE REDUNDANT
// ME : FIGURE OUT HOW TO LOCK EACH ELEMENT OF A MAP

void PicLibrary::print_picturestore() {
  internalstorage.printallfilenames();
}

void PicLibrary::loadpicture(string path, string filename) { // ME : COME BACK TO TEST THIS
  if (!internalstorage.search(filename)) {
    if (imgio.isitjpg(path)) {
      internalstorage.insertordered(path, filename);
    } else {
      cerr << "error: could not load image into internal picture storage because the path " + path +
              " does not point to a .jpg file." << endl;
    }
  } else {
    cerr << "error: could not load image into internal picture storage because " + filename + " already exists."
         << endl;
  }
}

void PicLibrary::unloadpicture(string filename) {
  if (internalstorage.search(filename)) {
    internalstorage.remove(filename);
  } else {
    cerr << "error: could not unload image from internal picture storage because " + filename + " doesn't exist."
         << endl;
  }
}

void PicLibrary::savepicture(string filename, string path) { // ME: I NEED A GETTER (AND SETTER?) FOR PicNode from LockablePicNodeList - WHAT TO DO IF PATH DOESN'T EXIST
  if (internalstorage.search(filename)) {
    Picture pic = internalstorage.getpicnodepicture(filename); // ME : GET PicNode THEN GET Picture
    imgio.saveimage(pic.getimage(), path);
  } else {
    cerr << "error: could not save image to internal picture storage because " + filename + " doesn't exist." << endl;
  }
}

void PicLibrary::display(string filename) { // ME : DOES display NEED A MUTEX? YES BECAUSE YOU'RE READING internalstorage USING .count(filename)
  if (internalstorage.search(filename)) {
    cout << filename + " is currently displayed." << endl;
    imgio.displayimage(internalstorage.getpicnodepicture(filename).getimage()); // ME : GET PicNode THEN GET Picture
  } else {
    cerr << "error: could not display image from internal picture storage because " + filename + " doesn't exist."
         << endl;
  }
}

void PicLibrary::invert(string filename) { // ME : DO ALL PICTURE TRANSFORMATIONS NEED MUTEXES? REFER TO QUESTIONS IN NOTES APP ON LAPTOP - NO BECAUSE OF THE WAY YOU IMPLEMENTED IT???
  if (internalstorage.search(filename)) {
    Picture pic = internalstorage.getpicnodepicture(filename);
    for (int i = 0; i < pic.getwidth(); i++) {
      for (int j = 0; j < pic.getheight(); j++) {
        Colour pixel = pic.getpixel(i, j); // ME : USE auto OR typeOfVariable CONSISTENTLY? CHOOSE WHEN AUTO APPROPRIATE?
        pic.setpixel(i, j, Colour(255 - pixel.getred(), 255 - pixel.getgreen(), 255 - pixel.getblue()));
      }
    }
    internalstorage.setpicnodepicture(filename, pic); // ME : YOU ONLY DID THIS TO UNLOCK IT THOUGH, IS THERE A BETTER WAY TO UNLOCK IT?
  } else {
    cerr << "error: could not invert image from internal picture storage because " + filename + " doesn't exist."
         << endl;
  }
}

void PicLibrary::grayscale(string filename) {
  if (internalstorage.search(filename)) {
    Picture pic = internalstorage.getpicnodepicture(filename);
    for (int i = 0; i < pic.getwidth(); i++) {
      for (int j = 0; j < pic.getheight(); j++) {
        Colour pixel = pic.getpixel(i, j);
        int avg = (pixel.getred() + pixel.getgreen() + pixel.getblue()) / 3; // ME : IS THIS SUFFICIENT FOR INTEGER DIVISION?
        pic.setpixel(i, j, Colour(avg, avg, avg));
      }
    }
    internalstorage.setpicnodepicture(filename, pic); // ME : ALSO, SETTING PIC MIGHT BE REDUNDANT WHEN IT'S ALREADY ACCESSED THE POINTER, SO YOU'RE JUST SETTING THE SAME IMAGE TWICE
  } else {
    cerr << "error: could not grayscale image from internal picture storage because " + filename + " doesn't exist."
         << endl;
  }
}

// ME : CHANGE for loop VARIABLES FROM x AND y TO i AND j
void PicLibrary::rotate(int angle, string filename) {
  if (internalstorage.search(filename)) {
    Picture pic = internalstorage.getpicnodepicture(filename);
    Picture newpic;
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    switch (angle) { // ME : IS THERE A WAY TO COMBINE THE STARTING OPERATIONS OF 90 AND 270? YES - PUT THE FOR LOOPS ON THE OUTSIDE AND SWITCH INSIDE IT
      case 90 : {
        newpic = Picture(picheight, picwidth);
        for (int i = 0; i < picwidth; i++) {
          for (int j = 0; j < picheight; j++) {
            Colour pixel = pic.getpixel(i, j); // ME : DEFINE A MACRO FOR THIS IN THE HEADER OF Picture.hpp?
            newpic.setpixel(picheight - j - 1, i, pixel); // ME : CONSISTENCY WITH DIFFERENT USES OF e.g. picwidth AND pic.getwidth AND newPic.getheight etc.
          }
        }
        break;
      }
      case 180 : {
        newpic = Picture(picwidth, picheight);
        for (int i = 0; i < picwidth; i++) {
          for (int j = 0; j < picheight; j++) {
            Colour pixel = pic.getpixel(i, j);
            newpic.setpixel(picwidth - i - 1, picheight - j - 1, pixel);
          }
        }
        break;
      }
      case 270 : {
        newpic = Picture(picheight, picwidth);
        for (int i = 0; i < picwidth; i++) {
          for (int j = 0; j < picheight; j++) {
            Colour pixel = pic.getpixel(i, j);
            newpic.setpixel(j, picwidth - i - 1, pixel);
          }
        }
        break;
      }
      default : {
        cerr << "error: can only rotate images by 90, 180 or 270 degrees clockwise." << endl;
        break;
      }
    }
    internalstorage.setpicnodepicture(filename, newpic);
  } else {
    cerr << "error: could not rotate image from internal picture storage because " + filename + " doesn't exist."
         << endl;
  }
}

void PicLibrary::flipVH(char plane, string filename) {
  if (internalstorage.search(filename)) {
    Picture pic = internalstorage.getpicnodepicture(filename);
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);
    if (plane == 'H') {
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
    internalstorage.setpicnodepicture(filename, newpic);
  } else {
    cerr << "error: could not flip image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
}

// ME : REMOVE THIS IF YOU'RE USING THE CONCURRENT IMPLEMENTATION
void PicLibrary::blur(string filename) {
  if (internalstorage.search(filename)) {
    Picture pic = internalstorage.getpicnodepicture(filename);
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);

    /*auto blurstart = high_resolution_clock::now();*/
    for (int x = 0; x < picwidth; x++) {
      for (int y = 0; y < picheight; y++) {
        newpic.setpixel(x, y, pic.blurpixel(x, y));
      }
    }
    /* auto blurfinish = high_resolution_clock::now(); // ME : ONLY CHECK THIS WHEN ROW BLUR IS ACTUALLY DONE
    auto sequentialblurduration = duration_cast<milliseconds>(blurfinish - blurstart);
    cout << filename + ": sequential blur's execution time = " << sequentialblurduration.count() << " milliseconds." << endl;*/

    internalstorage.setpicnodepicture(filename, newpic);
  } else {
    cerr << "error: could not blur image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
}

/* Used to check if thread implementations of blur gave the exact same output as my working blur. */
void PicLibrary::blurcheck(string filename1, string filename2) { // ME : DELETE THIS LATER
  Picture pic1 = internalstorage.getpicnodepicture(filename1);
  Picture pic2 = internalstorage.getpicnodepicture(filename2);
  bool result = true;
  if (pic1.getheight() == pic2.getheight() && pic1.getwidth() == pic2.getwidth()) {
    for (int i = 0; i < pic1.getwidth(); i++) {
      for (int j = 0; j < pic1.getheight(); j++) {
        auto p1 = pic1.getpixel(i,j);
        auto p2 = pic2.getpixel(i,j);
        result &= (p1.getred() == p2.getred() && p1.getgreen() == p2.getgreen() && p1.getblue() == p2.getblue());
      }
    }
    if (result) {
      cout << filename1 + " and " + filename2 + " are identically blurred." << endl;
    } else {
      cerr << filename1 + " and " + filename2 + " have not been identically blurred." << endl;
    }
  } else {
    cerr << filename1 + " and " + filename2 + " weren't of the same dimensions."  << endl;
  }
}

// ME : FOR ALL BELOW, ASSERT PIC SIZE HAS SAME DIMENSIONS AS NEW PIC
// ME : ORDER THESE PROPERLY
void rowblurthread(Picture pic, Picture newpic, int row) {
  for (int i = 0; i < pic.getwidth(); i++) {
    newpic.setpixel(i, row, pic.blurpixel(i, row));
  }
}

void PicLibrary::rowblur(string filename) {
  if (internalstorage.search(filename)) {
    Picture pic = internalstorage.getpicnodepicture(filename);
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);
    vector<thread> rowthreads;

    /*auto blurstart = high_resolution_clock::now();*/
    for (int i = 0; i < picheight; i++) {
      rowthreads.emplace_back(rowblurthread, pic, newpic, i);
    }
    for (int i = 0; i < picheight; i++) {
      rowthreads[i].join();
    }
    /*auto blurfinish = high_resolution_clock::now(); // ME : ONLY CHECK THIS WHEN ROW BLUR IS ACTUALLY DONE
    auto rowblurduration = duration_cast<milliseconds>(blurfinish - blurstart);
    cout << filename + ": row blur's execution time = " << rowblurduration.count() << " milliseconds." << endl;*/

    internalstorage.setpicnodepicture(filename, newpic);
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
  if (internalstorage.search(filename)) {
    Picture pic = internalstorage.getpicnodepicture(filename);
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);
    vector<thread> columnthreads;

    /*auto blurstart = high_resolution_clock::now();*/
    for (int i = 0; i < picwidth; i++) {
      columnthreads.emplace_back(columnblurthread, pic, newpic, i);
    }
    for (int i = 0; i < picwidth; i++) {
      columnthreads[i].join();
    }
    /*auto blurfinish = high_resolution_clock::now();
    auto columnblurduration = duration_cast<milliseconds>(blurfinish - blurstart);
    cout << filename + ": column blur's execution time = " << columnblurduration.count() << " milliseconds." << endl;*/

    internalstorage.setpicnodepicture(filename, newpic);
  } else {
    cerr << "error: could not blur image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
}

// ME : ASSERT sectorsize IS OF FORM 2^2n AND n > 0 (OTHERWISE IF n = 0 THEN THIS WILL HAVE THE SAME EFFECT AS pixelblurthread
void sectorblurthread(Picture pic, Picture newpic, int sectorsize, int xstart, int ystart) {
  int xend = xstart + (pic.getwidth()) / sectorsize;
  int yend = ystart + (pic.getheight()) / sectorsize;
  for (int i = xstart; i < xend; i++) {
    for (int j = ystart; j < yend; j++) {
      newpic.setpixel(i, j, pic.blurpixel(i, j));
    }
  }
}

void PicLibrary::sectorblur(string filename, int sectorsize) {
  if (internalstorage.search(filename)) {
    Picture pic = internalstorage.getpicnodepicture(filename);
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);
    vector<thread> sectorthreads;

    /*auto blurstart = high_resolution_clock::now();*/
    int threadcount = 0;
    for (int j = 0; j < picwidth; j += (picwidth) / sectorsize) {
      for (int k = 0; k < picheight; k += (picheight) / sectorsize) {
        sectorthreads.emplace_back(sectorblurthread, pic, newpic, sectorsize, j, k);
        threadcount++;
      }
    }
    for (int i = 0; i < threadcount; i++) {
      sectorthreads[i].join();
    }
    /*auto blurfinish = high_resolution_clock::now();
    auto sectorblurduration = duration_cast<milliseconds>(blurfinish - blurstart);
    cout << filename + ": sector blur's execution time = " << sectorblurduration.count() << " milliseconds." << endl;*/

    internalstorage.setpicnodepicture(filename, newpic);
  } else {
    cerr << "error: could not blur image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
}

void pixelblurthread(Picture pic, Picture newpic, int x, int y) {
  newpic.setpixel(x, y, pic.blurpixel(x, y));
}

void PicLibrary::pixelblur(string filename) {
  if (internalstorage.search(filename)) {
    Picture pic = internalstorage.getpicnodepicture(filename);
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);
    vector<thread> pixelthreads;
    int threadcount = 0;

    /*auto blurstart = high_resolution_clock::now();*/
    for (int j = 0; j < picwidth; j++) {
      for (int k = 0; k < picheight; k++) {
        pixelthreads.emplace_back(pixelblurthread, pic, newpic, j, k);
        threadcount++;
      }
    }
    for (int i = 0; i < threadcount; i++) {
      pixelthreads[i].join();
    }
    /*auto blurfinish = high_resolution_clock::now();
    auto pixelblurduration = duration_cast<milliseconds>(blurfinish - blurstart);
    cout << filename + ": pixel blur's execution time = " << pixelblurduration.count() << " milliseconds." << endl;*/

    internalstorage.setpicnodepicture(filename, newpic);
  } else {
    cerr << "error: could not blur image from internal picture storage because " + filename + " doesn't exist." << endl;
  }
}