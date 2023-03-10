#include <stdint.h>
#include <cstdio>

#define STEG_HEADER_SIZE sizeof(uint32_t) * 8

enum ImageType {
	PNG, JPG, BMP, TGA
};

struct Image{
	uint8_t* data = NULL;
	size_t size = 0;
	int w;
	int h;
	int channels;
	
	Image(const char* filename);
	Image(int w, int h, int channels);
	Image(const Image& img);
	~Image();
	
	bool read(const char* filename);
	bool write(const char* filename);
	
	ImageType getFileType(const char* filename);
	
	Image& encodeMessage(const char* message);
	
	Image& decodeMessage(char* buffer, size_t* messageLength);
};