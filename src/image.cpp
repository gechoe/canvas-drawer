/**
 * blobilism.cpp
 * Author: Grace Choe
 * Base Code Author: Aline Normoyle
 * Date: 2/1/2023
 * 
 * Description:
 * This program implements a class for loading and saving image files. It
 * implements various operations and filters for images. The operations and
 * filters are: flippig horizontally, resizing, gamma correcting, grayscaling,
 * blending, getting a subimage, and replacing.
 */

#include "image.h"

#include <cstdlib>
#include <cassert>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace agl {
//Memory cleanup in other functions
void Image::clean() {
  if (stbiAlloc == false) {
    delete[] pic;
    pic = NULL;
  } else {
    STBI_FREE(pic);
    pic = NULL;
  }
}

// Default constructor
Image::Image() {
  widthW = 20;
  heightH = 20;

  this->clean();
  pic = new char[widthW * heightH * num_chan];
}

// Constructor
Image::Image(int width, int height)  {
  widthW = width;
  heightH = height;

  this->clean();
  pic = new char[widthW * heightH * num_chan];
}

// Copy constructor
Image::Image(const Image& orig) {
  widthW = orig.widthW;
  heightH = orig.heightH;

  this->clean();
  pic = new char[widthW * heightH * num_chan];

  for (int i = 0; i < (widthW * heightH * num_chan); i++) {
    pic[i] = orig.pic[i];
  }
}

// Assignment operator
Image& Image::operator=(const Image& orig) {
  if (&orig == this) {
    return *this;
  }

  widthW = orig.widthW;
  heightH  = orig.heightH;

  this->clean();
  pic = new char[widthW * heightH * num_chan];

  for (int i = 0; i < (widthW * heightH * num_chan); i++) {
    pic[i] = orig.pic[i];
  }

  return *this;
}

// Destructor
Image::~Image() {
  this->clean();
}

// width
int Image::width() const { 
  return widthW;
}

// height
int Image::height() const {
  return heightH ;
}

// data
char* Image::data() const {
  return pic;
}

// set
void Image::set(int width, int height, unsigned char* data) {
}

// load
// Loads the file with the given name. Returns true if successful; False otherwise.
bool Image::load(const std::string& filename, bool flip) {
  stbi_set_flip_vertically_on_load(flip);

  this->clean();
  pic = (char*)stbi_load(filename.c_str(), &widthW, &heightH, &num_chan, des_chan);
  
  stbiAlloc = true;

  if (pic == NULL) {
    printf("Error in loading the image\n");
      return false;
  }

  // delete(pic);
  return true;
}

// save
// Saves the file with the given name. Returns true if successful; False otherwise.
bool Image::save(const std::string& filename, bool flip) const {
  stbi_flip_vertically_on_write(flip);
  return stbi_write_png(filename.c_str(), widthW, heightH, num_chan, pic,
    widthW * des_chan);
}

// get
// Gets a pixel (value range in [0,255]) at position (row, col).
Pixel Image::get(int row, int col) const {
  unsigned char red = pic[num_chan * (row * widthW + col)];
  unsigned char green =  pic[num_chan * (row * widthW + col) + 1];
  unsigned char blue = pic[num_chan * (row * widthW + col) + 2];

  // Shorthand for making a Pixel instance and adding r, g, and b colors to it.
  return Pixel{red, green, blue};
}

// set
// Sets a pixel (value range in [0,255]) at position (row, col).
void Image::set(int row, int col, const Pixel& color) {
  unsigned char red = color.r;
  unsigned char green = color.g;
  unsigned char blue = color.b;

  pic[num_chan * (row * widthW + col)] = red;
  pic[num_chan * (row * widthW + col) + 1] = green;
  pic[num_chan * (row * widthW + col) + 2] = blue;
}

Pixel Image::get(int i) const
{
  return Pixel{ 0, 0, 0 };
}

void Image::set(int i, const Pixel& c)
{
}

