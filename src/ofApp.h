#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	public:
    		//****************************************************** buit in OF method prototypes *****************************************************************
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    //*********************************************** Pixel by Pixel course method prototypes **************************************************************
    void ourProcessImage();
    void ourSetPixel(int horizontal,int vertical,unsigned char R,unsigned char G,unsigned char B,int width, unsigned char pixels[]);
    void ourGetPixel(int horizontal,int vertical,unsigned char* R,unsigned char* G,unsigned char* B,int width, unsigned char  pixels[]);
    void copyPixelArrays(unsigned char sourceArray[],unsigned char destArray[] ,int Width,int Height, int bpp);
    void copyPixelArrays(unsigned char* sourceArray,unsigned char* destArray ,int sourceWidth,int sourceHeight,int destWidth,int destHeight );
    void copyPixelArrays(unsigned char* sourceArray,unsigned char* destArray ,int Width,int Height, int sourceRectL,int sourceRectT,int sourceRectR,int sourceRectB, int destRectL,int destRectT,int destRectR,int destRectB);
    void clearPixelArrays(unsigned char* pixelArray,int Width,int Height,unsigned char R ,unsigned char G,unsigned char B );
    
    //************** Pixel by Pixel course macros (works like a method but is precompiled with the rest of the code******************************************
#define CLIP(val, low, high) {if(val<low) val=low; if(val>high)val=high;}
    

    
    
    int					width, height;
    int					source , grabbedFirstFrame;
    ofVideoGrabber 		vidGrabber;					//in case we want to use live video
    ofVideoPlayer			vidPlayer;						// in case we want to use digital video
    unsigned char			*ourImagePixels, *ourResultPixels ;
    ofTexture				 ourResultTexture ;
    ofImage				ourImage;
};
