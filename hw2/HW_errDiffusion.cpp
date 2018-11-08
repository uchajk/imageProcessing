// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_errDiffusion:
//
// Apply error diffusion algorithm to image I1.
//
// This procedure produces a black-and-white dithered version of I1.
// Each pixel is visited and if it + any error that has been diffused to it
// is greater than the threshold, the output pixel is white, otherwise it is black.
// The difference between this new value of the pixel from what it used to be
// (somewhere in between black and white) is diffused to the surrounding pixel
// intensities using different weighting systems.
//
// Use Floyd-Steinberg     weights if method=0.
// Use Jarvis-Judice-Ninke weights if method=1.
//
// Use raster scan (left-to-right) if serpentine=0.
// Use serpentine order (alternating left-to-right and right-to-left) if serpentine=1.
// Serpentine scan prevents errors from always being diffused in the same direction.
//
// A circular buffer is used to pad the edges of the image.
// Since a pixel + its error can exceed the 255 limit of uchar, shorts are used.
//
// Apply gamma correction to I1 prior to error diffusion.
// Output is saved in I2.
//
void
copy_to_buffer(ChannelPtr<uchar> row_ptr, short *buff, int width, int lut[MXGRAY])
{
  *buff = *row_ptr;
  *(buff + width + 1) = *(row_ptr + width - 1);
  for (int i = 0; i < width; ++i)
    {
      *(buff + i + 1) = lut[*(row_ptr + i)];
    }
}
void
HW_errDiffusion(ImagePtr I1, int method, bool serpentine, double gamma, ImagePtr I2)
{
  IP_copyImageHeader(I1,I2);

  int w = I1->width();
  int h = I1->height();
  int t = w*h;
  int type;
  short *buffPtr[2];
  buffPtr[0] = (short*)malloc((w+2)*sizeof(short));
  buffPtr[1] = (short*)malloc((w+2)*sizeof(short));
  ChannelPtr<uchar> p1,p2;

  //make gamma lut
  int lut[MXGRAY];
  for (int i = 0; i < 256; ++i)
    {
      lut[i] = (int)(255*pow(double(i)/255, 1/gamma));
    }
  
  for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++)
    {
      IP_getChannel(I2, ch, p2, type);

      //copy 0 and 1 row to buffers
      copy_to_buffer(p1,buffPtr[0],w,lut);

      //serpentine
      if (serpentine) {
	for (int j = 1; j < h; j++) {
	  p1+=w;		  
	  copy_to_buffer(p1,buffPtr[j%2],w,lut);
	  if (j%2) {
	    for (int i = w; i>0 ; i--)
	      {
		float oldpix = *(buffPtr[1-j%2] + i);
		float newpix = std::round(oldpix/255)*255;
		float error = oldpix - newpix;
		*(buffPtr[1-j%2] + i - 1) += 7*error/16;
		*(buffPtr[j%2] + i + 1) += 3*error/16;
		*(buffPtr[j%2] + i) += 5*error/16;
		*(buffPtr[j%2] + i - 1) += error/16;
		*(p2+i-1) = CLIP(newpix,0,255);
	      }
	    p2+=w;
	  }
	  else {
	    for (int i = 1; i < w+1; i++)
	      {
		float oldpix = *(buffPtr[1-j%2] + i);
		float newpix = std::round(oldpix/255)*255;
		float error = oldpix - newpix;
		*(buffPtr[1-j%2] + i + 1) += 7*error/16;
		*(buffPtr[j%2] + i - 1) += 3*error/16;
		*(buffPtr[j%2] + i) += 5*error/16;
		*(buffPtr[j%2] + i + 1) += error/16;
		*p2++ = CLIP(newpix,0,255);
	      }
	  }
	}
      }
      else{
	//starting from row 2 to last row
	for (int j = 1; j < h; j++)
	  {
	    p1+=w;		  
	    copy_to_buffer(p1,buffPtr[j%2],w,lut);
	    for (int i = 1; i < w+1; i++)
	      {
		float oldpix = *(buffPtr[1-j%2] + i);
		float newpix = std::round(oldpix/255)*255;
		float error = oldpix - newpix;
		*(buffPtr[1-j%2] + i + 1) += 7*error/16;
		*(buffPtr[j%2] + i - 1) += 3*error/16;
		*(buffPtr[j%2] + i) += 5*error/16;
		*(buffPtr[j%2] + i + 1) += error/16;
		*p2++ = CLIP(newpix,0,255);
	      }
	  }
      }
    }
}
