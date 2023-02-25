/**
 * canvas.cpp
 * Author: Grace Choe
 * Base Code Author: Aline Normoyle
 * Date: 2/14/2023
 * 
 * Description:
 * This program implements a class for drawing lines and simple shapes. Some being drawing points, lines, triangles, circles, and rectangles. It also employs interpolation on these shapes where gradients or one fill color can be applied. The background can also be gradient to create fun canvas images.
 */

#include "canvas.h"
#include "math.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

using namespace std;
using namespace agl;

// Constructor
Canvas::Canvas(int w, int h) : _canvas(w, h) {
}

// Destructor
Canvas::~Canvas() {
}

// Save
void Canvas::save(const std::string& filename) {
   _canvas.save(filename);
}

// Begin, to designate what shape to draw
void Canvas::begin(PrimitiveType type) {
   drawType = type;
}

// Draw, to designate fill type (just outlining the shape or filling it in)
void Canvas::draw(DrawFill type) {
   fillType = type;
}

// lineColor
// Designates what color each pixel in the line is.
void Canvas::lineColor(vertexPos pA, vertexPos pB) {
   Pixel pixColor;
   float dist1 = sqrt(pow((pointCurr.xLoc - pA.xLoc), 2) + pow((pointCurr.yLoc - pA.yLoc), 2));
   float dist2 = sqrt(pow((pB.xLoc - pA.xLoc), 2) + pow((pB.yLoc - pA.yLoc), 2));
   float t = dist1 / dist2;

   pixColor.r = ((float)pA.color.r * (1.0 - t)) + ((float)pB.color.r * t);
   pixColor.g = ((float)pA.color.g * (1.0 - t)) + ((float)pB.color.g * t);
   pixColor.b = ((float)pA.color.b * (1.0 - t)) + ((float)pB.color.b * t);

   pointCurr.color = pixColor;
}

// triangleColor
// Designates what color each pixel in the triangle is.
void Canvas::triangleColor(float alpha, float beta, float gamma, vertexPos pA, vertexPos pB, vertexPos pC) {
   Pixel triPix;
   triPix.r = (alpha * pA.color.r) + (beta * pB.color.r) + (gamma * pC.color.r);
   triPix.g = (alpha * pA.color.g) + (beta * pB.color.g) + (gamma * pC.color.g);
   triPix.b = (alpha * pA.color.b) + (beta * pB.color.b) + (gamma * pC.color.b);

   pointCurr.color = triPix;
}

// lineWidth
// Allows user to change line width.
void Canvas::lineWidth(int lw) {
   lineSize = lw;
}

// drawline
// Starts calculating to draw the line between two points.
void Canvas::drawline(vertexPos pointA, vertexPos pointB) {
   float W = pointB.xLoc - pointA.xLoc;
   float H = pointB.yLoc - pointA.yLoc;

   if (abs(H) < abs(W)) {
      if (pointA.xLoc > pointB.xLoc) {
         vertexPos temp = pointA;
         pointA = pointB;
         pointB = temp;
      }
      drawline_low(pointA, pointB);
   } else {
      if (pointA.yLoc > pointB.yLoc) {
         vertexPos temp = pointA;
         pointA = pointB;
         pointB = temp;
      }
      drawline_high(pointA, pointB);
   }
}

// drawline_low
// One type for drawing a line, calculates which pixel to color in.
void Canvas::drawline_low(vertexPos pointA, vertexPos pointB) {
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
      if ((x < _canvas.width()) && (y < _canvas.height())) {
         pointCurr = {x, y};
         lineColor(pointA, pointB);
         
         _canvas.set(pointCurr.yLoc, pointCurr.xLoc, pointCurr.color);

         if (lineSize > 1) {
            int bothsides = 2;
            for (int i = lineSize; i > 1; i--) {
               if ((i % 2 == 1) && (pointCurr.xLoc + (bothsides / 2) < _canvas.height())) {
                  _canvas.set(pointCurr.yLoc, pointCurr.xLoc + (bothsides / 2), pointCurr.color);
                  bothsides++;
               } else if (pointCurr.xLoc + (bothsides / 2) > 0) { 
                  _canvas.set(pointCurr.yLoc, pointCurr.xLoc - (bothsides / 2), pointCurr.color);
                  bothsides++;
               }
            }
         }
         if (lineSize > 1) {
            int bothsides = 2;
            for (int i = lineSize; i > 1; i--) {
               if ((i % 2 == 1) && (pointCurr.yLoc + (bothsides / 2) < _canvas.width())) {
                  _canvas.set(pointCurr.yLoc + (bothsides / 2), pointCurr.xLoc, pointCurr.color);
                  bothsides++;
               } else if (pointCurr.yLoc - (bothsides / 2) >= 0) {
                  _canvas.set(pointCurr.yLoc - (bothsides / 2), pointCurr.xLoc, pointCurr.color);
                  bothsides++;
               }
            }
         }
      }

      if (F > 0) {
         y += dy;
         F += 2 * (H - W);
      } else {
         F += 2 * H;
      }
   }
}

