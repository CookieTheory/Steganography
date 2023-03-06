#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "slike.h"
#include "stb_image.h"
#include "stb_image_write.h"

	Image::Image(const char* filename){
		if(read(filename)){
			printf("Ocitan %s\n", filename);
			size = w*h*channels;
		}
		else{
			printf("Failed to read %s\n", filename);
		}
	}
	
	Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels) {
		size = w*h*channels;
		data = new uint8_t[size];
	}
	
	Image::Image(const Image& img) : Image(img.w, img.h, img.channels) {
		memcpy(data, img.data, size);
	}
	
	Image::~Image(){
		stbi_image_free(data);
	}
	
	bool Image::read(const char* filename){
		data = stbi_load(filename, &w, &h, &channels, 0);
		return data != NULL;
	}
	
	bool Image::write(const char* filename){
		ImageType type = getFileType(filename);
		int success;
		switch(type){
			case PNG:
				success = stbi_write_png(filename, w, h, channels, data, w*channels);
				break;
			case BMP:
				success = stbi_write_bmp(filename, w, h, channels, data);
				break;
			case JPG:
				success = stbi_write_jpg(filename, w, h, channels, data, 100);
				break;
			case TGA:
				success = stbi_write_tga(filename, w, h, channels, data);
				break;
		}
		return success != 0;
	}
	
	ImageType Image::getFileType(const char* filename){
		const char* ext = strrchr(filename, '.');
		if(ext != nullptr){
			if(strcmp(ext, ".png") == 0) {
				return PNG;
			}
		}
		else if(ext != nullptr){
			if(strcmp(ext, ".jpg") == 0) {
				return JPG;
			}
		}
		else if(ext != nullptr){
			if(strcmp(ext, ".bmp") == 0) {
				return BMP;
			}
		}
		else if(ext != nullptr){
			if(strcmp(ext, ".tga") == 0) {
				return TGA;
			}
		}
		return PNG;
	}
	
	Image& Image::encodeMessage(const char* message){
		uint32_t len = strlen(message) * 8;
		
		if(len + STEG_HEADER_SIZE > size) {
			printf("\e[31m[ERROR] Ova poruka je prevelika (%llu bitova / %zu bitova)\e[0m\n", len+STEG_HEADER_SIZE, size);
			return *this;
		}
		
		for(uint8_t i = 0; i < STEG_HEADER_SIZE; i++){
			data[i] &= 0xFE;
			data[i] |= (len >> (STEG_HEADER_SIZE -1 - i)) & 1UL;
		}
		
		for(uint32_t i = 0; i < len; i++){
			data[i+STEG_HEADER_SIZE] &= 0xFE;
			data[i+STEG_HEADER_SIZE] |= (message[i/8] >> ((len-1-i)%8)) & 1;
		}
		
		return *this;
	}
	
	Image& Image::decodeMessage(char* buffer, size_t* messageLength){
		uint32_t len = 0;
		for(uint8_t i = 0; i < STEG_HEADER_SIZE; i++){
			len = (len << 1) | (data[i] & 1);
		}
		*messageLength = len / 8;
		
		for(uint32_t i = 0; i < len; i++) {
			buffer[i/8] = (buffer[i/8] << 1) | (data[i+STEG_HEADER_SIZE] & 1);
		}
		
		return *this;
	}