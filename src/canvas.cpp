#include "canvas.h"
#include "math.h"
#include <cassert>
// #include <vector>
#include <algorithm>

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

void Canvas::lineColor(vertexPos pA, vertexPos pB) {//}, vertexPos pCurr) {
   Pixel pixColor;
   float dist1 = sqrt(pow((pointCurr.xLoc - pA.xLoc), 2) + pow((pointCurr.yLoc - pA.yLoc), 2));
   float dist2 = sqrt(pow((pB.xLoc - pA.xLoc), 2) + pow((pB.yLoc - pA.yLoc), 2));
   float t = dist1 / dist2;

   pixColor.r = ((float)pA.color.r * (1.0 - t)) + ((float)pB.color.r * t);
   pixColor.g = ((float)pA.color.g * (1.0 - t)) + ((float)pB.color.g * t);
   pixColor.b = ((float)pA.color.b * (1.0 - t)) + ((float)pB.color.b * t);

   pointCurr.color = pixColor;
}

void Canvas::triangleColor(float alpha, float beta, float gamma, vertexPos pA, vertexPos pB, vertexPos pC) {
   Pixel triPix;
   triPix.r = (alpha * pA.color.r) + (beta * pB.color.r) + (gamma * pC.color.r);
   triPix.g = (alpha * pA.color.g) + (beta * pB.color.g) + (gamma * pC.color.g);
   triPix.b = (alpha * pA.color.b) + (beta * pB.color.b) + (gamma * pC.color.b);

   pointCurr.color = triPix;
}

void Canvas::lineWidth(int lw) {
   lineSize = lw;
}

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
      if ((y < _canvas.width()) && (x < _canvas.height())) {
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
      if ((y < _canvas.width()) && (x < _canvas.height())) {
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

void Canvas::drawTriangle(vertexPos pointA, vertexPos pointB, vertexPos pointC) {
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
            if ((y < _canvas.width()) && (x < _canvas.height())) {
               triangleColor(alpha, beta, gamma, pointA, pointB, pointC);
               _canvas.set(pointCurr.yLoc, pointCurr.xLoc, pointCurr.color);
            }
         }
      }
   }
}

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
               if ((y < _canvas.width()) && (x < _canvas.height())) {
                  triangleColor(alpha, beta, gamma, pointA, pointB, pointC);
                  _canvas.set(pointCurr.yLoc, pointCurr.xLoc, pointCurr.color);
               }
            }
         }
      }
   }
}

float Canvas::f01(vertexPos point0, vertexPos point1, vertexPos curr) {
   float result;
   float part1 = (point0.yLoc - point1.yLoc) * pointCurr.xLoc;
   float part2 = (point1.xLoc - point0.xLoc) * pointCurr.yLoc;
   result = part1 + part2 + (point0.xLoc * point1.yLoc) - (point1.xLoc * point0.yLoc);
   return result;
}

float Canvas::f12(vertexPos point1, vertexPos point2, vertexPos curr) {
   float result;
   float part1 = (point1.yLoc - point2.yLoc) * curr.xLoc;
   float part2 = (point2.xLoc - point1.xLoc) * curr.yLoc;
   result = part1 + part2 + (point1.xLoc * point2.yLoc) - (point2.xLoc * point1.yLoc);
   return result;
}

float Canvas::f20(vertexPos point2, vertexPos point0, vertexPos curr) {
   float result;
   float part1 = (point2.yLoc - point0.yLoc) * curr.xLoc;
   float part2 = (point0.xLoc - point2.xLoc) * curr.yLoc;
   result = part1 + part2 + (point2.xLoc * point0.yLoc) - (point0.xLoc * point2.yLoc);
   return result;
}

void Canvas::drawRectangle(vertexPos centerPos) {
   int halfW = wid / 2;
   int halfH = heig / 2;

   vertexPos rect1a = {centerPos.xLoc + halfW, centerPos.yLoc + halfH, centerPos.color};
   vertexPos rect1b = {centerPos.xLoc - halfW, centerPos.yLoc + halfH, centerPos.color};
   vertexPos rect1c = {centerPos.xLoc - halfW, centerPos.yLoc - halfH, centerPos.color};
   drawTriangle(rect1a, rect1b, rect1c);

   vertexPos rect2a = {centerPos.xLoc + halfW, centerPos.yLoc + halfH, centerPos.color};
   vertexPos rect2b = {centerPos.xLoc - halfW, centerPos.yLoc - halfH, centerPos.color};
   vertexPos rect2c = {centerPos.xLoc + halfW, centerPos.yLoc - halfH, centerPos.color};
   drawTriangle(rect2a, rect2b, rect2c);

   // if (fragment == true) {
   //    rect1a.color = {(unsigned char)(rect1a.color.r - 30), (unsigned char)(rect1a.color.g - 30), (unsigned char)(rect1a.color.b - 30)};
   //    rect1b.color = {(unsigned char)(rect1b.color.r - 60), (unsigned char)(rect1b.color.g - 60), (unsigned char)(rect1b.color.b - 60)};
   //    fragmenting(rect1a, rect1b, rect1c);
   //    rect2a.color = {(unsigned char)(rect2a.color.r - 30), (unsigned char)(rect2a.color.g - 30), (unsigned char)(rect2a.color.b - 30)};
   //    rect2b.color = {(unsigned char)(rect2b.color.r - 60), (unsigned char)(rect2b.color.g - 60), (unsigned char)(rect2b.color.b - 60)};
   //    fragmenting(rect2a, rect2b, rect2c);
   // } else {
   //    drawTriangle(rect1a, rect1b, rect1c);
   //    drawTriangle(rect2a, rect2b, rect2c);
   // }
}

void Canvas::widthLength(int w) {
   wid = w;
}

void Canvas::heightLength(int h) {
   heig = h;
}

void Canvas::fragmented(bool frag) {
   fragment = frag;
}

void Canvas::end() {
   if (drawType == PrimitiveType::LINES) {
      for (int i = 0; i + 2 <= vertices.size(); i += 2) {
         vertexPos v1 = vertices.at(i);
         vertexPos v2 = vertices.at(i + 1);
         drawline(v1, v2);
      }
      vertices.clear();
      lineSize = 0;
   } else if (drawType == PrimitiveType::TRIANGLES) {
      for (int i = 0; i + 3 <= vertices.size(); i += 3) {
         vertexPos v1 = vertices.at(i);
         vertexPos v2 = vertices.at(i + 1);
         vertexPos v3 = vertices.at(i + 2);

         // if (fragment) {
         //    fragmenting(v1, v2, v3);
         //    std::cout << fragment << std::endl;
         // } else {
         //    std::cout << fragment << std::endl;
            drawTriangle(v1, v2, v3);
         // }
      }

      // if (shareSide.size() >= 2) {
      //    // Put down how to color the shared line

      // }
      vertices.clear();
   } else if (drawType == PrimitiveType::RECTANGLES) {
      vertexPos center = vertices.at(0);
      drawRectangle(center);

      // if (sharedVertex.size() >= 2) {

      // }

      vertices.clear();
   }

   // fragment = false;
}

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

void Canvas::color(unsigned char r, unsigned char g, unsigned char b) {
   vertColor = {r, g, b};
}

void Canvas::background(unsigned char r, unsigned char g, unsigned char b) {
   for (int i = 0; i < _canvas.height(); i++) {
      for (int j = 0; j < _canvas.width(); j++) {
         Pixel pix = {r, g, b};
         _canvas.set(i, j, pix);
      }
   }
}

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