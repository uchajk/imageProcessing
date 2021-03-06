#include <tgmath.h>
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_gammaCorrect:
//
// Gamma correct image I1. Output is in I2.
//
void
HW_gammaCorrect(ImagePtr I1, double gamma, ImagePtr I2)
{
  IP_copyImageHeader(I1, I2);

  int w = I1->width();
  int h = I1->height();
  int total = w*h;
  int lut[MXGRAY];

  for (int i = 0; i < 256; ++i)
    {
      lut[i] = (int)(255*pow(double(i)/255, 1/gamma));
    }

  ChannelPtr<uchar> p1,p2,endPtr;
  int type;

  for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
    IP_getChannel(I2, ch, p2, type);
    for(endPtr = p1 + total; p1 < endPtr; )
      *p2++ = lut[*p1++];
  }
  
}