// resize
// Returns a copy of the imaged resized to the given width and height.
Image Image::resize(int w, int h) const {
  Image result(w, h);
  int locR, locC;
  float proportionR, proportionC;

  // for loop to resize according to calculations with proportions taken into
  // account.
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      proportionR = (i / ((float)(h - 1))) * (float)(heightH - 1);
      proportionC = (j / ((float)(w - 1))) * (float)(widthW - 1);
      locR = floor(proportionR);
      locC = floor(proportionC);

      Pixel pix = get(locR, locC);
      result.set(i, j, pix);
    }
  }

  return result;
}

// flipHorizontal
// Returns a copy of the image flipped along the horizontal middle axis.
Image Image::flipHorizontal() const {
  Image result(widthW, heightH);
  int changingW = widthW - 1;

  // for loop to flip image, for loop of width then height to make the flipping
  // horizontally by pixels easier.
  // Flipped by switching the ends first then switching and getting closer to
  // the middle then repeating.
  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW / 2; j++) {
      Pixel pix = get(j, i);
      Pixel otherPix = get(changingW, i);
      result.set(j, i, otherPix);
      result.set(changingW, i, pix);
      changingW--;
    }
    changingW = widthW - 1;
  }

  return result;
}

Image Image::flipVertical() const {
  Image result(widthW, heightH);
  int changingH = heightH - 1;

  for (int i = 0; i < widthW; i++) {
    for (int j = 0; j < heightH / 2; j++) {
      Pixel pix = get(i, j);
      Pixel otherPix = get(i, changingH);
      result.set(i, j, otherPix);
      result.set(i, changingH, pix);
      changingH--;
    }
    changingH = heightH - 1;
  }

  return result;
}

Image Image::rotate90() const {
  Image result(heightH, widthW);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);
      result.set(j, i, pix);
    }
  }
  
  int changingW = result.width() - 1;

  for (int i = 0; i < result.height(); i++) {
    for (int j = 0; j < result.width() / 2; j++) {
      Pixel pix = result.get(j, i);
      Pixel otherPix = result.get(changingW, i);
      result.set(j, i, otherPix);
      result.set(changingW, i, pix);
      changingW--;
    }
    changingW = result.width() - 1;
  }

  return result;
}

// subimage
// Returns a sub-image with top,left corner at (x,y) and width and height.
Image Image::subimage(int startx, int starty, int w, int h) const {
  Image sub(w, h);
  int locX, locY;

  // for loop to get subimage within bounds.
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      locX = i + startx;
      locY = j + starty;

      Pixel pix = get(locX, locY);
      sub.set(i, j, pix);
    }
  }
   
  return sub;
}

// replace
// Replaces the block of pixels starting at (row, col) with the image. Should
// not assume image fits on this image.
void Image::replace(const Image& image, int startx, int starty) {
  // Calculates where the foreground image will end within the background image.
  int imageHLimit = image.height() + startx;
  int imageWLimit = image.width() + starty;
  int k, m;

  // for loop to start from where to start replacing.
  // if statement to make sure pixels are replaced within background image's
  // boundaries.
  for (int i = startx; i < imageHLimit; i++) {
    for (int j = starty; j < imageWLimit; j++) {
      if ((i < heightH) && (j < widthW)) {
        k = i - startx;
        m = j - starty;
        Pixel pix = image.get(k, m);
        set(i, j, pix);
      }
    }
  }
}

Image Image::swirl() const {
  Image result(widthW, heightH);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);
      pix.r = pix.g;
      pix.g = pix.b;
      pix.b = pix.r;
         
      result.set(i, j, pix);
    }
  }

  return result;
}

Image Image::add(const Image& other) const {
  Image result(widthW, heightH);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel thisPix = get(i, j);
      Pixel otherPix = other.get(i, j);
      Pixel pix;
      pix.r = thisPix.r + otherPix.r;
      pix.g = thisPix.g + otherPix.g;
      pix.b = thisPix.b + otherPix.b;

      result.set(i, j, pix);
    }
  }

  return result;
}

