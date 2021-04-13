#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define BYTE    unsigned char
#define H 243
#define W 303

int hist[256];

void main() {
	BYTE *inImg = malloc(H*W);

	FILE *fp;
	int th=0, thres = 128;

	//File Read
	fp = fopen("hand.raw", "rb");
	fread(inImg, sizeof(BYTE), H*W, fp);
	fclose(fp);

	//Histogram
	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
			hist[inImg[i*W + j]]++;

	//iteration
	float m1 = 0, m2 = 0;
	int sum1 = 0, sum2 = 0, it=0;

	while (thres != th) {
		it++;
		for (int i = 0; i <= thres; i++) {
			m1 += (float)(i*hist[i]);
			sum1 += hist[i];
		}
		m1 = m1 / (float)sum1;

		sum2 = H * W - sum1;
		for (int i = thres + 1; i < 256; i++)
			m2 += (float)(i*hist[i]);
		m2 = m2 / sum2;
		th = thres;
		thres = (int)((m1 + m2) / (float)2);
		m1 = m2 = 0;
		sum1 = sum2 = 0;
	}

	printf("\nThreshold : %d\n", thres);
	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++) {
			if (inImg[i*W + j] < thres)
				inImg[i*W + j] = 0;
			else inImg[i*W + j] = 255;
		}

	//File Write
	fp = fopen("iterative.pgm", "wb");
	fprintf(fp, "P5\n");//magic no.
	fprintf(fp, "%d %d\n", W, H);
	fprintf(fp, "255\n");//max. value
	fwrite(inImg, sizeof(char), H*W, fp);
	fclose(fp);
}