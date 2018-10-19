#include <thread>
#include "PicLibrary.hpp"
#include "Colour.hpp"

using namespace std;
using namespace chrono;

void PicLibrary::print_picturestore() {
  internalstorage.printallfilenames();
}

void PicLibrary::loadpicture(string path, string filename) {
  if (imgio.isitjpg(path)) {
    bool result = internalstorage.insertordered(path, filename);
    if (!result) {
      cerr <<
           "error: could not load image into internal picture storage because " +
           filename + " already exists." << endl;
    }
  } else {
    cerr <<
         "error: could not load image into internal picture storage because the path " +
         path + " does not point to a .jpg file." << endl;
  }
}

void PicLibrary::unloadpicture(string filename) {
  bool result = internalstorage.remove(filename);

  if (!result) {
    cerr <<
         "error: could not unload image from internal picture storage because " +
         filename + " doesn't exist." << endl;
  }
}

void PicLibrary::savepicture(string filename, string path) {
  auto pair = internalstorage.findposition(filename);
  PicNode *picnode = pair.second;

  if (picnode != nullptr) {
    imgio.saveimage(picnode->getpic().getimage(), path);
  } else {
    cerr << "error: could not save image to internal picture storage because " +
            filename + " doesn't exist." << endl;
  }

  internalstorage.unlockpicnodes(pair);
}

void PicLibrary::display(string filename) {
  auto pair = internalstorage.findposition(filename);
  PicNode *picnode = pair.second;

  if (picnode != nullptr) {
    cout << filename + " is currently displayed" << endl;
    imgio.displayimage(picnode->getpic().getimage());
  } else {
    cerr <<
         "error: could not display image from internal picture storage because " +
         filename + " doesn't exist." << endl;
  }

  internalstorage.unlockpicnodes(pair);
}

void PicLibrary::invert(string filename) {
  auto pair = internalstorage.findposition(filename);
  PicNode *picnode = pair.second;

  if (picnode != nullptr) {
    Picture pic = picnode->getpic();
    for (int i = 0; i < pic.getwidth(); i++) {
      for (int j = 0; j < pic.getheight(); j++) {
        Colour pixel = pic.getpixel(i, j);
        pic.setpixel(i, j, Colour(255 - pixel.getred(), 255 - pixel.getgreen(),
                                  255 - pixel.getblue()));
      }
    }
  } else {
    cerr <<
         "error: could not invert image from internal picture storage because " +
         filename + " doesn't exist." << endl;
  }

  internalstorage.unlockpicnodes(pair);
}

void PicLibrary::grayscale(string filename) {
  auto pair = internalstorage.findposition(filename);
  PicNode *picnode = pair.second;

  if (picnode != nullptr) {
    Picture pic = picnode->getpic();
    for (int i = 0; i < pic.getwidth(); i++) {
      for (int j = 0; j < pic.getheight(); j++) {
        Colour pixel = pic.getpixel(i, j);
        int avg = (pixel.getred() + pixel.getgreen() + pixel.getblue()) / 3;
        pic.setpixel(i, j, Colour(avg, avg, avg));
      }
    }
  } else {
    cerr
        << "error: could not grayscale image from internal picture storage because \" + filename + \" doesn't exist."
        << endl;
  }

  internalstorage.unlockpicnodes(pair);
}

void PicLibrary::rotate(int angle, string filename) {
  auto pair = internalstorage.findposition(filename);
  PicNode *picnode = pair.second;

  if (picnode != nullptr) {
    Picture pic = picnode->getpic();
    Picture newpic;
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    switch (angle) {
      case 90 : {
        newpic = Picture(picheight, picwidth);
        for (int i = 0; i < picwidth; i++) {
          for (int j = 0; j < picheight; j++) {
            Colour pixel = pic.getpixel(i, j);
            newpic.setpixel(picheight - j - 1, i, pixel);
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
        cerr
            << "error: can only rotate images by 90, 180 or 270 degrees clockwise."
            << endl;
        break;
      }
    }
    picnode->setpic(newpic);
  } else {
    cerr <<
         "error: could not rotate image from internal picture storage because " +
         filename + " doesn't exist." << endl;
  }

  internalstorage.unlockpicnodes(pair);
}

void PicLibrary::flipVH(char plane, string filename) {
  auto pair = internalstorage.findposition(filename);
  PicNode *picnode = pair.second;

  if (picnode != nullptr) {
    Picture pic = picnode->getpic();
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
      cerr << "error: can only flip images vertically (V) or horizontally (H)."
           << endl;
    }

    picnode->setpic(newpic);
  } else {
    cerr <<
         "error: could not flip image from internal picture storage because " +
         filename + " doesn't exist." << endl;
  }

  internalstorage.unlockpicnodes(pair);
}

void PicLibrary::blur(string filename) {
  auto pair = internalstorage.findposition(filename);
  PicNode *picnode = pair.second;

  if (picnode != nullptr) {
    Picture pic = picnode->getpic();
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);

    /*auto blurstart = high_resolution_clock::now();*/
    for (int x = 0; x < picwidth; x++) {
      for (int y = 0; y < picheight; y++) {
        newpic.setpixel(x, y, pic.blurpixel(x, y));
      }
    }
    /* auto blurfinish = high_resolution_clock::now();
    auto sequentialblurduration = duration_cast<milliseconds>(blurfinish -
     blurstart);
    cout << filename + ": sequential blur's execution time = " <<
     sequentialblurduration.count() << " milliseconds." << endl;*/

    picnode->setpic(newpic);
  } else {
    cerr <<
         "error: could not blur image from internal picture storage because " +
         filename + " doesn't exist." << endl;
  }

  internalstorage.unlockpicnodes(pair);
}