// drawline_high
// Another type for drawing a line, calculates which pixel to color in.
void Canvas::drawline_high(vertexPos pointA, vertexPos pointB) {
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
      if ((x < _canvas.width()) && (y < _canvas.height())) {
         pointCurr = {x, y};
         lineColor(pointA, pointB);
         _canvas.set(pointCurr.yLoc, pointCurr.xLoc, pointCurr.color);
         if (lineSize > 1) {
            int bothsides = 2;
            for (int i = lineSize; i > 1; i--) {
               if ((i % 2 == 1) && (pointCurr.xLoc + (bothsides / 2) < _canvas.height())) {
                  _canvas.set(pointCurr.yLoc, pointCurr.xLoc + (bothsides / 2), pointCurr.color);
                  bothsides++;
               } else if (pointCurr.xLoc - (bothsides / 2) >= 0) { 
                  _canvas.set(pointCurr.yLoc, pointCurr.xLoc - (bothsides / 2), pointCurr.color);
                  bothsides++;
               }
            }
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

// drawTriangle
// Starts calculating to draw the triangle using three points.
void Canvas::drawTriangle(vertexPos pointA, vertexPos pointB, vertexPos pointC) {
   if (fillType == OUTLINE) {
      drawline(pointA, pointB);
      drawline(pointB, pointC);
      drawline(pointC, pointA);
   } else {
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
            pointCurr = {x, y};
            // alpha = f12(pointB, pointC, pointCurr) / f12(pointB, pointC, pointA);
            // beta = f20(pointC, pointA, pointCurr) / f20(pointC, pointA, pointB);
            // gamma = f01(pointA, pointB, pointCurr) / f01(pointA, pointB, pointC);
            alpha = f12(pointB, pointC, pointCurr) / f12(pointB, pointC, pointA);
            beta = f12(pointC, pointA, pointCurr) / f12(pointC, pointA, pointB);
            gamma = f12(pointA, pointB, pointCurr) / f12(pointA, pointB, pointC);

            if (alpha > 0 && beta > 0 && gamma > 0) {
               if ((x < _canvas.width()) && (y < _canvas.height())) {
                  triangleColor(alpha, beta, gamma, pointA, pointB, pointC);
                  _canvas.set(pointCurr.yLoc, pointCurr.xLoc, pointCurr.color);
               }
            }
         }
      }

      if (sharedVertex.size() > 0) {
         vertexPos v1 = sharedVertex.at(0);
         v1.xLoc--;
         v1.yLoc--;
         vertexPos v2 = sharedVertex.at(1);
         v2.xLoc++;
         v2.yLoc++;
         drawline(v1, v2);
      }
   }
}

// fragmenting
// Fragments the colors in a triangle to have more triangle gradiations within it.
void Canvas::fragmenting(vertexPos pointA, vertexPos pointB, vertexPos pointC) {
   float alpha, beta, gamma;

   float falpha = f12(pointB, pointC, pointA);
   float fbeta = f20(pointC, pointA, pointB);
   float fgamma = f01(pointA, pointB, pointC);

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
         pointCurr = {x, y};
         alpha = f12(pointB, pointC, pointCurr) / falpha;
         beta = f20(pointC, pointA, pointCurr) / fbeta;
         gamma = f01(pointA, pointB, pointCurr) / fgamma;

         float aComp = f12(pointB, pointC, {-1, -1}) * falpha;
         float bComp = f20(pointC, pointA, {-1, -1}) * fbeta;
         float cComp = f01(pointA, pointB, {-1, -1}) * fgamma;

         if (alpha >= 0 && beta >= 0 && gamma >= 0) {
            if (((alpha > 0) || (aComp > 0)) && ((beta > 0) || (bComp > 0)) && ((gamma > 0) || (cComp > 0))) {
               if ((x < _canvas.width()) && (y < _canvas.height())) {
                  triangleColor(alpha, beta, gamma, pointA, pointB, pointC);
                  _canvas.set(pointCurr.yLoc, pointCurr.xLoc, pointCurr.color);
               }
            }
         }
      }
   }
}

