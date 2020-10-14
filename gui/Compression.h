//***************************************************************************************

#pragma once

//***************************************************************************************

BOOL Compress(const void* pData, const int DataSize, void* pZip, int* pZipSize);

int GetDecompressedSize(const void* pZip, const int ZipSize);
BOOL Decompress(const void* pZip, const int ZipSize, void* pData);

//***************************************************************************************