Image Image::subtract(const Image& other) const {
  Image result(widthW, heightH);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel thisPix = get(i, j);
      Pixel otherPix = other.get(i, j);
      Pixel pix;
      pix.r = thisPix.r - otherPix.r;
      pix.g = thisPix.g - otherPix.g;
      pix.b = thisPix.b - otherPix.b;

      result.set(i, j, pix);
    }
  }
  
  return result;
}

Image Image::multiply(const Image& other) const {
  Image result(widthW, heightH);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel thisPix = get(i, j);
      Pixel otherPix = other.get(i, j);
      Pixel pix;
      pix.r = thisPix.r * otherPix.r;
      pix.g = thisPix.g * otherPix.g;
      pix.b = thisPix.b * otherPix.b;

      result.set(i, j, pix);
    }
  }
   
  return result;
}

Image Image::difference(const Image& other) const {
  Image result(widthW, heightH);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel thisPix = get(i, j);
      Pixel otherPix = other.get(i, j);
      Pixel pix;
      pix.r = abs(thisPix.r - otherPix.r);
      pix.g = abs(thisPix.g - otherPix.g);
      pix.b = abs(thisPix.b - otherPix.b);

      result.set(i, j, pix);
    }
  }
  
  return result;
}

Image Image::lightest(const Image& other) const {
  Image result(widthW, heightH);

  // for loop for alpha blend by using alpha blend equation.
  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel thisPix = get(i, j);
      Pixel otherPix = other.get(i, j);
      Pixel pix;
      pix.r = fmax(thisPix.r, otherPix.r);
      pix.g = fmax(thisPix.g, otherPix.g);
      pix.b = fmax(thisPix.b, otherPix.b);

      result.set(i, j, pix);
    }
  }
  
  return result;
}

Image Image::darkest(const Image& other) const {
  Image result(widthW, heightH);

  // for loop for alpha blend by using alpha blend equation.
  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel thisPix = get(i, j);
      Pixel otherPix = other.get(i, j);
      Pixel pix;
      pix.r = fmin(thisPix.r, otherPix.r);
      pix.g = fmin(thisPix.g, otherPix.g);
      pix.b = fmin(thisPix.b, otherPix.b);

      result.set(i, j, pix);
    }
  }
  
  return result;
}

// gammaCorrect
// Returns a copy of this image with the given gamma correction factor applied
// to it.
Image Image::gammaCorrect(float gamma) const {
  Image result(widthW, heightH);

  // for loop calculates gamma correction using gamma correction equation.
  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);

      // Finds the gamma exponent value/
      float gammaExp = 1 / gamma;

      // Uses gamma as a correction factor for red, green, and blue.
      float gammaRed = pow(((float)pix.r / 255), gammaExp);
      float gammaGreen = pow(((float)pix.g / 255), gammaExp);
      float gammaBlue = pow(((float)pix.b / 255), gammaExp);

      // Scales back to pixel color range for pixel intensity by multiplying by
      // max intensity (255).
      pix.r = 255 * gammaRed;
      pix.g = 255 * gammaGreen;
      pix.b = 255 * gammaBlue;
         
      result.set(i, j, pix);
    }
  }
 
  return result;
}

// alphaBlend
// Returns a copy of this image with the other image blended with it by factor
// alpha. E.g. result.pixel = this.pixel * (1 - alpha) + other.pixel * alpha.
// Assumes other and this have the same dimensions.
Image Image::alphaBlend(const Image& other, float alpha) const {
  Image result(widthW, heightH);

  // for loop for alpha blend by using alpha blend equation.
  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel thisPix = get(i, j);
      Pixel otherPix = other.get(i, j);
      Pixel pix;
      pix.r = ((float)thisPix.r * (1 - alpha)) + ((float)otherPix.r * alpha);
      pix.g = ((float)thisPix.g * (1 - alpha)) + ((float)otherPix.g * alpha);
      pix.b = ((float)thisPix.b * (1 - alpha)) + ((float)otherPix.b * alpha);

      result.set(i, j, pix);
    }
  }

  return result;
}

Image Image::invert() const {
  Image image(widthW, heightH);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);
      pix.r = (unsigned char)(255 - (int)pix.r);
      pix.g = (unsigned char)(255 - (int)pix.b);
      pix.b = (unsigned char)(255 - (int)pix.g);
         
      image.set(i, j, pix);
    }
  }
   
  return image;
}

