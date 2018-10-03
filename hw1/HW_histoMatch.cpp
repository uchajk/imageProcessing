
void histoMatchApprox(ImagePtr, ImagePtr, ImagePtr);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_histoMatch:
//
// Apply histogram matching to I1. Output is in I2.
//
void
HW_histoMatch(ImagePtr I1, ImagePtr targetHisto, bool approxAlg, ImagePtr I2)
{
  if(approxAlg) {
    histoMatchApprox(I1, targetHisto, I2);
    return;
  }

  IP_copyImageHeader(I1, I2);
  int width = I1->width();
  int height = I1->height();
  int total = width*height;

  //initialize histogram to 0
  int h[MXGRAY];
  for(int i = 0; i < MXGRAY; i++)
    h[i] = 0;

  ChannelPtr<uchar> p1, p2, endPtr;
  int type;

  //compute histogram
  for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++)
    for(endPtr = p1 + total; p1 < endPtr; )
      h[*p1++]++;

  //normalize histogram
  double c[MXGRAY];
  for(int i = 0; i < MXGRAY; i++)
    c[i] =  (double)h[i] / total;

  //compute CDF
  for(int i = 1; i < MXGRAY; i++)
    c[i] = c[i-1] + c[i];

  //compute desired CDF
  int dh[MXGRAY];
  for(int i = 0; i < MXGRAY; i++)
    dh[i] = 0;
  int ref_width = targetHisto->width();
  int ref_height = targetHisto->height();
  int ref_total = ref_width * ref_height;
  for(int ch = 0; IP_getChannel(targetHisto, ch, p1, type); ch++){
    for(endPtr = p1 + ref_total; p1 < endPtr; )
      dh[*p1++]++;
  }
  double dc[MXGRAY];
  for(int i = 0; i < MXGRAY; i++)
    dc[i] = (double)dh[i] / ref_total;
  for(int i = 1; i < MXGRAY; i++)
    dc[i] = dc[i-1] + dc[i];

  //initialize lut
  int lut[MXGRAY];
  for(int i = 0; i < MXGRAY; i++)
    lut[i] = i;

  //calculate transformation
  for(int l = 0; l < MXGRAY; l++){
    int L = 255;
    while(L >= 0 and dc[L] > c[l]){
      lut[l] = L;
      L--;
    }
  }

  // apply transformation
  for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++){
    IP_getChannel(I2, ch, p2, type);
    for(endPtr = p1 + total; p1 < endPtr; )
      *p2++ = lut[*p1++];
  }
  
}

void
histoMatchApprox(ImagePtr I1, ImagePtr targetHisto, ImagePtr I2)
{
}
