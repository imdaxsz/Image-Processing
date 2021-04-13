#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define BYTE    unsigned char
#define H 512
#define W 512

int sob[H][W];
int sx[3][3] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
int sy[3][3] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };

int hist[256];

void main() {
	BYTE *inImg = malloc(H*W);
	BYTE *outImg = malloc(H*W);
	memset(outImg, 0, H*W);

	int max = 0, min = 1000;
	FILE *fp;

	//File Read
	fp = fopen("Lena.raw", "rb");
	fread(inImg, sizeof(BYTE), H*W, fp);
	fclose(fp);

	// Sobel
	int dx, dy;
	for (int i = 1; i < H - 1; i++)
		for (int j = 1; j < W - 1; j++) {
			dx = dy = 0;
			for (int k = -1; k < 2; k++)
				for (int l = -1; l < 2; l++) {
					dx += sx[k + 1][l + 1] * inImg[(i + k)*W + j + l];
					dy += sy[k + 1][l + 1] * inImg[(i + k)*W + j + l];
				}
			sob[i][j] = abs(dx)+abs(dy);
		}

	//Find min, max
	for (int i = 1; i < H-1; i++)
		for (int j = 1; j < W-1; j++) {
			if (sob[i][j] > max) max = sob[i][j];
			if (sob[i][j] < min) min = sob[i][j];
		}

	//Normalization
	for (int i = 1; i < H-1; i++)
		for (int j = 1; j < W-1 ; j++)
			outImg[i*W+j] = (BYTE)(255.*(sob[i][j] - min) / (max - min));

	//Histogram
	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
			hist[outImg[i*W + j]]++;

	//30% 경계 추출
	int cnt = 0, tmp = 255;
	for (int i = 255; i >= 0; i--) {
		cnt += hist[i];
		if (cnt > H*W*0.3) {
			if(i!=255) tmp = i+1;
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
	fp = fopen("Sobel.pgm", "wb");
	fprintf(fp, "P5\n");//magic no.
	fprintf(fp, "%d %d\n", W, H);
	fprintf(fp, "255\n");//max. value
	fwrite(outImg, sizeof(char), H*W, fp);
	fclose(fp);
}