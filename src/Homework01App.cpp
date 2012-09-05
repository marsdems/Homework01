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
	
	int pixelCount;
	int pixelCountBackwards;
	int movingSquareCount;
	int greenShade;
	//Width and height of the screen
	static const int kAppWidth=800;
	static const int kAppHeight=600;
	static const int kTextureSize=1024;

	// Creates a basic Rectangle with starting point (x1, y1) and ending point (x2, y2) with color
	// c1 and c2. This satisfies Requirement A.1 (rectangle).
	void makeRectangle (uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u c1, Color8u c2);

	// Creates a basic circle with center x, y, radius r, satisfies Requirement A.2 (circle)
	void makeCircle(uint8_t* pixels, int xCenter, int yCenter, int r, Color8u c1);

	// Creates an unappealing seafoamGreen tint on the window, satisfies Requirement A.6 (tint). 
	void makeDarkSeafoamGreenTint (uint8_t* pixels);

	// Creates a basic line with starting point (x1, y1) and ending point (x2, y2) with color c1.
	// This satisfies Requirement A.3 (line).
	void makeLine (uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u c1);

	// A helper method for setting the pixel color
	void Homework01App::setPixel(uint8_t* pixels, int xCoor, int yCoor, Color8u c1);

	// Creates a basic triangle with three points (x1, y1), (x2, y2), (x3, y3) with color c1.
	// This satisfies Requirement A.7 (triangle).
	void makeTriangle (uint8_t* pixels, int x1, int y1, int x2, int y2, int x3, int y3, Color8u c1);
};

void Homework01App::prepareSettings(Settings* settings) {
	(*settings).setWindowSize(kAppWidth, kAppHeight);
	(*settings).setResizable(false);
}

void Homework01App::makeRectangle(uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u fill, Color8u fill2) {
	
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

void Homework01App::makeCircle(uint8_t* pixels, int xCenter, int yCenter, int r, Color8u c1)
{
	for (int y = yCenter - r; y <= yCenter + r; y++)
	{
		for (int x = xCenter - r; x <= xCenter + r; x++)
		{
			int distanceFromCenter = (int)sqrt((double)((x-xCenter)*(x-xCenter) + (y-yCenter)*(y-yCenter)));
			if (distanceFromCenter <= r)
			{
				setPixel(pixels, x, y, c1);
			}
		}
	}
}

void Homework01App::makeLine(uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u c1)
{
	// On this method I used the starting algorithm from this website:
	// http://www.codekeep.net/snippets/e39b2d9e-0843-4405-8e31-44e212ca1c45.aspx
	// and then rewrote sections of the code to better fit the rest of the program

	// Determines the starting and ending coordinates for the line.
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
        // 0 <= m <= 1 
        if (dy <= dx)   
        {
            F = dy2 - dx;
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
        // 1 < m < INF 
        else
        {
            F = dx2 - dy;
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
	else   // m < 0
    {
        // -1 <= m < 0 
        if (dx >= -dy)
        {
            F = -dy2 - dx;    
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
        // -INF < m < -1 
        else    
        {
            F = dx2 + dy;
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


void Homework01App::makeTriangle (uint8_t* pixels, int x1, int y1, int x2, int y2, int x3, int y3, Color8u c1)
{
	makeLine(pixels, x1, y1, x2, y2, c1);
	makeLine(pixels, x2, y2, x3, y3, c1);
	makeLine(pixels, x1, y1, x3, y3, c1);
}

void Homework01App::setup()
{
	frame_number_=0;
	pixelCount = 0;
	pixelCountBackwards = kAppHeight;
	movingSquareCount = 1;
	greenShade = 1;
	//This is the setup that everyone needs to do
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
}

void Homework01App::mouseDown( MouseEvent event )
{	
	uint8_t* dataArray = (*mySurface_).getData();
	Color8u WHT = Color8u(255,255,255);
	Color8u RND = Color8u((1+rand() * 255),(1+rand() * 255),(1+rand() * 255));	
	int xLoc = event.getX();
	int yLoc = event.getY();

	//Sets background color to White
	//makeRectangle(dataArray, 0, 0, kTextureSize, kTextureSize, WHT, WHT);		

	int rectSize = (rand() % 3 + 1);
	if (rectSize == 1) 
	{
		makeRectangle(dataArray, xLoc - 20, yLoc - 20, xLoc + 20, yLoc + 20, RND, RND);
	}
	if (rectSize == 2)
	{
	    makeRectangle(dataArray, xLoc - 40, yLoc - 40, xLoc + 40, yLoc + 40, RND, RND);
	}

	if (rectSize == 3)
	{
		makeRectangle(dataArray, xLoc - 60, yLoc - 60, xLoc + 60, yLoc + 60, RND, RND);
	} 	
}

void Homework01App::update()
{
	if (greenShade > 254)
	{
		greenShade = 0;
	}
	Color8u RND = Color8u((1+rand() * 255),(1+rand() * 255),(1+rand() * 255));	
	//Get our array of pixel information
	uint8_t* dataArray = (*mySurface_).getData();

	//Makes a two-colored rectangle
	Color8u rectFill = Color8u(255,0,0);
	Color8u rectFill2 = Color8u(0,100,200);
	makeRectangle(dataArray, 50, 50, 200, 200, rectFill, rectFill2); 

	//Makes a circle
	makeCircle(dataArray, 500, 500, 100, Color8u(160,32,240));
	
	//Makes a line
	Color8u lineColor = Color8u(0,255,5);	
	makeLine(dataArray, 600, 125, 680, 565, lineColor);

	//Makes a triangle
	Color8u triangleColor = Color8u(255,255,0);
	makeTriangle(dataArray, 500, 400, 500, 500, 600, 500, triangleColor);

	//Makes a moving line

	makeLine(dataArray, 650, 50, 750, 50, Color8u(0,255,0));	
	
	if (movingSquareCount < 128) 
	{
		makeLine(dataArray, 650, 50+movingSquareCount, 750, 50+movingSquareCount, Color8u(0,255 - greenShade,0));
	}
	else if (movingSquareCount < 255)
	{
		makeLine(dataArray, 650, 178 - (movingSquareCount - 128), 750, 178 - (movingSquareCount - 128), Color8u(0,255 - greenShade,0));
	}
	else 
	{
		movingSquareCount = 0;
	}
	//Makes intersecting lines from the corners of the window
	if (pixelCount < kAppHeight)
	{
		setPixel(dataArray, pixelCount, pixelCount, Color8u(255,0,0));
		setPixel(dataArray, kAppWidth - pixelCount, pixelCount, Color8u(255,0,0));			
	}
	else 
		pixelCount = 0;

	if (pixelCountBackwards > 0)
	{
		setPixel(dataArray, pixelCount, pixelCountBackwards, Color8u(0, 255, 255));
		setPixel(dataArray, kAppWidth - pixelCount, pixelCountBackwards, Color8u(0, 255, 255));				
	}
	else 
		pixelCountBackwards = kAppHeight;

	//Only save the first frame of drawing as output, code snippet via Dr. Brinkman
	if(frame_number_ == 0){
		writeImage("marsdems.png",*mySurface_);
	}
	//keeps track of how many frames we have shown.
	frame_number_++;
	pixelCount++;
	pixelCountBackwards--;
	movingSquareCount++;
	greenShade++;
}

void Homework01App::draw()
{
	uint8_t* dataArray = (*mySurface_).getData();
	gl::draw(*mySurface_);
}

CINDER_APP_BASIC( Homework01App, RendererGl )
