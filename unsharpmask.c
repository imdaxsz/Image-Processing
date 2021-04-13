#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define BYTE    unsigned char
#define H 512
#define W 512

void main() {

	BYTE *inImg = malloc(H*W);
	BYTE *tempImg = malloc(H*W);
	BYTE *outImg = malloc(H*W);
	memset(tempImg, 0, H*W);
	memset(outImg, 0, H*W);

	FILE *fp;

	//File Read
	fp = fopen("Pentagon.raw", "rb");
	fread(inImg, sizeof(BYTE), H*W, fp);
	fclose(fp);

	// ****** unsharp mask ******
	BYTE *avgImg = malloc(H*W);

	for (int i = 0; i < H; i++) { // 수평 처리
		tempImg[i*W] = inImg[i*W]; // 처리되지 않는 부분
		tempImg[i*W + 1] = inImg[i*W + 1];
		tempImg[i*W + W - 1] = inImg[i*W + W - 1];
		tempImg[i*W + W - 2] = inImg[i*W + W - 2];
		for (int j = 2; j < W - 2; j++) {
			int temp = 0;
			temp += inImg[i*W + j - 2]; // 평균값 구하기
			temp += inImg[i*W + j - 1];
			temp += inImg[i*W + j];
			temp += inImg[i*W + j + 1];
			temp += inImg[i*W + j + 2];

			int avg = (int)((double)temp / 5);
			tempImg[i*W + j] = avg;
		}
	}

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
			tempImg[i*W + j] = (int)((double)inImg[i*W + j] + 0.3*(inImg[i*W + j] - tempImg[i*W + j]));
	
		
	for (int j = 0; j < W; j++) { // 수직 처리
		for (int i = 2; i < H - 2; i++) {
			avgImg[0 * W + j] = tempImg[0 * W + j]; // 처리되지 않는 부분
			avgImg[W + j] = tempImg[W + j];
			avgImg[(H - 1)*W + j] = tempImg[(H - 1)*W + j];
			avgImg[(H - 2)*W + j] = tempImg[(H - 2)*W + j];

			int temp = 0; // 평균값 구하기
			temp += tempImg[(i - 2)*W + j];
			temp += tempImg[(i - 1)*W + j];
			temp += tempImg[i*W + j];
			temp += tempImg[(i + 1)*W + j];
			temp += tempImg[(i + 2)*W + j];

			int avg = (int)((double)temp / 5);
			avgImg[i*W + j] = avg;
		}
	}

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
			outImg[i*W + j] = (int)((double)tempImg[i*W + j] + 0.3*(tempImg[i*W + j] - avgImg[i*W + j]));


	//File Write
	fp = fopen("UnsharpMasking.pgm", "wb");
	fprintf(fp, "P5\n");//magic no.
	fprintf(fp, "%d %d\n", W, H);
	fprintf(fp, "255\n");//max. value
	fwrite(outImg, sizeof(char), H*W, fp);
	fclose(fp);
}