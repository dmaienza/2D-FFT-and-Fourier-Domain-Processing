
//Code to run 2D FFT and then Butterworth Low Pass Filter of an image
//Fall 2023
//Daniel Maienza

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Four2.h"

// Function to allocate a 2D array of floats
float **create_2D_array(int rows, int cols) {
  float **array = (float **)malloc(rows * sizeof(float *));
  if(array == NULL) {
      // Handle memory allocation failure
      exit(1);
  }
  for(int i = 0; i < rows; ++i) {
      array[i] = (float *)malloc(cols * sizeof(float));
      if(array[i] == NULL) {
          // Handle memory allocation failure
          exit(1);
      }
  }
  return array;
}

// Function to create a Butterworth filter
void create_butterworth_filter(float **filter, int n, float D0) {
    int center_x = n / 2;
    int center_y = n / 2;
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            float distance = sqrt((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y));
            filter[x][y] = 1.0 / (1.0 + pow(distance / D0, 2));
        }
    }
}

// Function to apply the Butterworth filter
void apply_butterworth_filter(float *spectrum, float **filter, int n) {
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            int index = x * n + y;
            // Multiply real and imaginary parts by filter
            spectrum[2 * index] *= filter[x][y];
            spectrum[2 * index + 1] *= filter[x][y];
        }
    }
}

// Function to calculate the total energy in the frequency domain within a circle of radius D0
float calculate_total_energy(float *magnitude_spectrum, int n, float D0) {
    float energy = 0.0;
    int center_x = n / 2;
    int center_y = n / 2;

    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            float distance = sqrt((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y));
            if (distance <= D0) {
                int index = x * n + y;
                energy += magnitude_spectrum[index];
            }
        }
    }
    return energy;
}



int main(int argc, char *argv[]){
  unsigned sizeX; //image width
  unsigned sizeY; //image height
  unsigned char *image; //image array
  unsigned levels;
  unsigned i;
  float min_val; //max of spectrum magnitude
  float max_val; //min of spectrum magnitude
  float D0 = 10; //cutoff frequency
  
    /* Read Image */
  FILE *iFile = fopen("mri.pgm","r");
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
  
  // Create a 2D array for the filter
  float **filter = create_2D_array(n, n);
  
  // Create the Butterworth filter
  create_butterworth_filter(filter, n, D0);
    
  // Apply the Butterworth filter to the spectrum
  apply_butterworth_filter(spectrum, filter, n);

  /*copy spectrum into output and call inverse FFT on output later */
  for(i=0; i<2*n*n; i++){
    output[i]=spectrum[i];
  }
  
  //Find magnitude of spectrum from real and imaginary part
  for(i=0; i<n*n; i++){
    magnitude_spectrum[i]=sqrt(spectrum[2*i]*spectrum[2*i]+spectrum[2*i+1]*spectrum[2*i+1]);
  }
  
  //Calculate the total energy remaining after filtering with the cutoff
  float total_energy = calculate_total_energy(magnitude_spectrum, n, D0);
  printf("Total energy with D0=%.0f: %.0f\n", D0, total_energy);
  
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
  iFile = fopen("mri_spectrum.pgm","w");
  if(iFile==0) return 1; //error handling
  fprintf(iFile, "P5 %d %d %d ",sizeX,sizeY,255);//write header
  fwrite(normalized_image, sizeof(unsigned char), sizeX*sizeY, iFile); //write binary image
  fclose(iFile);
  
  //call the 2D inverse FFT on output
  fft_Four2(output,n,n,true);
  
  // Extract real components
  unsigned char *real_image;
  real_image = (unsigned char*) malloc(n*n*sizeof(unsigned char));
  for(i=0; i<n*n; i++){
    real_image[i] = (unsigned char) (output[2*i]);
  }

  /*write real image to file*/
  iFile = fopen("mri_real.pgm","w");
  if(iFile==0) return 1; //error handling
  fprintf(iFile, "P5 %d %d %d ",sizeX,sizeY,255);//write header
  fwrite(real_image, sizeof(unsigned char), sizeX*sizeY, iFile); //write binary image
  fclose(iFile);
  return 0;
}

