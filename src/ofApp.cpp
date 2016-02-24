/*		ITP Pixel by Pixel
 adjust brightness
 change source to select video, movie or image
 adjusts the brightness of a region of an image or video
 Danny Rozin 2016
 */

#include "ofApp.h"


//************** Everything you want to happen once in the begining, but don't draw anything to the creen here a it will not work ****************
void ofApp::setup(){
    height = ofGetHeight();
    width = ofGetWidth();
    grabbedFirstFrame=0;
    source =2;	//  0= image file, 1 = video file ,2 = camera
    
    if (source ==2){													// if the source is live video
        vidGrabber.setVerbose(true);
        vidGrabber.setUseTexture(false);								// we are not showing the video on screen so we can live without the texture
        vidGrabber.initGrabber(width,height);							// start the video
        width = vidGrabber.getWidth();									// even though we asked for a certain size we better see what we got
        height = vidGrabber.getHeight();
    }
    if (source ==1){												// if the source is a movie
        vidPlayer.loadMovie("ourMovie.mov");							// load the movie, has to be in the data folder
        vidPlayer.play();											// start the play
        width = vidPlayer.getWidth();										// the size will be determined by the movie size
        height = vidPlayer.getHeight();
    }
    if (source ==0){												// if the source is an image
        ourImage.loadImage("ourImage.jpg");							// load the image, has to be in the data folder
        ourImage.resize(width,height);								// make it the size we like (stretches or shrinks as needed)
        width = ourImage.getWidth();										// just to be sure the size is right
        height = ourImage.getHeight();
    }
    
    ourResultTexture = *  new ofTexture();								// create a ofTexture to hold the result info
    ourResultTexture.allocate(width, height, GL_RGB);					// allocate memory for the ofTextue (This is RGB no Alpha)
    ourResultPixels = new unsigned char[width*height*3];					// create a pixel array to keep our processed pixel data (RGB, no Alpha).
}


//**************** Code in update() execute every frame, but don't draw anything here cause it won't work, in general put most stuff into draw() *************
void ofApp::update(){
    if (source ==2){																	// if the source is live video
        vidGrabber.update();														// grab a frame
        if( vidGrabber.isFrameNew()){														// if the frame we got is new
            ourImagePixels=vidGrabber.getPixels();											// get the pixel into our pixel array
            grabbedFirstFrame=1;
        }
    }
   	if (source ==1){																	// if the source is movie
        vidPlayer.update();															// grab a frame
        if (vidPlayer.isFrameNew()){														// if the frame we got is new
            ourImagePixels=vidPlayer.getPixels();											// get the pixel into our pixel array
            grabbedFirstFrame=1;
            
        }
    }
    if (source ==0){																	// if the source is an image
        ourImagePixels = ourImage.getPixels();												// get the pixel into our pixel array
        grabbedFirstFrame=1;
        
    }																				// this whole thing is a bit wasetful, for a static image we could have done this once in setup()
}


//********************************************* Code in draw() execute every frame,  **********************************************************
void ofApp::draw(){
    if (grabbedFirstFrame==1) ourProcessImage();						// wait till the first frame is grabbed before we start messing with it
    
    ourResultTexture.loadData(ourResultPixels, width, height, GL_RGB);		// This copies our processed pixel data into a texture that can be displayed
    ourResultTexture.draw(0,0);											// display the result on the screen
}


//******************************* Pixel by Pixel course method, I think most of your code will go in here   **************************************
void ofApp::ourProcessImage(){
    unsigned char R,G,B ;
    int newR,newG,newB, brightness=100;
    
    copyPixelArrays(ourImagePixels,ourResultPixels,width,height,3);								// copies the live image pixel data into the result pixel array..
    // using our function copyPixelArrays(from,to,width,height,bpp);
    
    int ProcccesedAreaL = mouseX-100; ProcccesedAreaL=ofClamp(ProcccesedAreaL,0,width);							// we want to process a region of 200 x 200 around the mouse...
    int ProcccesedAreaR = mouseX+100; ProcccesedAreaR=ofClamp(ProcccesedAreaR,0,width);						// but we want to make sure we are inside the pixel array...
    int ProcccesedAreaT = mouseY-100; ProcccesedAreaT=ofClamp(ProcccesedAreaT,0,height);						// so we call CLIP to make sure we are not lws than 0 or more than height, width
    int ProcccesedAreaB = mouseY+100; ProcccesedAreaB=ofClamp(ProcccesedAreaB,0,height);
    for (int y = ProcccesedAreaT; y < ProcccesedAreaB; y++) {
        for (int x = ProcccesedAreaL; x < ProcccesedAreaR; x++) {
            ourGetPixel(x,y,&R,&G,&B,width,ourImagePixels);									// our method that gets RGBs values at x,y position, note that it expects pointers for R,G,B
            //brightness = x-ProcccesedAreaL-100;											// add this to create a gradation of brightness change
            newR= R+brightness;														// adding some brightness to each pixel by adding to R,G,B
            newG= G+brightness;														// note that we are doing this on newR,newG,newB which are integer, not unsigned chars...
            newB= B+brightness;														// this way we can exceed 255 and go negative
            
            newR=ofClamp(newR,0,255);															// make sure the result is between 0-255
            newG=ofClamp(newG,0,255);
            newB=ofClamp(newB,0,255);
            
            ourSetPixel(x,y,newR,newG,newB,width,ourResultPixels);						// set the resulting R,G,B value in our result pixel array
        }
    }
    
}






//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    if (key == 's' || key == 'S'){
        vidGrabber.videoSettings();
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){  }

