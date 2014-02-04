#include <iostream>
#include "bmp.h"

using namespace std;

void dump_alpha_channel_to_file();
void dump_stroke_pixels_to_file();

int main()
{	
	// dump alpha channel to file
	dump_alpha_channel_to_file();

	// dump stroke pixels to file.
	dump_stroke_pixels_to_file();

	return 0;
}

void dump_alpha_channel_to_file()
{
	const char* file_name = "../resource/alpha.bmp";
	unsigned char* image_buffer = NULL;
	int width = 0;
	int height = 0;

	if ( 1==LoadFileBMP(file_name, &image_buffer, &width, &height, false) )
	{	
		FILE* fp = fopen("../resource/alpha.dat", "w+");

		if (fp)
		{	
			for (int y=0; y<height; y++)
			{
				for (int x=0; x<width; x++)
				{
					fprintf(fp, "%d, ", image_buffer[(y*width+x)*4 + 0 ]);
				} 
				fprintf(fp, "\n");
			}

			fflush(fp);
			fclose(fp);
		}

		delete[] image_buffer;
	}
}

void dump_stroke_pixels_to_file()
{
	const char* file_name = "../resource/stroke.bmp";
	unsigned char* image_buffer = NULL;
	int width = 0;
	int height = 0;

	if ( 1==LoadFileBMP(file_name, &image_buffer, &width, &height, false) )
	{	
		FILE* fp = fopen("../resource/stroke.dat", "w+");

		if (fp)
		{	
			for (int y=0; y<height; y++)
			{
				for (int x=0; x<width; x++)
				{
					int value = image_buffer[ (y*width+x)*4 + 0];
					if (value < 255)
					{
						fprintf(fp, "%d,%d,%d ", x,y,value);
					}
					// fprintf(fp, "%d, ", image_buffer[(y*width+x)*4 + 0 ]);
				} 
				fprintf(fp, "\n");
			}

			fflush(fp);
			fclose(fp);
		}

		delete[] image_buffer;
	}
}