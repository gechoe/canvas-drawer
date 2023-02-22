#include "canvas.h"
#include "math.h"
#include <cassert>
// #include <vector>
// #include <algorithm>

using namespace std;
using namespace agl;

Canvas::Canvas(int w, int h) : _canvas(w, h) {
   _canvas = Image(w, h);
}

Canvas::~Canvas() {
   // delete _canvas;
   // _canvas = NULL;
}

void Canvas::save(const std::string& filename) {
   _canvas.save(filename);
}

void Canvas::begin(PrimitiveType type) {
   drawType = type;
}

void Canvas::lineColor(vertexLoc pA, Pixel colorA, vertexLoc pB, Pixel colorB, vertexLoc pCurr) {
   float dist1 = sqrt(pow((pCurr.xLoc - pA.xLoc), 2) + pow((pCurr.yLoc - pA.yLoc), 2));
   float dist2 = sqrt(pow((pB.xLoc - pA.xLoc), 2) + pow((pB.yLoc - pA.yLoc), 2));
   float t = dist1 / dist2;

   pixColor.r = ((float)colorA.r * (1.0 - t)) + ((float)colorB.r * t);
   pixColor.g = ((float)colorA.g * (1.0 - t)) + ((float)colorB.g * t);
   pixColor.b = ((float)colorA.b * (1.0 - t)) + ((float)colorB.b * t);

   _canvas.set(pCurr.yLoc, pCurr.xLoc, pixColor);
}

void Canvas::triangleColor(float alpha, Pixel colorA, float beta, Pixel colorB, float gamma, Pixel colorC, vertexLoc currPoint) {
   Pixel triPix;
   triPix.r = (alpha * colorA.r) + (beta * colorB.r) + (gamma * colorC.r);
   triPix.g = (alpha * colorA.g) + (beta * colorB.g) + (gamma * colorC.g);
   triPix.b = (alpha * colorA.b) + (beta * colorB.b) + (gamma * colorC.b);
   _canvas.set(currPoint.yLoc, currPoint.xLoc, triPix);
}

void Canvas::drawline(vertexLoc pointA, vertexLoc pointB) {
   float W = pointB.xLoc - pointA.xLoc;
   float H = pointB.yLoc - pointA.yLoc;

   if (abs(H) < abs(W)) {
      if (pointA.xLoc > pointB.xLoc) {
         vertexLoc temp = pointA;
         pointA = pointB;
         pointB = temp;
      }
      drawline_low(pointA, pointB);
   } else {
      if (pointA.yLoc > pointB.yLoc) {
         vertexLoc temp = pointA;
         pointA = pointB;
         pointB = temp;
      }
      drawline_high(pointA, pointB);
   }
}

void Canvas::drawline_low(vertexLoc pointA, vertexLoc pointB) {
   int y = pointA.yLoc;
   float W = pointB.xLoc - pointA.xLoc;
   float H = pointB.yLoc - pointA.yLoc;
   float dy = 1;

   if (H < 0) {
      dy = -1;
      H = (-1) * H;
   }

   float F = (2 * H) - W;

   for (int x = pointA.xLoc; x <= pointB.xLoc; x++) {
      if (pixMultiColor.size() == 2) {
         color1 = pixMultiColor.at(0);
         color2 = pixMultiColor.at(1);
         vertexLoc pointCurr = {x, y};
         lineColor(pointA, color1, pointB, color2, pointCurr);
      } else {
         _canvas.set(y, x, pixColor);
      }

      if (F > 0) {
         y += dy;
         F += 2 * (H - W);
      } else {
         F += 2 * H;
      }
   }
}

void Canvas::drawline_high(vertexLoc pointA, vertexLoc pointB) {
   int x = pointA.xLoc;
   float W = pointB.xLoc - pointA.xLoc;
   float H = pointB.yLoc - pointA.yLoc;
   float dx = 1;

   if (W < 0) {
      dx = -1;
      W = (-1) * W;
   }

   float F = (2 * W) - H;

   for (int y = pointA.yLoc; y <= pointB.yLoc; y++) {
      if ((y < _canvas.width()) && (x < _canvas.height())) {
         if (pixMultiColor.size() == 2) {
            color1 = pixMultiColor.at(0);
            color2 = pixMultiColor.at(1);
            vertexLoc pointCurr = {x, y};

            lineColor(pointA, color1, pointB, color2, pointCurr);
         } else {
            _canvas.set(y, x, pixColor);
         }
      }

      if (F > 0) {
         x += dx;
         F += 2 * (W - H);
      } else {
         F += 2 * W;
      }
   }
}