// grayscale
// Returns a copy of this image as a grayscale image.
Image Image::grayscale() const {
  Image result(widthW, heightH);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);

      float average = (pix.r * 0.3) + (pix.g * 0.59) + (pix.b * 0.11);
      pix.r = average, pix.g = average, pix.b = average;
         
      result.set(i, j, pix);
    }
  }
   
  return result;
}

// extractRed
// Extract only the red color channel
Image Image::extractRed() const {
  Image result(widthW, heightH);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);
      pix.r = 0;
         
      result.set(i, j, pix);
    }
  }
   
  return result;
}

// extractGreen
// Extract only the green color channel
Image Image::extractGreen() const {
  Image result(widthW, heightH);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);
      pix.g = 0;
         
      result.set(i, j, pix);
    }
  }
   
  return result;
}

// extractBlue
// Extract only the blue color channel
Image Image::extractBlue() const {
  Image result(widthW, heightH);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);
      pix.b = 0;
         
      result.set(i, j, pix);
    }
  }
   
  return result;
}

// onlyRed
// Keeps only the red color channel
Image Image::onlyRed() const {
  Image result(widthW, heightH);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);
      pix.g = 0, pix.b = 0;
         
      result.set(i, j, pix);
    }
  }
   
  return result;
}

// onlyGreen
// Keep only the green color channel
Image Image::onlyGreen() const {
  Image result(widthW, heightH);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);
      pix.r = 0, pix.b = 0;
         
      result.set(i, j, pix);
    }
  }
   
  return result;
}

// onlyBlue
// Extract only the blue color channel
Image Image::onlyBlue() const {
  Image result(widthW, heightH);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);
      pix.r = 0, pix.g = 0;
         
      result.set(i, j, pix);
    }
  }
   
  return result;
}

Image Image::border() const {
  int borderSize;

  if (widthW < heightH) {
    borderSize = widthW * 0.1;
  } else {
    borderSize = heightH * 0.1;
  }

  if (borderSize == 0) {
    borderSize = 1;
  }

  int newWidth = widthW + (borderSize * 2);
  int newHeight = heightH + (borderSize * 2);
  Image image(newWidth, newHeight);

  int heightLimit = newHeight - borderSize;
  int widthLimit = newWidth - borderSize;

  for (int i = 0; i < newHeight ; i++) {
    for (int j = 0; j < newWidth; j++)  {
      if ((i <= borderSize) || (j <= borderSize) || (j >= widthLimit) ||
        (i >= heightLimit)) {
        Pixel border;
        border.r = 0, border.g = 100, border.b = 100;
        image.set(i, j, border);
      }
    }
  }

  for (int i = 0; i < heightH ; i++) {
    for (int j = 0; j < widthW; j++)  {
      Pixel pix = get(i, j);
      image.set(i + borderSize, j + borderSize, pix);
    }
  }

  return image;
}

Image Image::colorJitter(int size) const {
  Image image(0, 0);

  return image;
}

Image Image::grainyEffect() const {
  Image image(widthW, heightH);

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);
      int random = rand() % 100;

      pix.r = (255 - (float)pix.r) * ((float)random / 100) + (float)pix.r;
      pix.g = (255 - (float)pix.g) * ((float)random / 100) + (float)pix.g;
      pix.b = (255 - (float)pix.b) * ((float)random / 100) + (float)pix.b;

      image.set(i, j, pix);
    }
  }

  return image;
}

