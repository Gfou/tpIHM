#include "opencv2/opencv.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <X11/Xlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


#define PI 3.141592

using namespace cv;


int main(int, char**)
{

    //choisir l'ecran
    Display* dpy = XOpenDisplay(0);
    int scr = XDefaultScreen(dpy);
    Window root_window = XRootWindow(dpy, scr);

    //calculer la taille et de l'ecran
    int height = DisplayHeight(dpy, scr);
    int width  = DisplayWidth(dpy, scr);
    std::cout << "Screen size : " << width << "x" << height << std::endl;

    //initialiser la position du curseur au milieu de l'écran
    int m,n,l;
    n=width/2; l=height/2;
    int posx=n;
    m = 0;

    
    vector<Mat> images;
    vector<int> labels;

    CascadeClassifier haar_cascade;
    CascadeClassifier eyes_cascade;

    haar_cascade.load("./haarcascade_frontalface_default.xml");
    eyes_cascade.load("./haarcascade_eye_tree_eyeglasses.xml");

    VideoCapture cap(0); 
    if(!cap.isOpened()) 
        return -1;


    Mat grImage;
    Mat resizeImage;
    namedWindow("face",1);
    int x1,x2,y1,y2;
    int gauche=0, droite=0, haut=0, bas=0;
    int aire, aireMoyen=0, centre=0, centreMoyen=0, echantillon=20, nbEchantillon=0;

    for(nbEchantillon;nbEchantillon<echantillon;nbEchantillon++){

	Mat frame;
        cap >> frame; 						// get a new frame from camera
	resize(frame,resizeImage,Size(0,0),0.8,0.8);

        cvtColor(resizeImage, grImage, CV_BGR2GRAY);
        vector< Rect_<int> > faces;                             //on crée un tableau de rectangle
        haar_cascade.detectMultiScale(grImage, faces);          //on detecte le visage, chaque visage est stocké dans une case du tableau "faces"

                        
        Rect face_i = faces[0];                         
        rectangle(resizeImage, face_i, CV_RGB(255, 0, 0), 1);  	

        centre=((face_i.size().height)/2)+face_i.y;
	centreMoyen+=centre;
	aire=face_i.size().width*face_i.size().height;
        aireMoyen+=aire;
	imshow("face", resizeImage);
	if(waitKey(30) >= 0) break;


     }
    
    centreMoyen=centreMoyen/echantillon;
    aireMoyen=aireMoyen/echantillon;
    printf("aire moyen %d\n",aireMoyen);
    
    for(;;)
    {
	
        Mat frame;
        cap >> frame; // get a new frame from camera
	resize(frame,resizeImage,Size(0,0),0.8,0.8);
        cvtColor(resizeImage, grImage, CV_BGR2GRAY);
	vector< Rect_<int> > faces;    				//on crée un tableau de rectangle
        haar_cascade.detectMultiScale(grImage, faces);		//on detecte le visage, chaque visage est stocké dans une case du tableau "faces"

        for(int i = 0; i < faces.size(); i++) {			//faces.size() renvoie le nombre de rectangle, soit le nombre de visage
		Rect face_i = faces[i]; 			//
		rectangle(resizeImage, face_i, CV_RGB(0, 255,0), 1);  //fonction pour tracer le rectangle "face_i" dans l'image "frame"

		aire=face_i.size().width*face_i.size().height;
		centre=((face_i.size().height)/2)+face_i.y;


		if(centreMoyen-centre>0){
                        haut=(centreMoyen-centre);
                        bas=0;
			if (haut>30 || haut<5) haut=0;
                }
                else{
                        haut=0;
                        bas=-(centreMoyen-centre);
			if(bas>30 || bas<5) bas=0;
                }


		
		Mat faceROI = grImage( faces[i] ); 

     		std::vector<Rect> eyes;

   	        //-- In each face, detect eyes
 	        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

	
                for( size_t j = 0; j < eyes.size(); j++ ){
    	  		Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );  //on trouve le point central
  	  		int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );	//cvRound renvoie l'entier le plus proche, radius = rayon
      	  		circle( resizeImage, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );	//on trace le cercle sur l'image "frame" de centre "center" et de rayon "radius"
			if(eyes[0].x<eyes[1].x){
				x1=center.x;
				y1=center.y;
			}
			else{
				x2=center.x;
				y2=center.y;
			}

		}
		
		if(y2-y1>0){
			droite=(y2-y1);
			gauche=0;
			if(droite>30 || droite<5) droite=0;
		}
		else{ 
			gauche = -(y2-y1);
			droite=0;
			if(gauche>30 || gauche<5) gauche=0;
		}


					
		printf("gauche: %d droite: %d haut: %d bas: %d\n",gauche,droite,haut,bas);
		
        	n=n+(droite-gauche);
		if(n<0) n=0;
		else if(n>width) n=width;
		
		l=l-(haut-bas);
		if(l<0) l=0;
		else if(l>height) l=height;
        	//mettre à jour le curseur
        	XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, n, l);
        	//if(m==0){
          	//simuler le click si utilisateur entre 0
          	//click (dpy, Button1);
        	//}
        	XFlush(dpy);
        	usleep(50);

     		   
	}
        imshow("face", resizeImage);
        if(waitKey(30) >= 0) break;
	
    }

    return 0;
}
