#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BYTE    unsigned char
#define H 243
#define W 303

int hist[256];
float ent[256];

void main() {
	BYTE *inImg = malloc(H*W);

	FILE *fp;
	int thres = 0;

	//File Read
	fp = fopen("hand.raw", "rb");
	fread(inImg, sizeof(BYTE), H*W, fp);
	fclose(fp);

	//Histogram
	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
			hist[inImg[i*W + j]]++;

	float h1 = 0, h2 = 0, p = 0, max = 0;
	int sum1 = 0, sum2 = 0;

	//kapur
	for (int th = 0; th < 256; th++) {
		for (int i = 0; i <= th; i++)
			sum1 += hist[i];
		for (int i = 0; i <= th; i++) {
			p = (float)hist[i] / (float)sum1;
			if (p != 0)
				h1 += p * (-1) * log(p) / log(2);
		}
		sum2 = H * W - sum1;
		for (int i = th + 1; i < 256; i++) { // N
			p = (float)hist[i] / (float)sum2;
			if (p != 0)
				h2 += p * (-1) * log(p) / log(2);
		}
		ent[th] = h1 + h2;
		h1 = h2 = 0;
		sum1 = sum2 = 0;
	}

	for (int th = 0; th < 256; th++)
		if (max < ent[th]) {
			max = ent[th];
			thres = th;
		}

	printf("\nThreshold : %d\n", thres);
	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++) {
			if (inImg[i*W + j] < thres)
				inImg[i*W + j] = 0;
			else inImg[i*W + j] = 255;
		}

	//File Write
	fp = fopen("kapur.pgm", "wb");
	fprintf(fp, "P5\n");//magic no.
	fprintf(fp, "%d %d\n", W, H);
	fprintf(fp, "255\n");//max. value
	fwrite(inImg, sizeof(char), H*W, fp);
	fclose(fp);
}