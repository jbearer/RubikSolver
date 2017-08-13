// #include <opencv.hpp>
// #include <highgui.hpp>
// #include <videoio.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdlib.h>
#include <stdio.h>

using namespace std;
using namespace cv;

int thresh_val = 0;
int thresh_typ = 0;
int const max_typ = 4;

const char* tb_val = "Type: \n 0: Binary \n 1: Inverted Binary \n 2: Truncate \n 3: To Zero \n 4: Inverted To Zero";

Mat src, gray, dst;

void Thresh_Tut( int, void* )
{
  threshold( gray, dst, thresh_val, 255, thresh_typ );
  imshow( "A", dst );
}

int main( int argc, char** argv )
{

  src = imread( "fruits.jpg", 1 );
  cvtColor( src, gray, CV_RGB2GRAY );

  namedWindow( "A", CV_WINDOW_AUTOSIZE );

  createTrackbar( "Value", "A", &thresh_val, 255, Thresh_Tut );
  createTrackbar( tb_val, "A", &thresh_typ, max_typ, Thresh_Tut );

  Thresh_Tut( 0 , 0 );

  while(1)
  {
    int c;
    c = waitKey( 10 );
    if( (char)c == 27 ) break;
  }
  return 0;


}