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


	void basicRectangle (uint8_t* pixels, int x1, int y1, int x2, int y2, Color8u c1, Color8u c2);

	void redTintImage (uint8_t* pixels);
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
	
	//Boundary Checking
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
				pixels[3*(x + y*kTextureSize)] = fill.r;
				pixels[3*(x + y*kTextureSize)+1] = fill.g;
				pixels[3*(x + y*kTextureSize)+2] = fill.b;				
			}
			else if (currentPixel > (rectSize/2)) 
			{
				pixels[3*(x + y*kTextureSize)] = fill2.r;
				pixels[3*(x + y*kTextureSize)+1] = fill2.g;
				pixels[3*(x + y*kTextureSize)+2] = fill2.b;				
			}
			//Middle pixel of rectangle will be red
			else
			{
				pixels[3*(x + y*kTextureSize)] = 255;
				pixels[3*(x + y*kTextureSize)+1] = 0;
				pixels[3*(x + y*kTextureSize)+2] = 0;
			}
			currentPixel++;
		}
	}
}

void Homework01App::redTintImage(uint8_t* pixels) 
{
	for (int y = 0; y <= kAppHeight; y++) {
		for (int x = 0; x <= kAppWidth; x++) {	
			pixels[3*(x + y*kTextureSize)] = pixels[3*(x + y*kTextureSize)] + 100;
			pixels[3*(x + y*kTextureSize)+1] = pixels[3*(x + y*kTextureSize)+1];
			pixels[3*(x + y*kTextureSize)+2] = pixels[3*(x + y*kTextureSize)+2];		
		}
	}
}

//void Homework01App::basicCircle(
void Homework01App::setup()
{
	frame_number_=0;
	
	//This is the setup that everyone needs to do
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
}

void Homework01App::mouseDown( MouseEvent event )
{
}

void Homework01App::update()
{
	//Get our array of pixel information
	uint8_t* dataArray = (*mySurface_).getData();

	Color8u rectFill = Color8u(128,255,128);
	Color8u rectFill2 = Color8u(255,128,255);
	basicRectangle(dataArray, 100, 100, 400, 400, rectFill, rectFill2); 

	//redTintImage(dataArray);

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
