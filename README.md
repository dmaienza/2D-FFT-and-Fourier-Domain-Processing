# 2D-FFT-and-Fourier-Domain-Processing

Intro/Purpose:
  The main aim of this project is to familiarize with 2-D FFT and Fourier domain processing, learning how to work with complex numbers in image arrays and how to apply a low-pass filter for image smoothing. Specifically, this project works with two different images—a synthesized "cake" image and an MRI image—to understand these processes.

  The sample program, "TestFour2floats" creates a 2D complex array, initializing it to constant data, performing the forward 2D fft, then the inverse 2D fft of the results, outputing the results of each step.

  The "Lab2_Part1" program loads the "cake" image and then obtains its 2-D FFT and subsequently its inverse 2-D FFT to re-obtain the original image. A normalized spectrum image is created by using a function to normalize the image pixel values of the spectrum between 0-255.

  The "Lab2_Part2" program repeats the above FFT and IFFT steps but with an MRI image. It implement a Butterworth LPF on the MRI image in Fourier domain. The Butterworth filter is centered in the Fourier domain before application. Both real and imaginary parts of the Fourier Transform are multiplied by the Butterworth filter. The inverse 2-D FFT is performed to obtain the low-pass filtered MRI image in the spatial domain. Two different cut-off values (D0) are used for the Butterworth filter to see their effect on the output image.


Executables:
  TestFour2floats: C version with array of floats, takes forward 2D FFT, then 2D IFFT, and then output.

  Lab2_Part1: Takes forward FFT of cake.pgm, then 2D IFFT, and then output. Spectrum is normalized and then outputted.

  Lab2_Part2: Similar to part 1 program except a BWLP filter is applied in the Fourier domain at two different cut-off constants.
  

Instructions:
  Run make, then run each of the programs with everything in the same directory (./TestFour2floats, ./Lab2_Part1, ./Lab2_Part1)
  

Files:
  Makefile: builds the c files  and links them into the executable.
  
  TestFour2floats.c: C version to test Four2 using floats.

  Lab2_Part1.c: Performs part 1 of the lab.

  Lab2_Part2.c: Performs part 2 of the lab.
  
  Four2.h: contains function prototype for fft_Four2 which is accessable to the students.
  
  Four2_Private.h: countains prototype for functions needed by fft_Four2.
  
  Four2.c: contains the fft_Four2 function and all dependancies as prototyped in Four2.h and Four2_Private.h, as well as some local variables.

  cake.pgm: image of cake

  mri.pgm: image of MRI
