#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#define WIDTHBYTES(bits) (((bits)+31)/32*4)
#define BYTE    unsigned char


void BmptoPgm()
{
	FILE *infile;

	if ((infile = fopen("ImgBW1.bmp", "rb")) == NULL) { 
			printf("No Image File");
		return; 
	}

	// BMP Header Information
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile);//14 bytes
	//if (hf.bfType != 0x4D42) exit(1);//"BM"
	printf("bfType : 0x%x\n",hf.bfType);
	printf("bfSize : %ldBytes\n", hf.bfSize);
	printf("bfReserved1 : %x\n", hf.bfReserved1);
	printf("bfReserved2 : %x\n", hf.bfReserved2);
	printf("bfOffBits : %ldBytes = 14 + 40 + %ld(Pallete size)\n\n", hf.bfOffBits, hf.bfOffBits-54);

	_getch();

	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile);//40 bytes
//	if (hInfo.biBitCount != 8) { printf("Bad File Format!!"); return; }//BW
	printf("biSize : %ldBytes\n", hInfo.biSize);
	printf("Image Size : %ld x %ld -> %ld x %ld\n", hInfo.biWidth, hInfo.biHeight, WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth), hInfo.biHeight);
	printf("biPlanes : %d\n", hInfo.biPlanes);
	printf("biBitCount : %d\n", hInfo.biBitCount);
	printf("biCompression : %ld\n", hInfo.biCompression);
	printf("biSizeImage : %ldBytes\n", hInfo.biSizeImage);
	printf("biXPelsPerMeter : %ld\n", hInfo.biXPelsPerMeter);
	printf("biYPelsPerMeter : %ld\n", hInfo.biYPelsPerMeter);
	printf("biClrUsed : %ld\n", hInfo.biClrUsed);
	printf("biClrImportant : %ld\n\n", hInfo.biClrImportant);

	_getch();
	// BMP Pallete
	RGBQUAD hRGB[256];
	printf("sizeofRGBQUAD : %d\n", sizeof(RGBQUAD));
	_getch();
	fread(hRGB, sizeof(RGBQUAD), 256, infile);//read color pallete
	for (int i = 0; i < (int)pow(2, hInfo.biBitCount); i++) {
		printf("[%3d] B:%3d G:%3d R:%3d Res:%3d\n",i, hRGB[i].rgbBlue, hRGB[i].rgbGreen, hRGB[i].rgbRed, hRGB[i].rgbReserved);
		//_getch();
	}

	
	//Memory
	//BYTE *lpImg = new BYTE[hInfo.biSizeImage];//for c++
	BYTE *lpImg = malloc(hInfo.biSizeImage);//for c
	fread(lpImg, sizeof(char), hInfo.biSizeImage, infile);
	fclose(infile);

	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);

	//Image Output
	FILE *outfile = fopen("ImgBW.pgm", "wb");
	fprintf(outfile, "P5\n");//magic no.
	fprintf(outfile, "%d %d\n", hInfo.biWidth, hInfo.biHeight);
	fprintf(outfile, "%d\n", 255);//max. value


	// 행 순서 유지
	//for (int i = 0; i < hInfo.biHeight;i++)
	//	fwrite(&lpImg[i*rwsize], sizeof(char), hInfo.biWidth, outfile);

	// 행 순서 변환
	for (int i = hInfo.biHeight - 1; i >= 0; i--)
		fwrite(&lpImg[i*rwsize], sizeof(char), hInfo.biWidth, outfile);

	free(lpImg);
	fclose(outfile);
}



void BmptoPpm()
{
	FILE *infile;

	if ((infile = fopen("ImgColor.bmp", "rb")) == NULL) {
			printf("No Image File");
		return;
	}

	// BMP Header Information
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile);//14 bytes
	if (hf.bfType != 0x4D42) exit(1);//"BM"
	printf("\n\nbfType : 0x%x\n", hf.bfType);
	printf("bfSize : %ldBytes\n", hf.bfSize);
	printf("bfReserved1 : %x\n", hf.bfReserved1);
	printf("bfReserved2 : %x\n", hf.bfReserved2);
	printf("bfOffBits : %ldBytes = 14 + 40 + %ld(Pallete size)\n\n", hf.bfOffBits, hf.bfOffBits - 54);

	_getch();

	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile);//40 bytes
	if (hInfo.biBitCount != 24) { printf("Bad File Format!!"); return; }//24-bit
	printf("biSize : %ldBytes\n", hInfo.biSize);
	printf("Image Size : %ld x %ld -> %ld x %ld\n", hInfo.biWidth, hInfo.biHeight, WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth), hInfo.biHeight);
	printf("biPlanes : %d\n", hInfo.biPlanes);
	printf("biBitCount : %d\n", hInfo.biBitCount);
	printf("biCompression : %ld\n", hInfo.biCompression);
	printf("biSizeImage : %ldBytes\n", hInfo.biSizeImage);
	printf("biXPelsPerMeter : %ld\n", hInfo.biXPelsPerMeter);
	printf("biYPelsPerMeter : %ld\n", hInfo.biYPelsPerMeter);
	printf("biClrUsed : %ld\n", hInfo.biClrUsed);
	printf("biClrImportant : %ld\n\n", hInfo.biClrImportant);

	_getch();

	// BMP Pallete, There's no pallete in the true color.
	//RGBQUAD hRGB[256];
	//fread(hRGB, sizeof(RGBQUAD), 256, infile);//read color pallete

	// Memory
	BYTE *lpImg = malloc(hInfo.biSizeImage*3);//for c
	fread(lpImg, sizeof(char), hInfo.biSizeImage, infile);
	fclose(infile);

	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);

	//Image Output
	FILE *outfile = fopen("ImgBW.ppm", "wb");
	fprintf(outfile, "P6\n");//magic no.
	fprintf(outfile, "%d %d\n", hInfo.biWidth, hInfo.biHeight);
	fprintf(outfile, "%d\n", 255);//max. value


	// BGR -> RGB 변환
	int temp;
	for (int i = 0; i < hInfo.biHeight; i++)
		for (int j = 0; j < hInfo.biWidth; j++)
			for (int k = 0; k < 3; k++)
				if (k == 0) {
					temp = lpImg[i*rwsize + j * 3];
					lpImg[i*rwsize + j * 3] = lpImg[i*rwsize + j * 3 + 2];
					lpImg[i*rwsize + j * 3 + 2] = temp;
				}


	// 행 순서 유지
	//for (int i = 0; i < hInfo.biHeight; i++)
	//	fwrite(&lpImg[i*rwsize], sizeof(char), hInfo.biWidth * 3, outfile);

	// 행 순서 변환
	for (int i = hInfo.biHeight - 1; i >= 0; i--)
		fwrite(&lpImg[i*rwsize], sizeof(char), hInfo.biWidth * 3, outfile);

	free(lpImg);
	fclose(outfile);

}

void main()
{
	BmptoPgm();
	BmptoPpm();
}
