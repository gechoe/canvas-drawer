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
   enum PrimitiveType {UNDEFINED, LINES, TRIANGLES, RECTANGLES, CIRCLES, POINT};
   enum DrawFill {OUTLINE, FILL};
  
   class vertexPos {
      public:
         int xLoc;
         int yLoc;
         Pixel color;
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
         void draw(DrawFill type);

         void lineColor(vertexPos locA, vertexPos locB);//, vertexPos currLoc);
         void triangleColor(float alphaA, float betaB, float gammaG, vertexPos locA, vertexPos locB, vertexPos locC);

         void lineWidth(int lineWidthSize);
         void drawline(vertexPos locA, vertexPos locB);
         void drawline_low(vertexPos locA, vertexPos locB);
         void drawline_high(vertexPos locA, vertexPos locB);

         void drawTriangle(vertexPos locA, vertexPos locB, vertexPos locC);
         void fragmenting(vertexPos locA, vertexPos locB, vertexPos locC);
         void drawRectangle(vertexPos centerPos, int widthLen, int heightLen);
         void widthLength(int w);
         void heightLength(int h);
         void drawCircle(vertexPos centerPos, int radiusLen);
         void radiusLength(int r);
         void drawPoint(vertexPos centerPos);

         float f01(vertexPos loc0, vertexPos loc1, vertexPos locCurr);
         float f12(vertexPos loc1, vertexPos loc2, vertexPos locCurr);
         float f20(vertexPos loc2, vertexPos loc0, vertexPos locCurr);

         void shareSide(vertexPos loc1, vertexPos loc2);

         void end();

         // Specifiy a vertex at raster position (x,y)
         // x corresponds to the column; y to the row
         void vertex(int x, int y);

         void fragmented(bool f);

         // Specify a color. Color components are in range [0,255]
         void color(unsigned char r, unsigned char g, unsigned char b);

         // Fill the canvas with the given background color
         void background(unsigned char r, unsigned char g, unsigned char b);

         // Fill the canvas with the given background color
         void background(Pixel bottomColor, Pixel topColor);

      private:
         Image _canvas;
         Pixel vertColor;
         PrimitiveType drawType;
         DrawFill fillType = FILL;
         vertexPos pointCurr;
         vector<vertexPos> sharedVertex;
         vector<int> widths;
         vector<int> heights;
         vector<int> radiuses;
         int rad, lineSize = 1;
         int fragment = 0;
         vector<vertexPos> vertices;
         vector<vertexPos> circVertices;
      };
}

#endif