// f01
// Equation, used in calculations for drawing the triangle.
float Canvas::f01(vertexPos point0, vertexPos point1, vertexPos curr) {
   float result;
   float part1 = (point0.yLoc - point1.yLoc) * pointCurr.xLoc;
   float part2 = (point1.xLoc - point0.xLoc) * pointCurr.yLoc;
   result = part1 + part2 + (point0.xLoc * point1.yLoc) - (point1.xLoc * point0.yLoc);
   return result;
}

// f12
// Equation, used in calculations for drawing the triangle.
float Canvas::f12(vertexPos point1, vertexPos point2, vertexPos curr) {
   float result;
   float part1 = (point1.yLoc - point2.yLoc) * curr.xLoc;
   float part2 = (point2.xLoc - point1.xLoc) * curr.yLoc;
   result = part1 + part2 + (point1.xLoc * point2.yLoc) - (point2.xLoc * point1.yLoc);
   return result;
}

// f20
// Equation, used in calculations for drawing the triangle.
float Canvas::f20(vertexPos point2, vertexPos point0, vertexPos curr) {
   float result;
   float part1 = (point2.yLoc - point0.yLoc) * curr.xLoc;
   float part2 = (point0.xLoc - point2.xLoc) * curr.yLoc;
   result = part1 + part2 + (point2.xLoc * point0.yLoc) - (point0.xLoc * point2.yLoc);
   return result;
}

// drawRectangle
// Starts calculating to draw the rectangle using its center point and its width and height.
void Canvas::drawRectangle(vertexPos centerPos, int wid, int heig) {
   int halfW = wid / 2;
   int halfH = heig / 2;

   vertexPos rect1a = {centerPos.xLoc + halfW, centerPos.yLoc + halfH, centerPos.color};
   vertexPos rect1b = {centerPos.xLoc - halfW, centerPos.yLoc + halfH, centerPos.color};
   vertexPos rect1c = {centerPos.xLoc - halfW, centerPos.yLoc - halfH, centerPos.color};

   vertexPos rect2a = {centerPos.xLoc + halfW, centerPos.yLoc + halfH, centerPos.color};
   vertexPos rect2b = {centerPos.xLoc - halfW, centerPos.yLoc - halfH, centerPos.color};
   vertexPos rect2c = {centerPos.xLoc + halfW, centerPos.yLoc - halfH, centerPos.color};

   drawline(rect1a, rect1b);
   drawline(rect1b, rect1c);
   drawline(rect1c, rect2c);
   drawline(rect2c, rect1a);
   
   if (fillType == FILL) {
      drawTriangle(rect1a, rect1b, rect1c);
      drawTriangle(rect2a, rect2b, rect2c);
      drawline(rect1a, rect1c);
   }
}

// widthLength
// Gets the width, for width calculations when making a shape.
void Canvas::widthLength(int w) {
   widths.push_back(w);
}

// heightLength
// Gets the height, for height calculations when making a shape.
void Canvas::heightLength(int h) {
   heights.push_back(h);
}

// drawCircle
// Starts calculating to draw the circle using its center point and its radius.
void Canvas::drawCircle(vertexPos centerPos, int rad) {
   int sides = 36;
   int degree = 360 / sides;//2 * M_PI / sides;//360 / sides;

   for (int i = 1; i < sides; i++) {
      int currDegree = degree * i;
      float radians = currDegree * (M_PI / 180);//2 * M_PI * i / sides;//currDegree * M_PI / 180;

      int x = (rad * cos(radians)) + centerPos.xLoc;
      int y = (rad * sin(radians)) + centerPos.yLoc;
      vertexPos circVert = {x, y, centerPos.color};
      circVertices.push_back(circVert);
   }

   for (int j = 0; j + 2 <= circVertices.size(); j++) {
      vertexPos circ1 = circVertices.at(j);
      vertexPos circ2 = circVertices.at(j + 1);

      drawline(circ1, circ2);

      if (fillType == FILL) {
         drawTriangle(circ1, circ2, centerPos);
         drawline(centerPos, circ1);
         drawline(centerPos, circ2);
      }
   }

   vertexPos circFirst = circVertices.at(0);
   int lastPos = circVertices.size() - 1;
   vertexPos circLast = circVertices.at(lastPos);

   drawline(circFirst, circLast);

   if (fillType == FILL) {
      drawTriangle(circFirst, centerPos, circLast);
   }

   circVertices.clear();
}

// radiusLength
// Gets the radius, for radius calculations when making a shape.
void Canvas::radiusLength(int r) {
   radiuses.push_back(r);
}

