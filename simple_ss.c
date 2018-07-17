#include "simple_ss.h"

int takeScreenshot (char *file_name) {
    FILE *img_file = NULL;
    LPVOID buffer = NULL;
    BITMAPINFO bmpInfo;
    BITMAPFILEHEADER bmpFileHeader;
   	HDC hdc = NULL;
   	HBITMAP hBMP;

    keybd_event (VK_SNAPSHOT, 69, KEYEVENTF_EXTENDEDKEY | 0 , 0);
	keybd_event (VK_SNAPSHOT, 69, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);

	Sleep (100);
	OpenClipboard (NULL);
	hBMP = (HBITMAP) GetClipboardData (CF_BITMAP);
    CloseClipboard ();
    hdc = GetDC (NULL);
    ZeroMemory (&bmpInfo, sizeof (BITMAPINFO));
    bmpInfo.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
    GetDIBits (hdc, hBMP, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);

    if (bmpInfo.bmiHeader.biSizeImage <= 0) {
        bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth * abs (bmpInfo.bmiHeader.biHeight) * (bmpInfo.bmiHeader.biBitCount + 7) / 8;
    }
    if ((buffer = malloc (bmpInfo.bmiHeader.biSizeImage)) == NULL) {
        return (-1);
    }
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    GetDIBits (hdc, hBMP, 0, bmpInfo.bmiHeader.biHeight, buffer, &bmpInfo, DIB_RGB_COLORS);
    if ((img_file = fopen (file_name, "wb")) == NULL) {
        return (-1);
    }
    bmpFileHeader.bfReserved1 = 0;
    bmpFileHeader.bfReserved2 = 0;
    bmpFileHeader.bfSize = sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER) + bmpInfo.bmiHeader.biSizeImage;
    bmpFileHeader.bfType = (DWORD) 'MB';
    bmpFileHeader.bfOffBits = sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER);
    fwrite (&bmpFileHeader, sizeof (BITMAPFILEHEADER), 1, img_file);
    fwrite (&bmpInfo.bmiHeader, sizeof (BITMAPINFOHEADER), 1, img_file);
    fwrite (buffer, bmpInfo.bmiHeader.biSizeImage, 1, img_file);

    if (hdc) {
        ReleaseDC (NULL,hdc);
    }
    if (buffer) {
        free (buffer);
    }
    if (img_file) {
        fclose (img_file);
    }
    return (0);
}
