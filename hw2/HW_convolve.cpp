// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_convolve:
//
// Convolve I1 with filter kernel in Ikernel.
// Output is in I2.
//
void
HW_convolve(ImagePtr I1, ImagePtr Ikernel, ImagePtr I2)
{

  IP_copyImageHeader(I1,I2);
  
  //create vars for kernel
  int wk = Ikernel->width();
  int hk = Ikernel->height();

  //create vars for I1
  int w = I1->width();
  int h = I1->height();
  int total = w*h;

  //create vars for buffer
  int wb = w + wk -1;
  int hb = h + hk -1;
  int totalb = wb*hb;
  unsigned char buff [totalb];
  
  //create pixel pointers
  ChannelPtr<float> pk;
  ChannelPtr<uchar> p1, p2, endPtr;
  int type;
  int ch = 0;

  //set p1 to point to first pixel
  IP_getChannel(I1, ch, p1, type);

  //copy I1 to buff using p1
  for(int i = hk/2; i < hb - hk/2; i++)
    for(int j = wk/2; j < wb - wk/2; j++){
      buff[j + i*wb] = *p1++;
    }

  //padding
  for(int i = 0; i < hk/2; i++)
    for(int j = 0; j < wb; j++)
      buff[j + i*wb] = buff[j + wb*hk/2];
  for(int i = hb - hk/2; i < hb; i++)
    for(int j = 0; j < wb; j++)
      buff[j + i*wb] = buff[j + wb*(hb - hk/2 - 1)];
  for(int i = 0; i < wk/2; i++)
    for(int j = 0; j < hb; j++)
      buff[j*wb + i] = buff[j*wb + wk/2];
  for(int i = wb - wk/2; i < wb; i++)
    for(int j = 0; j < hb; j++)
      buff[j*wb + i] = buff[j*wb + wb - wk - 1];
  
  //create buffers for rows
  unsigned char *arrayofpointers[hk];
  for(int i = 0; i < hk; i++)
    arrayofpointers[i] = buff + i*wb;

    //get kernel pointer pk
  IP_getChannel(Ikernel, ch, pk, type);

  //get I2 pointer p2
  IP_getChannel(I2, ch, p2, type);  
}
