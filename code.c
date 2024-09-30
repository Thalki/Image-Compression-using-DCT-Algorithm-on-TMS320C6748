#include <stdio.h>
#include <math.h>
#include <string.h>
#include "input_image.h"
#include "huff.h"

#define pi 3.142857
const int N = 256;

float dctc[8][8];
float dctct[8][8];

// Function to find discrete cosine transform and print it
void dctTransform() {
    int i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if(i == 0){
                dctc[i][j] = 1 / sqrt(8);
            } else {
                dctc[i][j] = (sqrt(2) / sqrt(8)) * cos((2 * j + 1) * i * pi / (2 * 8));
            }
            dctct[j][i] = dctc[i][j];
        }
    }
}

int q[8][8] = {{16,11,10,16,24,40,51,61},{12,12,14,19,26,58,60,55},
               {14,13,16,24,40,57,69,56},{14,17,22,29,51,87,80,62},
               {18,22,37,56,68,109,103,77},{24,35,55,64,81,104,113,92},
               {149,64,78,87,103,121,120,101},{72,92,95,98,112,100,103,99}};

int im[256][256], im2[256][256];

int main() {
    dctTransform();

    int i, j, k;
    float dum[8][8], dum2[8][8];
    int dct[8][8], dct2[8][8];

    printf("Discrete Cosine Transform Coefficient Matrix\n");
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            printf("%f ", dctc[i][j]);
        }
        printf("\n");
    }

    printf("Quantization Q50 Matrix\n");
    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            printf("%d ", q[i][j]);
        }
        printf("\n");
    }

    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            im[i][j] -= 128;
        }
    }

    int r = 0, c = 0;
    for(r = 0; r < N; r += 8){
        for(c = 0; c < N; c += 8){
            for(i = 0; i < 8; i++){
                for(j = 0; j < 8; j++){
                    dum[i][j] = 0;
                    for(k = 0; k < 8; k++){
                        dum[i][j] += dctc[i][k] * im[r + k][c + j];
                    }
                }
            }

            for(i = 0; i < 8; i++){
                for(j = 0; j < 8; j++){
                    dct[i][j] = 0;
                    for(k = 0; k < 8; k++){
                        dct[i][j] += dum[i][k] * dctct[k][j];
                    }
                    dct[i][j] = round((double)dct[i][j] / (double)q[i][j]);
                    im[r + i][c + j] = dct[i][j];
                }
            }
        }
    }

    for(r = 0; r < N; r += 8){
        for(c = 0; c < N; c += 8){
            for(i = 0; i < 8; i++){
                for(j = 0; j < 8; j++){
                    im[r + i][c + j] = round((double)im[r + i][c + j] * (double)q[i][j]);
                }
            }
        }
    }

    for(r = 0; r < N; r += 8){
        for(c = 0; c < N; c += 8){
            for(i = 0; i < 8; i++){
                for(j = 0; j < 8; j++){
                    dum2[i][j] = 0;
                    for(k = 0; k < 8; k++){
                        dum2[i][j] += dctct[i][k] * im[r + k][c + j];
                    }
                }
            }

            for(i = 0; i < 8; i++){
                for(j = 0; j < 8; j++){
                    dct2[i][j] = 0;
                    for(k = 0; k < 8; k++){
                        dct2[i][j] += dum2[i][k] * dctc[k][j];
                    }
                    im2[r + i][c + j] = dct2[i][j] + 128;
                }
            }
        }
    }

    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            im[i][j] += 128;
        }
    }

    // Huffman Coding steps
    int maxval = 256;
    int hist[maxval];
    for(i = 0; i < maxval; i++){
        hist[i] = 0;
    }

    for(i = 0; i < 256; i++){
        for(j = 0; j < 256; j++){
            hist[255 + im[i][j]]++;
        }
    }

    // Frequency calculation and Huffman encoding implementation
    // Fib function, Huffman tree generation, and encoding steps omitted here for brevity

    // Output final results
    printf("Pixel Values after Decompression\n");
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            printf("%d ", im2[i][j]);
        }
        printf("\n");
    }

    return 0;
}
