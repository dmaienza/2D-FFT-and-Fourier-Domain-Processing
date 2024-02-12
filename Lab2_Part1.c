
//Code to run 2D FFT of an image
//Fall 2023
//Daniel Maienza

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Four2.h"

int main(int argc, char *argv[]){
  unsigned sizeX; //image width
  unsigned sizeY; //image height
  unsigned char *image; //image array
  unsigned levels;
  unsigned i;
  float min_val; //max of spectrum magnitude
  float max_val; //min of spectrum magnitude
  
    /* Read Image */
  FILE *iFile = fopen("cake.pgm","r");
  if(iFile==0) return 1;
  if(3!=fscanf(iFile, "P5 %d %d %d ", &sizeX, &sizeY, &levels)) return 1;
  image=(unsigned char *) malloc(sizeX*sizeY);
  fread(image,sizeof(unsigned char),sizeX*sizeY,iFile);
  fclose(iFile);
  
  unsigned n=sizeX; //size of 2D image in both directions
  
  /* 3 arrays with the following meaning */
  /* image --> FFT --> spectrum --> IFFT --> output */
  float *spectrum, *output, *magnitude_spectrum;
  
  spectrum=(float*)malloc(sizeof(float)*2*n*n);
//NOTE: DO NOT CONFUSE Spectrum which is just a variable name we give here for the complex Fourrier transform, with  the image spectrum as we define it in class
  output=(float*)malloc(sizeof(float)*2*n*n);
  magnitude_spectrum = (float*)malloc(sizeof(float)*n*n);
  
  if(spectrum==0 || output==0) return 1;
  
  /* since fourl destroys the input array with the output */
  /* use the output of the FFT as the input value */
  for(i=0; i<n*n; i++){
    spectrum[2*i]=image[i];
    spectrum[2*i+1]=0;
  }
    
  //call 2D fft
  fft_Four2(spectrum,n,n,false);
  
  /*copy spectrum into output and call inverse FFT on output later */
  for(i=0; i<2*n*n; i++){
    output[i]=spectrum[i];
  }
  
  //Find magnitude of spectrum from real and imaginary part
  for(i=0; i<n*n; i++){
    magnitude_spectrum[i]=sqrt(spectrum[2*i]*spectrum[2*i]+spectrum[2*i+1]*spectrum[2*i+1]);
  }
  
  //Find min and max
  min_val = magnitude_spectrum[0];
  max_val = magnitude_spectrum[0];
  
  for(i=0; i<n*n; i++) {
    if (magnitude_spectrum[i] < min_val) {
      min_val = magnitude_spectrum[i];
      }
    if (magnitude_spectrum[i] > max_val) {
      max_val = magnitude_spectrum[i];
      }
  }
  
  //Normalize the spectrum
  unsigned char *normalized_image;
  normalized_image = (unsigned char*) malloc(n*n*sizeof(unsigned char));
  
  for(i=0; i<n*n; i++) {
    normalized_image[i] = (255 * (magnitude_spectrum[i]-min_val) / (max_val-min_val));
  }
  
  /*write spectrum image to file*/
  iFile = fopen("cake_spectrum.pgm","w");
  if(iFile==0) return 1; //error handling
  fprintf(iFile, "P5 %d %d %d ",sizeX,sizeY,255);//write header
  fwrite(normalized_image, sizeof(unsigned char), sizeX*sizeY, iFile); //write binary image
  fclose(iFile);
  
  //call the 2D inverse FFT on output
  fft_Four2(output,n,n,true);
  
  // Extract real components for display
  unsigned char *real_image;
  real_image = (unsigned char*) malloc(n*n*sizeof(unsigned char));
  for(i=0; i<n*n; i++){
    real_image[i] = (unsigned char) (output[2*i]);
  }

  /*write real image to file*/
  iFile = fopen("cake_real.pgm","w");
  if(iFile==0) return 1; //error handling
  fprintf(iFile, "P5 %d %d %d ",sizeX,sizeY,255);//write header
  fwrite(real_image, sizeof(unsigned char), sizeX*sizeY, iFile); //write binary image
  fclose(iFile);
  return 0;
}