void rowblurthread(Picture pic, Picture newpic, int row) {
  for (int i = 0; i < pic.getwidth(); i++) {
    newpic.setpixel(i, row, pic.blurpixel(i, row));
  }
}

void PicLibrary::rowblur(string filename) {
  auto pair = internalstorage.findposition(filename);
  PicNode *picnode = pair.second;

  if (picnode != nullptr) {
    Picture pic = picnode->getpic();
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
    /*auto blurfinish = high_resolution_clock::now();
    auto rowblurduration = duration_cast<milliseconds>(blurfinish -
     blurstart);
    cout << filename + ": row blur's execution time = " <<
     rowblurduration.count() << " milliseconds." << endl;*/

    picnode->setpic(newpic);
  } else {
    cerr <<
         "error: could not blur image from internal picture storage because " +
         filename + " doesn't exist." << endl;
  }

  internalstorage.unlockpicnodes(pair);
}

void columnblurthread(Picture pic, Picture newpic, int column) {
  for (int i = 0; i < pic.getheight(); i++) {
    newpic.setpixel(column, i, pic.blurpixel(column, i));
  }
}

void PicLibrary::columnblur(string filename) {
  auto pair = internalstorage.findposition(filename);
  PicNode *picnode = pair.second;

  if (picnode != nullptr) {
    Picture pic = picnode->getpic();
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
    auto columnblurduration = duration_cast<milliseconds>(blurfinish -
     blurstart);
    cout << filename + ": column blur's execution time = " <<
     columnblurduration.count() << " milliseconds." << endl;*/

    picnode->setpic(newpic);
  } else {
    cerr <<
         "error: could not blur image from internal picture storage because " +
         filename + " doesn't exist." << endl;
  }

  internalstorage.unlockpicnodes(pair);
}

void sectorblurthread(Picture pic, Picture newpic, int sectorsize, int xstart,
                      int ystart) {
  int xend = xstart + (pic.getwidth()) / sectorsize;
  int yend = ystart + (pic.getheight()) / sectorsize;
  for (int i = xstart; i < xend; i++) {
    for (int j = ystart; j < yend; j++) {
      newpic.setpixel(i, j, pic.blurpixel(i, j));
    }
  }
}

/* Divides the image into (sectorsize)^2 equal sectors */
void PicLibrary::sectorblur(string filename, int sectorsize) {
  auto pair = internalstorage.findposition(filename);
  PicNode *picnode = pair.second;

  if (picnode != nullptr) {
    Picture pic = picnode->getpic();
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);
    vector<thread> sectorthreads;

    /*auto blurstart = high_resolution_clock::now();*/
    for (int j = 0; j < picwidth; j += (picwidth) / sectorsize) {
      for (int k = 0; k < picheight; k += (picheight) / sectorsize) {
        sectorthreads.emplace_back(sectorblurthread, pic, newpic, sectorsize, j,
                                   k);
      }
    }
    for (int i = 0; i < sectorthreads.size(); i++) {
      sectorthreads[i].join();
    }
    /*auto blurfinish = high_resolution_clock::now();
    auto sectorblurduration = duration_cast<milliseconds>(blurfinish -
     blurstart);
    cout << filename + ": sector blur's execution time = " <<
     sectorblurduration.count() << " milliseconds." << endl;*/

    picnode->setpic(newpic);
  } else {
    cerr <<
         "error: could not blur image from internal picture storage because " +
         filename + " doesn't exist." << endl;
  }

  internalstorage.unlockpicnodes(pair);
}

void pixelblurthread(Picture pic, Picture newpic, int x, int y) {
  newpic.setpixel(x, y, pic.blurpixel(x, y));
}

void PicLibrary::pixelblur(string filename) {
  auto pair = internalstorage.findposition(filename);
  PicNode *picnode = pair.second;
  // PicNode* picnode = internalstorage.findpicnode(filename);
  if (picnode != nullptr) {
    Picture pic = picnode->getpic();
    int picwidth = pic.getwidth();
    int picheight = pic.getheight();
    Picture newpic = Picture(picwidth, picheight);
    vector<thread> pixelthreads;

    /*auto blurstart = high_resolution_clock::now();*/
    for (int j = 0; j < picwidth; j++) {
      for (int k = 0; k < picheight; k++) {
        pixelthreads.emplace_back(pixelblurthread, pic, newpic, j, k);
      }
    }
    for (int i = 0; i < pixelthreads.size(); i++) {
      pixelthreads[i].join();
    }
    /*auto blurfinish = high_resolution_clock::now();
    auto pixelblurduration = duration_cast<milliseconds>(blurfinish -
     blurstart);
    cout << filename + ": pixel blur's execution time = " <<
     pixelblurduration.count() << " milliseconds." << endl;*/

    picnode->setpic(newpic);
  } else {
    cerr <<
         "error: could not blur image from internal picture storage because " +
         filename + " doesn't exist." << endl;
  }

  internalstorage.unlockpicnodes(pair);
}