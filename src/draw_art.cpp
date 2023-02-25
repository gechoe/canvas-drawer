#include <iostream>
#include <cstdlib>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>
#include "canvas.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
   Canvas drawer(640, 380);
   int amount;
   srand(time(0));
   // your code here
   // Example 1: User being able to gradient the background
   drawer.background({110, 48, 125}, {48, 71, 125});
   drawer.save("gradient-background.png");

   // Example 2: User being able to change line width, black and white version
   drawer.background({0, 0, 0}, {255, 255, 255});
   int parts = 380 / 10;
   for (int i = 0; i < 9; i++) {
      drawer.begin(LINES);
      drawer.color(i * 31, i * 31, i * 31);
      drawer.lineWidth(i * 2 + 1);
      drawer.vertex(0, (i + 1) * parts);
      drawer.vertex(640, (i + 1) * parts);
      drawer.end();
   }

   drawer.save("diff-widths-bw.png");

   // Example 3: User being able to change line width, colorful version
   drawer.background({126, 174, 109}, {171, 109, 174});
   for (int i = 0; i < 9; i++) {
      drawer.begin(LINES);
      drawer.color(i * 25, i * 10, i * 31);
      drawer.lineWidth(i * 2 + 1);
      drawer.vertex(0, (i + 1) * parts);
      drawer.vertex(640, (i + 1) * parts);
      drawer.end();
   }

   drawer.save("diff-widths-color.png");

   // Example 4: User being able to create triangles.
   // Makes randomized triangles of randomized colors and sizes at randomized locations.
   drawer.background(0,0,0);

   amount = 100;
   for (int i = 0; i < amount; i++) {
      drawer.begin(TRIANGLES);
      int range, randX, randY;

      int randR = 50;
      if (i > (amount / 4) * 3) {
         range = 25 - 5 + 1;
         randR = (rand() % range) + 5;
      } else if (i > (amount / 4) * 2) {
         range = 50 - 25 + 1;
         randR = (rand() % range) + 25;
      } else if (i > (amount / 4)) {
         range = 150 - 75 + 1;
         randR = (rand() % range) + 75;
      } else {
         range = 400 - 250 + 1;
         randR = (rand() % range) + 250;
      }
      
      range = 640 - (randR * 2) + 1;
      randX = (rand() % range) + (randR * 2);
      range = 380 - (randR * 2) + 1;
      randY = (rand() % range) + (randR * 2);

      int range1 = 255 - (i) + 1;

      int red = (rand() % range1) + i;
      int green = (rand() % range1) + i;
      int blue = (rand() % range1) + i;
      drawer.color(red, green, blue);
      
      float radian90 = 90 * (M_PI / 180);
      drawer.vertex(randR * cos(radian90) + randX, randR * sin(radian90) + randY);

      red = (rand() % range1) + i;
      green = (rand() % range1) + i;
      blue = (rand() % range1) + i;

      drawer.color(red, green, blue);
      float radian210 = 210 * (M_PI / 180);
      drawer.vertex(randR * cos(radian210) + randX, randR * sin(radian210) + randY);

      red = (rand() % range1) + i;
      green = (rand() % range1) + i;
      blue = (rand() % range1) + i;

      drawer.color(red, green, blue);
      float radian330 = 330 * (M_PI / 180);
      drawer.vertex(randR * cos(radian330) + randX, randR * sin(radian330) + randY);
      drawer.end();
   }

   drawer.save("triangle-randomized.png");

   // Example 5: User being able to create an outline of triangles, circles, and rectangles.
   // Makes randomized triangles, circles, and rectangles of randomized colors and sizes at randomized locations.
   drawer.background(255, 255, 255);

   amount = 25;
   for (int i = 0; i < amount; i++) {
      drawer.begin(TRIANGLES);
      drawer.lineWidth(2);
      drawer.draw(OUTLINE);
      int range;

      int randR = 50;
      if (i > (amount / 4) * 3) {
         range = 25 - 5 + 1;
         randR = (rand() % range) + 5;
      } else if (i > (amount / 4) * 2) {
         range = 50 - 25 + 1;
         randR = (rand() % range) + 25;
      } else if (i > (amount / 4)) {
         range = 150 - 75 + 1;
         randR = (rand() % range) + 75;
      } else {
         range = 400 - 250 + 1;
         randR = (rand() % range) + 250;
      }
      
      range = 640 - (randR * 2) + 1;
      int triX = (rand() % range) + (randR * 2);
      range = 380 - (randR * 2) + 1;
      int triY = (rand() % range) + (randR * 2);

      int rangeR = 255 - (i) + 1;
      int red = (rand() % rangeR) + (i);
      int rangeG = 102 - (i) + 1;
      int green = (rand() % rangeG) + (i);
      int rangeB = 102 - (i) + 1;
      int blue = (rand() % rangeB) + (i);
      drawer.color(red, green, blue);
      
      float radian90 = 90 * (M_PI / 180);
      drawer.vertex(randR * cos(radian90) + triX, randR * sin(radian90) + triY);

      float radian210 = 210 * (M_PI / 180);
      drawer.vertex(randR * cos(radian210) + triX, randR * sin(radian210) + triY);

      float radian330 = 330 * (M_PI / 180);
      drawer.vertex(randR * cos(radian330) + triX, randR * sin(radian330) + triY);
      drawer.end();

      drawer.begin(RECTANGLES);
      drawer.lineWidth(2);
      drawer.draw(OUTLINE);
      int w, h; 
      if (i > (amount / 4) * 3) {
         range = 25 - 5 + 1;
         w = (rand() % range) + 5;
         h = (rand() % range) + 5;
      } else if (i > (amount / 4) * 2) {
         range = 50 - 25 + 1;
         w = (rand() % range) + 25;
         h = (rand() % range) + 25;
      } else if (i > (amount / 4)) {
         range = 150 - 75 + 1;
         w = (rand() % range) + 75;
         h = (rand() % range) + 75;
      } else {
         range = 400 - 250 + 1;
         w = (rand() % range) + 250;
         h = (rand() % range) + 250;
      }

      range = 640 - (w * 2) + 1;
      int rectX = (rand() % range) + (w * 2);
      range = 380 - (w * 2) + 1;
      int rectY = (rand() % range) + (w * 2);

      rangeR = 178 - (i) + 1;
      red = (rand() % rangeR) + (i);
      rangeG = 255 - (i) + 1;
      green = (rand() % rangeG) + (i);
      rangeB = 102 - (i) + 1;
      blue = (rand() % rangeB) + (i);
      drawer.color(red, green, blue);

      drawer.widthLength(w);
      drawer.heightLength(h);

      drawer.vertex(rectX, rectY);
      drawer.end();

      drawer.begin(CIRCLES);
      drawer.lineWidth(2);
      drawer.draw(OUTLINE);

      int r; 
      if (i > (amount / 4) * 3) {
         range = 25 - 5 + 1;
         r = (rand() % range) + 5;
         drawer.radiusLength(r);
      } else if (i > (amount / 4) * 2) {
         range = 50 - 25 + 1;
         r = (rand() % range) + 25;
         drawer.radiusLength(r);
      } else if (i > (amount / 4)) {
         range = 150 - 75 + 1;
         r = (rand() % range) + 75;
         drawer.radiusLength(r);
      } else {
         range = 400 - 250 + 1;
         r = (rand() % range) + 250;
         drawer.radiusLength(r);
      }

      range = 640 - (r * 2) + 1;
      int circX = (rand() % range) + (r * 2);
      range = 380 - (r * 2) + 1;
      int circY = (rand() % range) + (r * 2);

      // int range3 = 255 - (r) + 1;
      rangeR = 102 - (i) + 1;
      red = (rand() % rangeR) + (i);
      rangeG = 178 - (i) + 1;
      green = (rand() % rangeG) + (i);
      rangeB = 255 - (i) + 1;
      blue = (rand() % rangeB) + (i);
      drawer.color(red, green, blue);

      drawer.vertex(circX, circY);

      drawer.end();
   }

   drawer.save("outline-shapes-colorful.png");

   // Example 6: User being able to create filled triangles, circles, and rectangles.
   // Makes randomized triangles, circles, and rectangles of randomized colors and sizes at randomized locations.
   drawer.background(255, 255, 255);

   amount = 25;
   for (int i = 0; i < amount; i++) {
      drawer.begin(TRIANGLES);
      drawer.lineWidth(2);
      int range;

      int randR = 50;
      if (i > (amount / 4) * 3) {
         range = 25 - 5 + 1;
         randR = (rand() % range) + 5;
      } else if (i > (amount / 4) * 2) {
         range = 50 - 25 + 1;
         randR = (rand() % range) + 25;
      } else if (i > (amount / 4)) {
         range = 150 - 75 + 1;
         randR = (rand() % range) + 75;
      } else {
         range = 400 - 250 + 1;
         randR = (rand() % range) + 250;
      }
      
      // range = 640 - randR + 1;
      range = 640 - (randR * 2) + 1;
      int triX = (rand() % range) + (randR * 2);
      range = 380 - (randR * 2) + 1;
      int triY = (rand() % range) + (randR * 2);

      int rangeR = 255 - (i) + 1;
      int red = (rand() % rangeR) + (i);
      int rangeG = 102 - (i) + 1;
      int green = (rand() % rangeG) + (i);
      int rangeB = 102 - (i) + 1;
      int blue = (rand() % rangeB) + (i);
      drawer.color(red, green, blue);
      
      float radian90 = 90 * (M_PI / 180);
      drawer.vertex(randR * cos(radian90) + triX, randR * sin(radian90) + triY);

      float radian210 = 210 * (M_PI / 180);
      drawer.vertex(randR * cos(radian210) + triX, randR * sin(radian210) + triY);

      float radian330 = 330 * (M_PI / 180);
      drawer.vertex(randR * cos(radian330) + triX, randR * sin(radian330) + triY);
      drawer.end();

      drawer.begin(RECTANGLES);
      drawer.lineWidth(2);
      int w, h; 
      if (i > (amount / 4) * 3) {
         range = 25 - 5 + 1;
         w = (rand() % range) + 5;
         h = (rand() % range) + 5;
      } else if (i > (amount / 4) * 2) {
         range = 50 - 25 + 1;
         w = (rand() % range) + 25;
         h = (rand() % range) + 25;
      } else if (i > (amount / 4)) {
         range = 150 - 75 + 1;
         w = (rand() % range) + 75;
         h = (rand() % range) + 75;
      } else {
         range = 400 - 250 + 1;
         w = (rand() % range) + 250;
         h = (rand() % range) + 250;
      }

      range = 640 - (w * 2) + 1;
      int rectX = (rand() % range) + (w * 2);
      range = 380 - (w * 2) + 1;
      int rectY = (rand() % range) + (w * 2);

      rangeR = 178 - (i) + 1;
      red = (rand() % rangeR) + (i);
      rangeG = 255 - (i) + 1;
      green = (rand() % rangeG) + (i);
      rangeB = 102 - (i) + 1;
      blue = (rand() % rangeB) + (i);
      drawer.color(red, green, blue);

      drawer.widthLength(w);
      drawer.heightLength(h);

      drawer.vertex(rectX, rectY);
      drawer.end();

      drawer.begin(CIRCLES);
      drawer.lineWidth(2);

      int r; 
      if (i > (amount / 4) * 3) {
         range = 25 - 5 + 1;
         r = (rand() % range) + 5;
         drawer.radiusLength(r);
      } else if (i > (amount / 4) * 2) {
         range = 50 - 25 + 1;
         r = (rand() % range) + 25;
         drawer.radiusLength(r);
      } else if (i > (amount / 4)) {
         range = 150 - 75 + 1;
         r = (rand() % range) + 75;
         drawer.radiusLength(r);
      } else {
         range = 400 - 250 + 1;
         r = (rand() % range) + 250;
         drawer.radiusLength(r);
      }

      range = 640 - (r * 2) + 1;
      int circX = (rand() % range) + (r * 2);
      range = 380 - (r * 2) + 1;
      int circY = (rand() % range) + (r * 2);

      // int range3 = 255 - (r) + 1;
      rangeR = 102 - (i) + 1;
      red = (rand() % rangeR) + (i);
      rangeG = 178 - (i) + 1;
      green = (rand() % rangeG) + (i);
      rangeB = 255 - (i) + 1;
      blue = (rand() % rangeB) + (i);
      drawer.color(red, green, blue);

      drawer.vertex(circX, circY);

      drawer.end();
   }

   drawer.save("fill-shapes-colorful.png");

   return 0;
}