// drawPoint
// Starts calculating to draw the point using its center point.
void Canvas::drawPoint(vertexPos centerPos) {
   if (lineSize == 1) { // handles creating a point that is one pixel in size
      _canvas.set(centerPos.xLoc, centerPos.yLoc, centerPos.color);
   } else { // handles creating a point that is more than one pixel in size.
      rad = lineSize / 2;
      drawCircle(centerPos, lineSize);
   }
}

// fragmented
// Checks if user requested for fragmenting a triangle shape
void Canvas::fragmented(bool frag) {
   if (frag == true) {
      fragment = 1;
   }
}

// end
// Prompts for all the shapes to be drawn based on the user's shape type input.
void Canvas::end() {
   if (drawType == PrimitiveType::LINES) { // for lines
      for (int i = 0; i + 2 <= vertices.size(); i += 2) {
         vertexPos v1 = vertices.at(i);
         vertexPos v2 = vertices.at(i + 1);
         drawline(v1, v2);
      }
      vertices.clear();
      lineSize = 0;
   } else if (drawType == PrimitiveType::TRIANGLES) { // for triangles
      for (int i = 0; i + 3 <= vertices.size(); i += 3) {
         vertexPos v1 = vertices.at(i);
         vertexPos v2 = vertices.at(i + 1);
         vertexPos v3 = vertices.at(i + 2);

         if (fragment == 0) {
            drawTriangle(v1, v2, v3);
         } else {
            fragmenting(v1, v2, v3);
         }
      }

      vertices.clear();
      fragment = 0;
   } else if (drawType == PrimitiveType::RECTANGLES) { // for rectangles
      for (int i = 0; i < vertices.size(); i++) {
         vertexPos vert = vertices.at(i);
         int w = widths.at(i);
         int h = heights.at(i);
         drawRectangle(vert, w, h);
      }

      vertices.clear();
      widths.clear();
      heights.clear();
   } else if (drawType == PrimitiveType::CIRCLES) { // for circles
      for (int i = 0; i < radiuses.size(); i++) {
         vertexPos vert = vertices.at(i);
         int r = radiuses.at(i);
         drawCircle(vert, r);
      }

      vertices.clear();
      radiuses.clear();
   } else if (drawType == PrimitiveType::POINT) { // for point
      vertexPos center = vertices.at(0);
      drawPoint(center);
      vertices.clear();
   }

   fillType = FILL;
   sharedVertex.clear();
}

// vertex
// Creates a vertex with a respective color and stores it within a vector.
void Canvas::vertex(int x, int y) {
   vertexPos vertex = {x, y, vertColor};

   for (vertexPos vert : vertices) {
      if ((vertex.xLoc == vert.xLoc) && (vertex.yLoc == vert.yLoc)) {
         sharedVertex.push_back(vertex);
         break;
      }
   }

   vertices.push_back(vertex);
}

// color
// Designates the color the user inputs.
void Canvas::color(unsigned char r, unsigned char g, unsigned char b) {
   vertColor = {r, g, b};
}

// background
// Makes the background color the color the user inputs. This method is just for one fill color.
void Canvas::background(unsigned char r, unsigned char g, unsigned char b) {
   for (int i = 0; i < _canvas.height(); i++) {
      for (int j = 0; j < _canvas.width(); j++) {
         Pixel pix = {r, g, b};
         _canvas.set(i, j, pix);
      }
   }
}

// background
// Same name as the other background method to allow user to either input one or two colors.
// Inputting two colors allows for background to have gradient colors.
void Canvas::background(Pixel bottom, Pixel top) {
   vertexPos botVert = {_canvas.height(), 0, bottom};
   vertexPos topVert = {0, 0, top};

   for (int i = 0; i < _canvas.height(); i++) {
      vertexPos currVert = {i, 0};
      Pixel pix;
      float dist1 = sqrt(pow((currVert.xLoc - botVert.xLoc), 2) + pow((currVert.yLoc - botVert.yLoc), 2));
      float dist2 = sqrt(pow((topVert.xLoc - botVert.xLoc), 2) + pow((topVert.yLoc - botVert.yLoc), 2));
      float t = dist1 / dist2;

      pix.r = ((float)botVert.color.r * (1.0 - t)) + ((float)topVert.color.r * t);
      pix.g = ((float)botVert.color.g * (1.0 - t)) + ((float)topVert.color.g * t);
      pix.b = ((float)botVert.color.b * (1.0 - t)) + ((float)topVert.color.b * t);

      for (int j = 0; j < _canvas.width(); j++) {
         _canvas.set(i, j, pix);
      }
   }
}