void Canvas::drawTriangle(vertexLoc pointA, vertexLoc pointB, vertexLoc pointC) {
   float alpha, beta, gamma;

   int xMin = min(pointA.xLoc, pointB.xLoc);
   xMin = min(xMin, pointC.xLoc);

   int xMax = max(pointA.xLoc, pointB.xLoc);
   xMax = max(xMax, pointC.xLoc);
   
   int yMin = min(pointA.yLoc, pointB.yLoc);
   yMin = min(yMin, pointC.yLoc);

   int yMax = max(pointA.yLoc, pointB.yLoc);
   yMax = max(yMax, pointC.yLoc);

   for (int y = yMin; y <= yMax; y++) {
      for (int x = xMin; x <= xMax; x++) {
         vertexLoc curr = {x, y};
         alpha = f12(pointB, pointC, curr) / f12(pointB, pointC, pointA);
         beta = f12(pointC, pointA, curr) / f12(pointC, pointA, pointB);
         gamma = f12(pointA, pointB, curr) / f12(pointA, pointB, pointC);

         if (alpha > 0 && beta > 0 && gamma > 0) {
            if (pixMultiColor.size() == 3) {
               color1 = pixMultiColor.at(0);
               color2 = pixMultiColor.at(1);
               color3 = pixMultiColor.at(2);

               triangleColor(alpha, color1, beta, color2, gamma, color3, curr);
            } else {
               _canvas.set(y, x, pixColor);
            }
         }
      }
   }
}

float Canvas::f01(vertexLoc point0, vertexLoc point1, vertexLoc pointCurr) {
   float result;
   float part1 = (point0.yLoc - point1.yLoc) * pointCurr.xLoc;
   float part2 = (point1.xLoc - point0.xLoc) * pointCurr.yLoc;
   result = part1 + part2 + (point0.xLoc * point1.yLoc) - (point1.xLoc * point0.yLoc);
   return result;
}

float Canvas::f12(vertexLoc point1, vertexLoc point2, vertexLoc pointCurr) {
   float result;
   float part1 = (point1.yLoc - point2.yLoc) * pointCurr.xLoc;
   float part2 = (point2.xLoc - point1.xLoc) * pointCurr.yLoc;
   result = part1 + part2 + (point1.xLoc * point2.yLoc) - (point2.xLoc * point1.yLoc);
   return result;
}

float Canvas::f20(vertexLoc point2, vertexLoc point0, vertexLoc pointCurr) {
   float result;
   float part1 = (point2.yLoc - point0.yLoc) * pointCurr.xLoc;
   float part2 = (point0.xLoc - point2.xLoc) * pointCurr.yLoc;
   result = part1 + part2 + (point2.xLoc * point0.yLoc) - (point0.xLoc * point2.yLoc);
   return result;
}

void Canvas::end() {
   if (drawType == PrimitiveType::LINES) {
      for (int i = 0; i + 2 <= vertices.size(); i += 2) {
         vertexLoc v1 = vertices.at(i);
         vertexLoc v2 = vertices.at(i + 1);
         drawline(v1, v2);
      }
      vertices.clear();
      pixMultiColor.clear();
   } else if (drawType == PrimitiveType::TRIANGLES) {
      for (int i = 0; i + 3 <= vertices.size(); i += 3) {
         vertexLoc v1 = vertices.at(i);
         vertexLoc v2 = vertices.at(i + 1);
         vertexLoc v3 = vertices.at(i + 2);
         drawTriangle(v1, v2, v3);
      }

      // if (shareSide == 2) {
      //    // Put down how to color the shared line
      // }

      shareSide = 0;
      vertices.clear();
      pixMultiColor.clear();
   }
}

void Canvas::vertex(int x, int y) {
   vertexLoc vertex = {x, y};

   // int *iter;
   // iter = find(vertices.begin(), vertices.end(), vertex);
   // if (iter != vertices.end()) {
   //    shareSide++;
   // }

   vertices.push_back(vertex);

   // if ((drawType == PrimitiveType::LINES) && (vertices.size() == 2)) {
   //    vertexLoc v1 = vertices.at(0);
   //    vertexLoc v2 = vertices.at(1);
   //    drawline(v1, v2);
   //    vertices.clear();
   //    pixMultiColor.clear();
   // } else if ((drawType == PrimitiveType::TRIANGLES) && (vertices.size() == 3)) {
   //    vertexLoc v1 = vertices.at(0);
   //    vertexLoc v2 = vertices.at(1);
   //    vertexLoc v3 = vertices.at(2);
      
   //    drawTriangle(v1, v2, v3);
   //    vertices.clear();
   //    pixMultiColor.clear();
   // }
}

void Canvas::color(unsigned char r, unsigned char g, unsigned char b) {
   pixColor = {r, g, b};
   pixMultiColor.push_back(pixColor);
}

void Canvas::background(unsigned char r, unsigned char g, unsigned char b) {
   for (int i = 0; i < _canvas.height(); i++) {
      for (int j = 0; j < _canvas.width(); j++) {
         Pixel pix;
         pix.r = r;
         pix.g = g;
         pix.b = b;
         _canvas.set(i, j, pix);
      }
   }
}

