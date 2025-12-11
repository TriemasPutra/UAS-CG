#include <iostream>
#include <ostream>
#include <string>
#include <fstream>
#include "include/getBMP.h"

imageFile *getBMP(std::string fileName)
{
    int offset, w, h;
    imageFile *tempStore = new imageFile;
    imageFile *outRGB = new imageFile;
    imageFile *outRGBA = new imageFile;

    std::ifstream inFile(fileName.c_str(), std::ios::binary);

    inFile.seekg(10);
    inFile.read((char*)&offset, 4);

    inFile.seekg(18);
    inFile.read((char*)&w, 4);
    inFile.read((char*)&h, 4);

    int padding = (3 * w) % 4 ? 4 - (3 * w) % 4 : 0;
    tempStore->data = new unsigned char[(3 * w + padding) * h];
    inFile.seekg(offset);
    inFile.read((char *)tempStore->data, (3 * w + padding) * h);
    inFile.close();

    outRGB->width = w;
    outRGB->height = h;
    outRGB->data = new unsigned char[3 * w * h];
    
    int tempStorePos = 0;
    int outRGBPos = 0;
    for(int j = 0; j < h; j++)
        for(int i = 0; i < 3 * w; i += 3)
        {
            tempStorePos = (3 * w + padding) * j + i;
            outRGBPos = 3 * w * j + i;
            outRGB->data[outRGBPos] = 
                tempStore->data[tempStorePos + 2];
            outRGB->data[outRGBPos + 1] = 
                tempStore->data[tempStorePos + 1];
            outRGB->data[outRGBPos + 2] = 
                tempStore->data[tempStorePos];
        }

    outRGBA->width = w;
    outRGBA->height = h;
    outRGBA->data = new unsigned char[4 * w * h];

    for (int j = 0; j < 4 * w * h; j += 4)
    {
        outRGBA->data[j] = outRGB->data[(j / 4) * 3];
        outRGBA->data[j + 1] = outRGB->data[(j / 4) * 3 + 1];
        outRGBA->data[j + 2] = outRGB->data[(j / 4) * 3 + 2];
        outRGBA->data[j + 3] = 0xFF;
    }

    delete[] tempStore->data;
    delete tempStore;

    delete[] outRGB->data;
    delete outRGB;
    
    return outRGBA;
}
