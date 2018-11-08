// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_blur:
//
// Blur image I1 with a box filter (unweighted averaging).
// The filter has width filterW and height filterH.
// We force the kernel dimensions to be odd.
// Output is in I2.
//
void
HW_blur(ImagePtr I1, int filterW, int filterH, ImagePtr I2)
{
  IP_copyImageHeader(I1,I2);

  //create vars for I1
  int w = I1->width();
  int h = I1->height();
  
  //create channel pointers
  ChannelPtr<uchar> p1, p2;
  int type;

  for (int ch=0; IP_getChannel(I1,ch,p1,type); ch++) {
    IP_getChannel(I2,ch,p2,type);
    
  }

}
