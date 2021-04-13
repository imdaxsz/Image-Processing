#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define BYTE    unsigned char
#define H 243
#define W 303

int hist[256];
float mean, bclass_var[256];

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

	for (int i = 0; i < 256; i++)
		mean += (float)(i*hist[i]);
	mean = mean / (float)(H*W);

	//otsu
	float p1 = 0, m1 = 0, m2 = 0, p=0, m=0;
	for (int th = 0; th < 256; th++) {
		p1 = p + (float)hist[th] / (float)(H*W);
		if (p1 == 0.) m1 = 0;
		else m1 = (p*m + (float)(th*hist[th]) / (float)(H*W))/p1;
		if (p1 == 1.) m2 = 0;
		else m2 = (mean - p1 * m1) / (1. - p1);
		bclass_var[th] = p1 * (m1 - mean)*(m1 - mean) + (1. - p1)*(m2 - mean)*(m2 - mean);
		p = p1;
		m = m1;
	}

	float max = 0;
	for (int th = 0; th < 256; th++) {
		if (max < bclass_var[th]) {
			max = bclass_var[th];
			thres = th;
		}
	}

	printf("\nThreshold : %d\n", thres);
	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++) {
			if (inImg[i*W + j] < thres)
				inImg[i*W + j] = 0;
			else inImg[i*W + j] = 255;
		}

	//File Write
	fp = fopen("otsu.pgm", "wb");
	fprintf(fp, "P5\n");//magic no.
	fprintf(fp, "%d %d\n", W, H);
	fprintf(fp, "255\n");//max. value
	fwrite(inImg, sizeof(char), H*W, fp);
	fclose(fp);
}