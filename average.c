#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

#define BYTE    unsigned char
#define H 2199
#define W 3136

void main() {

	BYTE *inImg = malloc(H*W);
	BYTE *outImg = malloc(H*W);
	BYTE *avgImg = malloc(H*W);
	BYTE *avgImg2 = malloc(H*W);
	memset(avgImg, 0, H*W);
	memset(avgImg2, 0, H*W);
	FILE *fp;

	//File Read
	fp = fopen("Snow.raw", "rb");
	fread(inImg, sizeof(BYTE), H*W, fp);
	fclose(fp);

	clock_t start2d, end2d, start1d, end1d;

	// ****** 2D 21x21 average filter ******
	start2d = clock();
	for (int l = 0; l < H; l++) { // 처리가 되지 않는 곳
		for (int p = 0; p < W; p++) {
			for (int r = 0; r < 10; r++) {
				avgImg[r*W + p] = inImg[r*W + p];
				avgImg[(H-r-1)*W + p] = inImg[(H-r-1)*W + p];
				avgImg[l*W + r] = inImg[l*W + r];
				avgImg[l*W + W-r-1] = inImg[l*W + W-r-1];
			}
		}
	}

	for (int i = 10; i < H-10; i++) {
		for (int j = 10; j < W-10; j++) {
			int temp = 0;

			for (int k = 0; k < 11; k++) {
				for (int p = 0; p < 11; p++) {
					if (k == 0) temp += inImg[i*W + j];
					else {
						temp += inImg[(i - k)*W + j - p];
						temp += inImg[(i - k)*W + j + p];

						temp += inImg[(i + k)*W + j + p];
						temp += inImg[(i + k)*W + j - p];
					}
				}
				temp += inImg[(i - k)*W + j];
				temp += inImg[(i + k)*W + j];

			}
			
			int avg = (int)((double)temp/(21*21));
			avgImg[i*W + j] = avg;
		}
	}
	end2d = clock();

	// ****** 1D 1x21 average filter ******
	start1d = clock();
	for (int i = 0; i < H; i++) { // 수평 처리
		for (int k = 0; k < 10; k++) {
			avgImg[i*W + k] = inImg[i*W + k];
			avgImg[i*W + W - k - 1] = inImg[i*W + W - k - 1];
		}
		for (int j = 10; j < W - 10; j++) {
			int temp = 0;
			
			for (int l = 0; l < 11; l++) {
				if (l==0) temp += inImg[i*W + j];
				else {
					temp += inImg[i*W + j - l];
					temp += inImg[i*W + j + l];
				}
			}

			int avg = (int)((double)temp / 21);
			avgImg[i*W + j] = avg;
		}
	}
		
	for (int j = 0; j < W; j++) { // 수직 처리
		for (int i = 10; i < H - 10; i++) {
			for (int k = 0; k < 10; k++) {
				avgImg2[k*W+j] = avgImg[k*W + j];
				avgImg2[(H-k-1)*W + j] = avgImg[(H - k - 1)*W + j];
			}

			int temp = 0; // 평균값 구하기
			for (int l = 0; l < 11; l++) {
				if (l == 0) temp += avgImg[i*W + j];
				else {
					temp += avgImg[(i-l)*W + j];
					temp += avgImg[(i+1)*W + j];
				}
			}

			int avg = (int)((double)temp / 21);
			avgImg2[i*W + j] = avg;
		}
	}
	end1d = clock();


	printf("2D 수행 시간 : %f sec", (float)(end2d - start2d) / CLOCKS_PER_SEC);
	printf("1D 수행 시간 : %f sec", (float)(end1d - start1d) / CLOCKS_PER_SEC);

	//File Write
	fp = fopen("avg-2D.pgm", "wb"); //21x21
	//fp = fopen("avg-1D.pgm", "wb"); //1x21
	fprintf(fp, "P5\n");//magic no.
	fprintf(fp, "%d %d\n", W, H);
	fprintf(fp, "255\n");//max. value
	fwrite(avgImg, sizeof(char), H*W, fp); //21x21
	//fwrite(avgImg2, sizeof(char), H*W, fp); //1x21
	fclose(fp);
}