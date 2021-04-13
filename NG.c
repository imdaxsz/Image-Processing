#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define BYTE    unsigned char
#define H 512
#define W 512

int NG[H][W];
int hist[256];

void main() {
	BYTE *inImg = malloc(H*W);
	BYTE *outImg = malloc(H*W);
	memset(outImg, 0, H*W);

	FILE *fp;

	//File Read
	fp = fopen("Lena.raw", "rb");
	fread(inImg, sizeof(BYTE), H*W, fp);
	fclose(fp);

	// Nonlinear Gradient 规过 1
	int max;
	for (int i = 2; i < H - 2; i++)
		for (int j = 2; j < W - 2; j++) {
			max = 0;
			for (int k = -2; k < 3; k++)
				for (int l = -2; l < 3; l++) {
					if (max < inImg[(i + k)*W + j + l])
						max = inImg[(i + k)*W + j + l];
				}
			NG[i][j] = max - inImg[i*W + j];
			outImg[i*W + j] = NG[i][j];
		}

	// Nonlinear Gradient 规过 2
	int min;
	for (int i = 2; i < H - 2; i++)
		for (int j = 2; j < W - 2; j++) {
			min = 1000;
			for (int k = -2; k < 3; k++)
				for (int l = -2; l < 3; l++) {
					if (min > inImg[(i + k)*W + j + l])
						min = inImg[(i + k)*W + j + l];
				}
			NG[i][j] = inImg[i*W + j] - min;
			outImg[i*W + j] = NG[i][j];
		}

	//Histogram
	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
			hist[outImg[i*W + j]]++;

	//30% 版拌 眠免
	int cnt = 0, tmp = 255;
	for (int i = 255; i >= 0; i--) {
		cnt += hist[i];
		if (cnt > H*W*0.3) {
			if (i != 255) tmp = i + 1;
			else tmp = 255;
			break;
		}
	}

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++) {
			if (outImg[i*W + j] >= tmp)
				outImg[i*W + j] = 255;
		}

	//File Write
	fp = fopen("NG.pgm", "wb");
	fprintf(fp, "P5\n");//magic no.
	fprintf(fp, "%d %d\n", W, H);
	fprintf(fp, "255\n");//max. value
	fwrite(outImg, sizeof(char), H*W, fp);
	fclose(fp);
}