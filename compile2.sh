#bin/bash
g++  -I/home/evan/Documents/IHM/opencvfiles/include/opencv -I/home/evan/Documents/IHM/opencvfiles/include/  -L/home/evan/Documents/IHM/opencvfiles/lib -g -o tpIHM2  copiemain.cpp -lX11 -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video -lopencv_contrib -lopencv_objdetect
