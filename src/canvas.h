/*-----------------------------------------------
 * Author: Grace Choe
 * Date: 2/14/2023
 * Description:
 ----------------------------------------------*/

#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include "image.h"
using namespace std;

namespace agl
{
   enum PrimitiveType {UNDEFINED, LINES, TRIANGLES};
   
   class vertexLoc {
      public:
         int xLoc;
         int yLoc;
   };

   class Canvas {
      public:
         Canvas(int w, int h);
         virtual ~Canvas();

         // Save to file
         void save(const std::string& filename);

         // Draw primitives with a given type (either LINES or TRIANGLES)
         // For example, the following draws a red line followed by a green line
         // begin(LINES);
         //    color(255,0,0);
         //    vertex(0,0);
         //    vertex(100,0);
         //    color(0,255,0);
         //    vertex(0, 0);
         //    vertex(0,100);
         // end();
         void begin(PrimitiveType type);

         void lineColor(vertexLoc locA, Pixel colA, vertexLoc locB, Pixel colB, vertexLoc currLoc);
         void triangleColor(float alphaA, Pixel colA, float betaB, Pixel colB, float gammaG, Pixel colC, vertexLoc currLoc);

         void drawline(vertexLoc locA, vertexLoc locB);
         void drawline_low(vertexLoc locA, vertexLoc locB);
         void drawline_high(vertexLoc locA, vertexLoc locB);

         void drawTriangle(vertexLoc locA, vertexLoc locB, vertexLoc locC);

         float f01(vertexLoc loc0, vertexLoc loc1, vertexLoc locCurr);
         float f12(vertexLoc loc1, vertexLoc loc2, vertexLoc locCurr);
         float f20(vertexLoc loc2, vertexLoc loc0, vertexLoc locCurr);

         void end();

         // Specifiy a vertex at raster position (x,y)
         // x corresponds to the column; y to the row
         void vertex(int x, int y);

         // Specify a color. Color components are in range [0,255]
         void color(unsigned char r, unsigned char g, unsigned char b);

         // Fill the canvas with the given background color
         void background(unsigned char r, unsigned char g, unsigned char b);

      private:
         Image _canvas;
         Pixel pixColor, color1, color2, color3;
         PrimitiveType drawType;
         // vertexLoc vert;
         vector<vertexLoc> vertices;
         vector<Pixel> pixMultiColor;
      };
}

#endif


