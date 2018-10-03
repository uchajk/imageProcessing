// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_quantize:
//
// Quantize I1 to specified number of levels. Apply dither if flag is set.
// Output is in I2.
//
// //	Written by :	BASHIR AHAMED
//			SHOFIQUR RAHMAN
//			UCHA SAMADASHVILI
//
//	Semester   :	FALL 2018
//
void
HW_quantize(ImagePtr I1, int levels, bool dither, ImagePtr I2)
{
  int w = I1->width();
  int h = I1->height();
  int total =  w * h;
  
  int i, lut[MXGRAY];

  int noise;
  IP_copyImageHeader(I1, I2);

  double scale = (double)MXGRAY/levels;
    
  for(i = 0; i < MXGRAY; ++i) 
    lut[i] = CLIP((int)(i/scale)*scale + scale/2, 0, 255);

  int type;
  ChannelPtr<uchar> p1, p2, endPtr;
  for(int ch = 0; IP_getChannel(I1, ch, p1, type); ++ch) {
    IP_getChannel(I2, ch, p2, type);
    for(endPtr = p1 + total; endPtr > p1;){
      noise = std::min(std::max(*p1++ + dither * (rand() % ((int)scale * 2) - (int)scale), 0), 255);
      *p2++ = lut[noise];
    }
  }
}
