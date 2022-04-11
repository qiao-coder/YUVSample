// YUVSample.cpp: 定义应用程序的入口点。
//

#include "YUVSample.h"
#include <fstream>

using namespace std;

unsigned char clip(int num) {
	if (num > 255) {
		return 255;
	}
	else if (num < 0) {
		return 0;
	}
	return num;
}

/**
 * @param in_path		The path of YUV444P file
 * @param out_path		The path of RGB24 file
 * @param w				The width of YUV444P file
 * @param h				The height of YUV444P file
 */
void yuv444_to_rgb24(string in_path, string out_path, int w, int h) {
	ifstream in_file(in_path, std::ifstream::binary);
	ofstream out_file(out_path, std::ofstream::binary);

	if (in_file.fail()) {
		cout << "open " << in_path << " failed." << endl;
		exit(0);
	}
	unsigned char* yuv444 = static_cast<unsigned char*>(malloc(w * h * 3));
	in_file.read(reinterpret_cast<char*>(yuv444), w * h * 3);
	unsigned char* y, * u, * v;
	y = yuv444;
	u = yuv444 + w * h;
	v = yuv444 + w * h * 2;

	unsigned char* rgb24 = static_cast<unsigned char*>(malloc(w * h * 3));
	unsigned char* rgb = rgb24;

	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			unsigned char r = clip((298 * (*y - 16) + 409 * (*v - 128) + 128) >> 8);
			unsigned char g = clip((298 * (*y - 16) - 100 * (*u - 128) - 208 * (*v - 128) + 128) >> 8);
			unsigned char b = clip((298 * (*y - 16) + 516 * (*u - 128) + 128) >> 8);

			//unsigned char r = clip(round(1.164383 * ((*y) - 16) + 1.596027 * ((*v) - 128)));
			//unsigned char g = clip(round(1.164383 * ((*y) - 16) - (0.391762 * (*u) - 128)) - (0.812968 * (*v) - 128));
			//unsigned char b = clip(round(1.164383 * ((*y) - 16) + 2.017232 * (*u) - 128));

			*(rgb++) = r;
			*(rgb++) = g;
			*(rgb++) = b;

			*(y++);
			*(u++);
			*(v++);
		}
	}

	out_file.write(reinterpret_cast<char*> (rgb24), w * h * 3);

	free(rgb24);
	free(yuv444);

	in_file.close();
	out_file.close();
}

/**
 * @param in_path		The path of RGB24 file
 * @param out_path		The path of YUV444P file
 * @param w				The width of RGB24 file
 * @param h				The height of RGB24 file
 */
void rgb24_to_yuv444(string in_path, string out_path, int w, int h) {
	ifstream in_file(in_path, std::ifstream::binary);
	ofstream out_file(out_path, std::ofstream::binary);

	if (in_file.fail()) {
		cout << "open " << in_path << " failed." << endl;
		exit(0);
	}
	unsigned char* yuv444 = static_cast<unsigned char*>(malloc(w * h * 3));
	unsigned char* y, * u, * v;
	y = yuv444;
	u = yuv444 + w * h;
	v = yuv444 + w * h * 2;

	unsigned char* rgb24 = static_cast<unsigned char*>(malloc(w * h * 3));
	in_file.read(reinterpret_cast<char*>(rgb24), w * h * 3);
	unsigned char* rgb = rgb24;

	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			unsigned char r = *(rgb++);
			unsigned char g = *(rgb++);
			unsigned char b = *(rgb++);

			*(y++) = clip(((66 * r + 129 * g + 25 * b + 128) >> 8) + 16);
			*(u++) = clip(((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128);
			*(v++) = clip(((112 * r - 94 * g - 18 * b + 128) >> 8) + 128);
		}
	}

	out_file.write(reinterpret_cast<char*> (yuv444), w * h * 3);

	free(rgb24);
	free(yuv444);

	in_file.close();
	out_file.close();
}

int main()
{
	string yuv_in_path = "../../../park_joy_444_1280x720.yuv";
	string rgb_out_path = "../../../park_joy_888_1280x720.rgb";
	yuv444_to_rgb24(yuv_in_path, rgb_out_path, 1280, 720);

	string rgb_in_path = "../../../park_joy_888_1280x720.rgb";
	string yuv_out_path = "../../../park_joy_out_444_1280x720.yuv";
	rgb24_to_yuv444(rgb_in_path, yuv_out_path, 1280, 720);
	return 0;
}
