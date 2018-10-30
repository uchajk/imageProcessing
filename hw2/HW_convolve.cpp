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
  
  //create pixel pointers
  ChannelPtr<float> pk, endPtr;
  ChannelPtr<uchar> p1, p2;
  int type;
  int ch =0;

  IP_getChannel(I1, ch, p1, type);
  
  int wb = w + wk -1;
  int hb = h + hk -1;
  int totalb = wb*hb;
  unsigned char buff [totalb];
  for(int i = hk/2; i < hb - hk/2; i++)
    for(int j = wk/2; j < wb - wk/2; j++){
      buff[j + i*wb] = *p1++;
    }
  for(int i = 0; i < hk/2; i++)
    for(int j = 0; j < wb; j++){
      buff[j + i*wb] = buff[j + wb*hk/2];
    }
  for(int i = hb - 1; i >= hb - hk/2; i--)
    for(int j = 0; j < wb; j++){
      buff[j + i*wb] = buff[j + wb*(hb - hk/2 - 1)];
    }
  for(int i = 0; i < wk/2; i++)
    for(int j = 0; j < hb; j++){
      buff[j*wb + i] = buff[wk/2 + j*wb];
    }
  for(int i = wb - wk/2; i < wb; i++)
    for(int j = 0; j < hb; j++){
      buff[j*wb + i] = buff[wb - wk/2 - 1 + j*wb];
    }

  for(int i = 0; i < 9; i++){
    for(int j = 0; j < 9; j++){
      std::cout << (int)buff[j + i*wb] << " ";
    }
    std::cout << "\n";
  }

  IP_getChannel(I1, ch, p1, type);
  std::cout << "------------\n";
  for(int i = 0; i < 9; i++){
    for(int j = 0; j < 9; j++){
      std::cout << (int)*(p1+j+i*w) << " ";
    }
    std::cout << "\n";
  }  
  IP_getChannel(Ikernel, ch, pk, type);
  IP_getChannel(I2, ch, p2, type);
  for(int i = hk/2; i < hb - hk/2; i++)
    for(int j = wk/2; j < wb - wk/2; j++){
      //for all pixels excluding padding
      float tmp = 0;
      for(int k = 0; k < hk; k++)
	for(int l = 0; l < wk; l++){
	  tmp += *(pk + l + k*wk) * (int)buff[j + i*wb - wk/2 - hk/2*wb + l + k*wb];
	    }
      *p2++ = (uchar)tmp;
    }
    
  
  // IP_getChannel(I1, ch, p1, type);
  // p1 += (w + 1);
  // std::cout << (int)*p1 << "\n";

  // IP_getChannel(Ikernel, ch, pk, type);
  // for(int i = 0; i < 9; i++)
  //   std::cout << *pk++ << "\n";

  // float tmp = 0;
  // for(int i = 0; i < wk; i++)
  //   for(int j = 0; j < hk; j ++){
  //     tmp += (int)*(p1 - hk/2*w - wk/2 + w*j + i) * *(pk + i + wk*j);
  //   }
  // std::cout << int(tmp) << "\n";
      

  
  //iterate pixels
  // std::cout << *pk << "\n" << ch << "\n" << type << "\n";

  // std::cout << *p1 << "\n" << ch << "\n" << type << "\n";
  // std::cout << "kernel width: " << wk  << "\n";
  // std::cout << "kernel height: " << hk << "\n";
  // std::cout << "kernel total: " << totalk << "\n";
  //  for(endPtr = pk + wtotal; pk < endPtr; )
  
}