void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::dragEvent(ofDragInfo dragInfo){}
void ofApp::gotMessage(ofMessage msg){}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){ }

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){	}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}
//**********************  Pixel by Pixel course method that sets a pixel RGB in an x, y position in a pixel array ***********************

void ofApp::ourSetPixel(int horizontal,int vertical,unsigned char R,unsigned char G,unsigned char B,int w, unsigned char pixels[]){
    int thisPixel;
    thisPixel = 3*(w * vertical +horizontal);
    pixels[thisPixel]=R;
    pixels[thisPixel+1]=G;
    pixels[thisPixel+2]=B;
}

//**********************  Pixel by Pixel course method that sets a pixel RGB in an x, y position in a pixel array ************************

void ofApp::ourGetPixel(int horizontal,int vertical,unsigned char* R,unsigned char* G,unsigned char* B,int w, unsigned char pixels[]){
    int thisPixel;
    thisPixel = 3*(w * vertical +horizontal);
    *R= pixels[thisPixel];
    *G= pixels[thisPixel+1];
    *B= pixels[thisPixel+2];
}

//**********************  Pixel by Pixel course method that copies the content of one pixel array to another ******************************
//*****************************  Note that both pixel arays must have the same height and width  ******************************************

void ofApp::copyPixelArrays(unsigned char sourceArray[],unsigned char destArray[] ,int Width,int Height, int bpp){
    memcpy(destArray,sourceArray,Width*Height*bpp) ;
}


//**********************  Pixel by Pixel course method that copies the content of one pixel array to another ******************************
//********************  Note that both pixel arays can have different sizes and this will shrink or stretch  ******************************

void ofApp::copyPixelArrays(unsigned char* sourceArray,unsigned char* destArray ,int sourceWidth,int sourceHeight,int destWidth,int destHeight ){
    int bpp = 3;
    int xFactor = (100*sourceWidth)/destWidth;
    int yFactor =( 100*sourceHeight)/destHeight;
    
    for (int y = 0 ;y< destHeight;y++){
        for (int x = 0 ;x< destWidth;x++){
            int destPixel = bpp*(y*destWidth+x);
            //int sourcePixel = bpp*(y*sourceWidth*yFactor+x*xFactor);
            int sourceX= (x*xFactor)/100;
            int sourceY = (y*yFactor)/100;
            int sourcePixel = bpp*(sourceY*sourceWidth+sourceX);
            destArray[destPixel]= sourceArray[sourcePixel];
            destArray[destPixel+1]= sourceArray[sourcePixel+1];
            destArray[destPixel+2]= sourceArray[sourcePixel+2];
        }
    }
}

//*******************  Pixel by Pixel course method that copies a region of pixels from one array to a region in anther  ***********************
//***************   Note that both pixel arrays must have the same height and widths and this will shrink or stretch the region ****************
//***************								This is uefull for cropping images												****************

void ofApp::copyPixelArrays(unsigned char* sourceArray,unsigned char* destArray ,int Width,int Height,
                              int sourceRectL,int sourceRectT,int sourceRectR,int sourceRectB,
                              int destRectL,int destRectT,int destRectR,int destRectB){
    int bpp = 3;
    if (sourceRectR-sourceRectL==destRectR-destRectL && sourceRectT-sourceRectB==destRectT-destRectB){
        int deltaX= destRectL-sourceRectL;
        int deltaY= destRectT-sourceRectT;
        for (int y = destRectT ;y< destRectB;y++){
            for (int x = destRectL ;x< destRectR;x++){
                int destPixel = bpp*(y*Width+x);
                int sourcePixel = bpp*((y+deltaY)*Width+deltaX+x);
                destArray[destPixel]= sourceArray[sourcePixel];
                destArray[destPixel+1]= sourceArray[sourcePixel+1];
                destArray[destPixel+2]= sourceArray[sourcePixel+2];
            }
        }
    }else{
        int deltaX= destRectL-sourceRectL;
        int deltaY= destRectT-sourceRectT;
        int sourceWidth = sourceRectL-sourceRectR;
        int sourceHeight = sourceRectT-sourceRectB;
        int destWidth = destRectL-destRectR;
        int destHeight = destRectT-destRectB;
        int xFactor = (100*sourceWidth)/destWidth;
        int yFactor =( 100*sourceHeight)/destHeight;
        for (int y = destRectT ;y< destRectB;y++){
            int sourceY = deltaY+(y*yFactor)/100;
            for (int x = destRectL ;x< destRectR;x++){
                int destPixel = bpp*(y*Width+x);
                int sourceX= deltaX+(x*xFactor)/100;
                int sourcePixel = bpp*(sourceY*Width+sourceX);
                destArray[destPixel]= sourceArray[sourcePixel];
                destArray[destPixel+1]= sourceArray[sourcePixel+1];
                destArray[destPixel+2]= sourceArray[sourcePixel+2];
            }
        }
    }	
}

//**********************  Pixel by Pixel course method that set all the pixel in pixel array to a certain R,G,B value  *****************************

void ofApp::clearPixelArrays(unsigned char* pixelArray,int Width,int Height,unsigned char R ,unsigned char G,unsigned char B ){
    int bpp = 3;
    for (int y = 0 ;y< Height;y++){
        for (int x = 0 ;x< Width;x++){
            int Pixel = bpp*(y*Width+x);
            pixelArray[Pixel]= R;
            pixelArray[Pixel+1]= G;
            pixelArray[Pixel+2]= B;
        }	
    }
}


