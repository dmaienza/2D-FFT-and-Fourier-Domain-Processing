
//Sample code to run 2D FFT
//fall 2023
//Prof. M.
#include <stdio.h>
#include <stdlib.h>
#include "Four2.h"

void printvector(float *data, 
                 const unsigned n){
  unsigned i,j;
  for(j=0; j<n; j++){
    if(j==0){
      printf("\342\224\217 ");
    }else if (j==n-1){
      printf("\342\224\227 ");
    }else{
      printf("\342\224\203 ");
    }
    for(i=0; i<n; i++){
      printf("%+02.2f%+02.2fi ",data[2*i+j*n*2],data[1+2*i+2*j*n]);
    }
    if(j==0){
      printf("\342\224\223\n");
    }else if (j==n-1){
      printf("\342\224\233\n");
    }else{
      printf("\342\224\203\n");
    }
  }
}

int main(int argc, char *argv[]){
  unsigned i;
  
  unsigned n=4; //size of 2D data in both directions
  
  /* 3 arrays with the following meaning */
  /* data -->FFT --> spectrum --> IFFT --> output */
  float *data, *spectrum, *output;
  
  data=(float*)calloc(sizeof(float),2*n*n);
  spectrum=(float*)malloc(sizeof(float)*2*n*n);
//NOTE: DO NOT CONFUSE Spectrum which is just a variable name we give here for the complex Fourrier transform, with  the image spectrum as we define it in class
  output=(float*)malloc(sizeof(float)*2*n*n);
  
  if(data==0 || spectrum==0 || output==0) return 1;
  
  
  
  //setup data
  data[2+1*n*2]=0.1;
  data[4+1*n*2]=0.5;
  data[6+1*n*2]=0.3;
  data[2+2*n*2]=0.5;
  data[4+2*n*2]=1.0;
  data[6+2*n*2]=0.5;
  data[2+3*n*2]=0.3;
  data[4+3*n*2]=0.5;
  data[6+3*n*2]=0.3;
  
  /* since fourl destroys the input array with the output */
  /* use the output of the FFT as the input value */
  
  for(i=0; i<2*n*n; i++){
    spectrum[i]=data[i];
  }
  //call 2D fft
  fft_Four2(spectrum,n,n,false);
  
  /*copy spectrum into output and call invcrse FFT */
  for(i=0; i<2*n*n; i++){
    output[i]=spectrum[i];
  }

//call the 2D inverse FFT

  fft_Four2(output,n,n,true);
  
  printf("Data\n");
  printvector(data,n);
  
  printf("Spectrum:\n");
  printvector(spectrum,n);
  
  printf("Output:\n");
  printvector(output,n);
  
  
}

