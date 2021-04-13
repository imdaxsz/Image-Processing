#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define BYTE    unsigned char
#define H 512
#define W 512

void main() {

	BYTE *inImg = malloc(H*W);
	BYTE *outImg = malloc((2*H)*(2*W));
	BYTE *outImg2 = malloc((4 * H)*(4 * W));
	memset(outImg, 0, 2*H*2*W);
	memset(outImg2, 0, 4 * H * 4 * W);
	FILE *fp;

	//File Read
	fp = fopen("Lena.raw", "rb");
	fread(inImg, sizeof(BYTE), H*W, fp);
	fclose(fp);


	// ****** zero-order interpolation ******
	int H1 = 2 * H;
	int W1 = 2 * W;

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
			outImg[(2 * i*W1) + (2 * j)] = inImg[i*W + j];

	for (int i = 0; i < H1; i++)
		for (int j = 1; j < W1; j++)
			if (j % 2 == 1) outImg[i*W1 + j] = outImg[i*W1 + j - 1];

	for (int i = 1; i < H1; i++)
		for (int j = 0; j < W1; j++)
			if (i % 2 == 1) outImg[i*W1 + j] = outImg[(i - 1)*W1 + j];


	// ****** first-order interpolation ******
	int H2 = 4 * H;
	int W2 = 4 * W;
	
	for (int i = 0; i < H1; i++)
		for (int j = 0; j < W1; j++)
			outImg2[(2*i*W2) + (2*j)] = outImg[i*W1 + j];

	for (int i = 0; i < H2; i++)
		for (int j = 0; j < W2; j++) {
			if (j == W2 - 1) outImg2[i*W2 + j] = outImg2[i*W2 + j - 1];
			else if (j % 2 == 1) outImg2[i*W2 + j] = (double)(outImg2[i*W2 + j - 1] + outImg2[i*W2 + j + 1]) / 2;
		}

	for (int i = 1; i < H2; i++)
		for (int j = 0; j < W2; j++) {
			if (i == H2 - 1) outImg2[i*W2 + j] = outImg2[(i - 1)*W2 + j];
			else if (i % 2 == 1) outImg2[i*W2 + j] = (double)(outImg2[(i - 1)*W2 + j] + outImg2[(i + 1)*W2 + j]) / 2;
		}

	for (int i = 1; i < H2; i++)
		for (int j = 0; j < W2; j++)
			if (j == W2 - 1) outImg2[i*W2 + j] = outImg2[i*W2 + j - 1];

	//File Write
	fp = fopen("interpolation.pgm", "wb");
	fprintf(fp, "P5\n");//magic no.
	fprintf(fp, "%d %d\n", W2, H2);
	fprintf(fp, "255\n");//max. value
	fwrite(outImg2, sizeof(char), H2*W2, fp);
	fclose(fp);
}