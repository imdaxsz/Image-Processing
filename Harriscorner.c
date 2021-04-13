#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define BYTE    unsigned char
#define H 550
#define W 550

float R[H][W];
int sx[3][3] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
int sy[3][3] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
int Ix2[H][W], Iy2[H][W], Ixy[H][W];
int Gx2[H][W], Gy2[H][W], Gxy[H][W];

void main() {
	BYTE *inImg = malloc(H*W);
	BYTE *outImg = malloc(H*W);
	memset(outImg, 0, H*W);

	FILE *fp;

	//File Read
	fp = fopen("Ctest.raw", "rb");
	fread(inImg, sizeof(BYTE), H*W, fp);
	fclose(fp);

	// Sobel 연산
	int dx, dy;
	for (int i = 1; i < H - 1; i++)
		for (int j = 1; j < W - 1; j++) {
			dx = dy = 0;
			for (int k = -1; k < 2; k++) {
				for (int l = -1; l < 2; l++) {
					dx += sx[k + 1][l + 1] * inImg[(i + k)*W + j + l];
					dy += sy[k + 1][l + 1] * inImg[(i + k)*W + j + l];
				}
			}
			Ix2[i][j] = dx * dx;
			Iy2[i][j] = dy * dy;
			Ixy[i][j] = abs(dx)*abs(dy);
		}

	// 5x5 균일 가중치 평균 계산
	int sumx, sumy, sumxy;
	for (int i = 2; i < H - 2; i++)
		for (int j = 2; j < W - 2; j++) {
			sumx = sumy = sumxy = 0;
			for (int k = -2; k < 3; k++) {
				for (int l = -2; l < 3; l++) {
					sumx += Ix2[i + k][j + l];
					sumy += Iy2[i + k][j + l];
					sumxy += Ixy[i + k][j + l];
				}
			}
			Gx2[i][j] = sumx / 25;
			Gy2[i][j] = sumy / 25;
			Gxy[i][j] = sumxy / 25;
		}

	// Harris Corner Response Function 계산
	for (int i = 2; i < H - 2; i++) 
		for (int j = 2; j < W - 2; j++)
			R[i][j] = Gx2[i][j] * Gy2[i][j] - Gxy[i][j] * Gxy[i][j] - 0.05*(Gx2[i][j] + Gy2[i][j])*(Gx2[i][j] + Gy2[i][j]);

	// R[x][y]>0.01인 경우, Corner 출력
	for (int i = 2; i < H - 2; i++)
		for (int j = 2; j < W - 2; j++) {
			if (R[i][j] > 0.01)
				outImg[i*W + j] = 255;
		}

	fp = fopen("HarrisCorner.pgm", "wb");
	fprintf(fp, "P5\n");//magic no.
	fprintf(fp, "%d %d\n", W, H);
	fprintf(fp, "255\n");//max. value
	fwrite(outImg, sizeof(char), H*W, fp);
	fclose(fp);
}