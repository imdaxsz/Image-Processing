//#define _CRT_SECURE_NO_WARNINGS
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <memory.h>
//
//#define BYTE    unsigned char
//#define H 512
//#define W 512
//
//void main() {
//
//	BYTE *inImg = malloc(H*W);
//	BYTE *tempImg = malloc(H*W);
//	BYTE *outImg = malloc(H*W);
//	memset(tempImg, 0, H*W);
//	memset(outImg, 0, H*W);
//
//	FILE *fp;
//
//	//File Read
//	fp = fopen("Lena.raw", "rb");
//	fread(inImg, sizeof(BYTE), H*W, fp);
//	fclose(fp);
//
//	// ****** salt&pepper 잡음 생성 ******
//	for (int i = 0; i < H*W*0.05; i++) {//5% 잡음
//		int x = rand() % W; //generate x coordinate
//		int y = rand() % H; //generate y coordinate
//		int SorP = rand() % 2; //salt or pepper?
//		if (SorP) inImg[y*W + x] = 255; //salt
//		else inImg[y*W + x] = 0; //pepper
//	}
//
//	// ****** median filter ******
//	int med = 0;
//	int temp[5] = { 0,0,0,0,0 };
//
//	for (int i = 0; i < H; i++) { // 수평 처리
//		tempImg[i*W] = inImg[i*W];
//		tempImg[i*W+1] = inImg[i*W+1];
//		tempImg[i*W+W-1] = inImg[i*W+W-1];
//		tempImg[i*W+W-2] = inImg[i*W+W-2];
//		for (int j = 2; j < W - 2; j++) {
//			temp[0] = inImg[i*W + j - 2];
//			temp[1] = inImg[i*W + j - 1];
//			temp[2] = inImg[i*W + j];
//			temp[3] = inImg[i*W + j + 1];
//			temp[4] = inImg[i*W + j + 2];
//
//			for (int i = 0; i < 4; i++) // 크기 순으로 정렬
//				for (int j = i + 1; j < 5; j++)
//					if (temp[i] < temp[j])
//					{
//						int a = temp[i];
//						temp[i] = temp[j];
//						temp[j] = a;
//					}
//
//			med = temp[2]; // 중간값 찾기
//			tempImg[i*W + j] = med;
//		}
//	}
//	for (int j = 0; j < W; j++) { // 수직 처리
//		for (int i = 2; i < H - 2; i++) {
//			outImg[0*W+j] = tempImg[0*W+j];
//			outImg[W+j] = tempImg[W+j];
//			outImg[(H-1)*W+j] = tempImg[(H-1)*W+j];
//			outImg[(H-2)*W+j] = tempImg[(H-2)*W+j];
//			temp[0] = tempImg[(i-2)*W + j];
//			temp[1] = tempImg[(i-1)*W + j];
//			temp[2] = tempImg[i*W + j];
//			temp[3] = tempImg[(i+1)*W + j];
//			temp[4] = tempImg[(i+2)*W + j];
//
//			for (int i = 0; i < 4; i++) // 크기 순으로 정렬
//				for (int j = i + 1; j < 5; j++)
//					if (temp[i] < temp[j])
//					{
//						int a = temp[i];
//						temp[i] = temp[j];
//						temp[j] = a;
//					}
//
//			med = temp[2]; // 중간값 찾기
//			outImg[i*W + j] = med;
//		}
//	}
//
//	//File Write
//	fp = fopen("median.pgm", "wb");
//	fprintf(fp, "P5\n");//magic no.
//	fprintf(fp, "%d %d\n", W, H);
//	fprintf(fp, "255\n");//max. value
//	fwrite(outImg, sizeof(char), H*W, fp);
//	fclose(fp);
//}