Image Image::tvcolors() const {
  Image image(widthW, heightH);
  int division = widthW / 8;
  int bottomDivision = heightH - (2 * (heightH / 6));

  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);

      if (((j < division) && (i < bottomDivision)) || ((j > (7 * division)) &&
        (i > bottomDivision))) {
        pix.r = (255 - (float)pix.r) * 0.10 + (float)pix.r;
        pix.g = (255 - (float)pix.g) * 0.10 + (float)pix.g;
        pix.b = (255 - (float)pix.b) * 0.10 + (float)pix.b;
      } else if (((j < (2 * division)) && (i < bottomDivision)) ||
        ((j > (6 * division)) && (i > bottomDivision))) {
        pix.r = (255 - (float)pix.r) * 0.10 + (float)pix.r;
        pix.g = (255 - (float)pix.g) * 0.10 + (float)pix.g;
        pix.b = (255 - (float)pix.b) * 0.10;
      } else if (((j <  (3 * division)) && (i < bottomDivision)) ||
        ((j > (5 * division)) && (i > bottomDivision))) {
        pix.r = (255 - (float)pix.r) * 0.10;
        pix.g = (255 - (float)pix.g) * 0.10 + (float)pix.g;
        pix.b = (255 - (float)pix.b) * 0.10 + (float)pix.b;
      } else if (((j <  (4 * division)) && (i < bottomDivision)) ||
        ((j > (4 * division)) && (i > bottomDivision))) {
        pix.r = (255 - (float)pix.r) * 0.10;
        pix.g = (255 - (float)pix.g) * 0.10 + (float)pix.g;
        pix.b = (255 - (float)pix.b) * 0.10;
      } else if (((j <  (5 * division)) && (i < bottomDivision)) ||
        ((j > (3 * division)) && (i > bottomDivision))) {
        pix.r = (255 - (float)pix.r) * 0.10 + (float)pix.r;
        pix.g = (255 - (float)pix.g) * 0.10;
        pix.b = (255 - (float)pix.b) * 0.10 + (float)pix.b;
      } else if (((j <  (6 * division)) && (i < bottomDivision)) ||
        ((j > (2 * division)) && (i > bottomDivision))) {
        pix.r = (255 - (float)pix.r) * 0.10 + (float)pix.r;
        pix.g = (255 - (float)pix.g) * 0.10 + (float)pix.g;
        pix.b = (255 - (float)pix.b) * 0.10 + (float)pix.b;
      } else if (((j <  (7 * division)) && (i < bottomDivision)) ||
        ((j > (division)) && (i > bottomDivision))) {
        pix.r = (255 - (float)pix.r) * 0.10 + (float)pix.r;
        pix.g = (255 - (float)pix.g) * 0.10;
        pix.b = (255 - (float)pix.b) * 0.10;
      } else {
        pix.r = (255 - (float)pix.r) * 0.10;
        pix.g = (255 - (float)pix.g) * 0.10;
        pix.b = (255 - (float)pix.b) * 0.10 + (float)pix.b;
      }

      image.set(i, j, pix);
    }
  }

  return image;
}

Image Image::bitmap(int size) const {
  Image image(0, 0);
   
  return image;
}

Image Image::redDyePrint() const {
  Image result(widthW, heightH);

  // for loop for alpha blend by using alpha blend equation.
  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);

      pix.r = (255 - (float)pix.r) * 0.10 + (float)pix.r;
      pix.g = (255 - (float)pix.g) * 0.10;
      pix.b = (255 - (float)pix.b) * 0.10;

      result.set(i, j, pix);
    }
  }

  return result;
}

Image Image::greenDyePrint() const {
  Image result(widthW, heightH);

  // for loop for alpha blend by using alpha blend equation.
  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);

      pix.r = (255 - (float)pix.r) * 0.10;
      pix.g = (255 - (float)pix.g) * 0.10 + (float)pix.g;
      pix.b = (255 - (float)pix.b) * 0.10;

      result.set(i, j, pix);
    }
  }

  return result;
}

Image Image::blueDyePrint() const {
  Image result(widthW, heightH);

  // for loop for alpha blend by using alpha blend equation.
  for (int i = 0; i < heightH; i++) {
    for (int j = 0; j < widthW; j++) {
      Pixel pix = get(i, j);

      pix.r = (255 - (float)pix.r) * 0.10;
      pix.g = (255 - (float)pix.g) * 0.10;
      pix.b = (255 - (float)pix.b) * 0.10 + (float)pix.b;

      result.set(i, j, pix);
    }
  }

  return result;
}

void Image::fill(const Pixel& c) {


}

}  // namespace agl
