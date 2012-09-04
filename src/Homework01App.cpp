/**
 * @file Homework01App.cpp
 * CSE 274 - Fall 2012
 * My solution for HW01 Phase 01.
 *
 * @author Mike Marsden
 * @date 2012-09-01
 *
 * @note This file is (c) 2012. It is licensed under the 
 * CC BY 3.0 license (http://creativecommons.org/licenses/by/3.0/),
 * which means you are free to use, share, and remix it as long as you
 * give attribution. Commercial uses are allowed.
 *
 * @note This project satisfies goals A.1 (rectangle), A.2 (circle), B.1 (blur), E.2 (transparency),
 * E.5 (animation) and E.6 (mouse interaction)
 */
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Homework01App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings* setttings);

  private:
	Surface* mySurface_;

	int frame_number_;

	//Width and height of the screen
	static const int kAppWidth=800;
	static const int kAppHeight=600;
	static const int kTextureSize=1024;

	// Creates a basic Rectangle with starting point (x1, y1) and ending point (x2, y2) with color
	// c1 and c2. This satisfies Requirement A.1 (rectangle).
	void basicRectangle (uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u c1, Color8u c2);


	void redTintImage (uint8_t* pixels);

	// Creates a basic line with starting point (x1, y1) and ending point (x2, y2) with color c1.
	// This satisfies Requirement A.3 (line).
	void basicLine (uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u c1);

	// A helper method
	void Homework01App::setPixel(uint8_t* pixels, int xCoor, int yCoor, Color8u c1);

	// Creates a basic triangle with three points (x1, y1), (x2, y2), (x3, y3) with color c1.
	// This satisfies Requirement A.7 (triangle).
	void basicTriangle (uint8_t* pixels, int x1, int y1, int x2, int y2, int x3, int y3, Color8u c1);
};

void Homework01App::prepareSettings(Settings* settings) {
	(*settings).setWindowSize(kAppWidth, kAppHeight);
	(*settings).setResizable(false);
}

void Homework01App::basicRectangle(uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u fill, Color8u fill2) {
	//Determines the starting and ending coordinates for the rectangle
	int startx = (x1 < x2) ? x1 : x2;
	int endx = (x1 < x2) ? x2 : x1;
	int starty = (y1 < y2) ? y1 : y2;
	int endy = (y1 < y2) ? y2 : y1;
	
	//Boundary Checking, credit to Dr. Brinkman
	if (endx < 0) return;
	if (endy < 0) return;
	if (startx >= kAppWidth) return;
	if (starty >= kAppHeight) return;
	if (endx >= kAppWidth) endx = kAppWidth - 1;
	if (endy >= kAppHeight) endy = kAppHeight - 1;	

	int rectSize = (endx - startx) * (endy - starty);
	int currentPixel = 0;
	for (int y = starty; y <= endy; y++) {
		for (int x = startx; x <= endx; x++) {			
			if (currentPixel < (rectSize/2)) 
			{
				setPixel(pixels, x, y, fill);				
			}
			else if (currentPixel > (rectSize/2)) 
			{
				setPixel(pixels, x, y, fill2);			
			}
			//Middle pixel of rectangle will be red
			else
			{
				setPixel(pixels, x, y, Color8u(255,0,0));
			}
			currentPixel++;
		}
	}
}

void Homework01App::redTintImage(uint8_t* pixels) 
{
	for (int y = 0; y <= kTextureSize; y++) {
		for (int x = 0; x <= kTextureSize; x++) {	
			pixels[3*(x + y*kTextureSize)] = 255;
			//pixels[3*(x + y*kTextureSize)+1] = pixels[3*(x + y*kTextureSize)+1];
			//pixels[3*(x + y*kTextureSize)+2] = pixels[3*(x + y*kTextureSize)+2];		
		}
	}
}


void Homework01App::basicLine(uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u c1)
{
	// On this method I used the starting algorithm from this website:
	// http://www.codekeep.net/snippets/e39b2d9e-0843-4405-8e31-44e212ca1c45.aspx
	// and then rewrote section of the code to better fit the rest of the program

	// Determines the starting and ending coordinates for the line
	int startx = (x1 < x2) ? x1 : x2;
	int endx = (x1 < x2) ? x2 : x1;
	int starty = (y1 < y2) ? y1 : y2;
	int endy = (y1 < y2) ? y2 : y1;
		

	int F, x, y;
    int dy            = endy - starty;  // y-increment from p1 to p2
    int dx            = endx - endy;  // x-increment from p1 to p2
    int dy2           = (dy << 1);  // dy << 1 == 2*dy
    int dx2           = (dx << 1);
    int dy2_minus_dx2 = dy2 - dx2;  // precompute constant for speed up
    int dy2_plus_dx2  = dy2 + dx2;

	// Vertical Line
	if (startx == endx)
	{
        x = startx;
        y = starty;
        while (y <= endy)
        {
           setPixel(pixels, x, y, c1);
           y++;
        }
        return;
   }
    // Horizontal line
    else if (starty == endy)
    {
        x = startx;
        y = starty;

        while (x <= endx)
        {
            setPixel(pixels, x, y, c1);
            x++;
        }
        return;
    }
	
    if (dy >= 0)    
    {
        // Case 1: 0 <= m <= 1 (Original case)
        if (dy <= dx)   
        {
            F = dy2 - dx;    // initial F

            x = startx;
            y = starty;
            while (x <= endx)
            {
                setPixel(pixels, x, y, c1);	
                if (F <= 0)
                {
                    F += dy2;
                }
                else
                {
                    y++;
                    F += dy2_minus_dx2;
                }
                x++;
            }
        }
        // Case 2: 1 < m < INF (Mirror about y=x line
        // replace all dy by dx and dx by dy)
        else
        {
            F = dx2 - dy;    // initial F

            y = starty;
            x = startx;
            while (y <= endy)
            {
                setPixel(pixels, x, y, c1);
                if (F <= 0)
                {
                    F += dx2;
                }
                else
                {
                    x++;
                    F -= dy2_minus_dx2;
                }
                y++;
            }
        }
    }
	else    // m < 0
    {
        // Case 3: -1 <= m < 0 (Mirror about x-axis, replace all dy by -dy)
        if (dx >= -dy)
        {
            F = -dy2 - dx;    // initial F

            x = startx;
            y = starty;
            while (x <= endx)
            {
                setPixel(pixels, x, y, c1);
                if (F <= 0)
                {
                    F -= dy2;
                }
                else
                {
                    y--;
                    F -= dy2_plus_dx2;
                }
                x++;
            }
        }
        // Case 4: -INF < m < -1 (Mirror about x-axis and mirror 
        // about y=x line, replace all dx by -dy and dy by dx)
        else    
        {
            F = dx2 + dy;    // initial F

            y = starty;
            x = startx;
            while (y >= endy)
            {
                setPixel(pixels, x, y, c1);
                if (F <= 0)
                {
                    F += dx2;
                }
                else
                {
                    x++;
                    F += dy2_plus_dx2;
                }
                y--;
            }
        }
	}
}

void Homework01App::setPixel(uint8_t* pixels, int xCoor, int yCoor, Color8u c1)
{
	pixels[3*(xCoor + yCoor*kTextureSize)] = c1.r;
	pixels[3*(xCoor + yCoor*kTextureSize)+1] = c1.g;
	pixels[3*(xCoor + yCoor*kTextureSize)+2] = c1.b;

}


void Homework01App::basicTriangle (uint8_t* pixels, int x1, int y1, int x2, int y2, int x3, int y3, Color8u c1)
{
	basicLine(pixels, x1, y1, x2, y2, c1);
	basicLine(pixels, x2, y2, x3, y3, c1);
	basicLine(pixels, x1, y1, x3, y3, c1);
}

void Homework01App::setup()
{
	frame_number_=0;
	
	//This is the setup that everyone needs to do
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
}

void Homework01App::mouseDown( MouseEvent event )
{
	int xCoor = event.getX();
	int yCoor = event.getY();
	
}

void Homework01App::update()
{
	//Get our array of pixel information
	uint8_t* dataArray = (*mySurface_).getData();

	Color8u rectFill = Color8u(128,255,128);
	Color8u rectFill2 = Color8u(255,128,255);
	basicRectangle(dataArray, 100, 100, 400, 400, rectFill, rectFill2); 

	redTintImage(dataArray);

	Color8u lineColor = Color8u(0,0,0);
	basicLine(dataArray, 5, 5, 500, 300, lineColor);

	basicTriangle(dataArray, 500, 400, 500, 500, 600, 500, lineColor);

	//Only save the first frame of drawing as output, code snippet via Dr. Brinkman
	if(frame_number_ == 0){
		writeImage("marsdems.png",*mySurface_);
	}
	//keeps track of how many frames we have shown.
	frame_number_++;
}

void Homework01App::draw()
{
	gl::draw(*mySurface_); 
}

CINDER_APP_BASIC( Homework01App, RendererGl )
