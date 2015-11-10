/*Published under The MIT License (MIT)

See LICENSE.TXT*/

// Ryan Pridgeon COM2032 rp00091

#include "tga.h"
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl\GL.h>
#include <gl\GLU.h>

// define the header struct for
// loading the TGA header info.
#pragma pack(1) // pack it so that I can have shorts & chars in sequence and read directly from file memory
struct TGAHeader{
    char    id_length;
    char    map_type;
    char    type;
    short   map_start;
    short   map_length;
    char    map_depth;
    short   x_origin;
    short   y_origin;
    short   width;
    short   height;
    char    bpp;
    char    descriptor_bits;
};
#pragma pack()

TGA::TGA(char* imagePath)
{
    FILE* file = NULL; // the file handle
    TGAHeader header; // struct for the header info
    char* pixels, *buffer;

	// column counter, row counter, i & j loop counters, and bytes per pixel
    int c, r, i, j, bytespp = 4;
    char n, packet_header;
    char pixel[4];

    // open the file
    file = fopen(imagePath, "rb");

    // read the header
    fread(&header, 18, 1, file);

    bytespp = header.bpp / 8; // bytes per pixel

	pixels = (char*)malloc(bytespp * header.width * header.height);

    // header type 2 is uncompressed RGB data without a color map / pallette
    if (header.type == 2)
	{
        // seek to the start of the data
        fseek(file, header.map_start + header.map_length * bytespp + 18, SEEK_SET);

        // read the pixel data into a buffer
        buffer = (char*)malloc(bytespp * header.width * header.height);
        fread(buffer, bytespp, header.width * header.height, file);
        
        // plot the pixel data into pixels buffer
        for (c = 0; c < header.width; c ++) // count up columns
		{
            for (r = 0; r < header.height; r ++) // rows
			{
                if (bytespp == 4)
				{
                    pixels[(c + (header.height - r - 1) * header.width) * bytespp + 0] =
                        buffer[(c + r * header.width) * bytespp + 2];
                    pixels[(c + (header.height - r - 1) * header.width) * bytespp + 1] =
                        buffer[(c + r * header.width) * bytespp + 1];
                    pixels[(c + (header.height - r - 1) * header.width) * bytespp + 2] =
                        buffer[(c + r * header.width) * bytespp + 0];
                    pixels[(c + (header.height - r - 1) * header.width) * bytespp + 3] =
                        buffer[(c + r * header.width) * bytespp + 3];
                }
				else if (bytespp == 3)
				{
                    pixels[(c + (header.height - r - 1) * header.width) * bytespp + 0] =
                        buffer[(c + r * header.width) * bytespp + 2];
                    pixels[(c + (header.height - r - 1) * header.width) * bytespp + 1] =
                        buffer[(c + r * header.width) * bytespp + 1];
                    pixels[(c + (header.height - r - 1) * header.width) * bytespp + 2] =
                        buffer[(c + r * header.width) * bytespp + 0];
                }
            }
        }

        free(buffer);

    } 
	else if (header.type == 10) // run length encoded, non color mapped rgb
	{
        // find the start of the data
        fseek(file, header.map_start + header.map_length * bytespp + 18, SEEK_SET);

        c = 0; r = header.height - 1; // start at the top left
        // work through the bitmap
        while (r >= 0)
		{
            // read in the packet header
            fread(&packet_header, sizeof(packet_header), 1, file);

            // find the number of reps
            n = packet_header & 0x7F;

            *((int*)pixel) = 0;
            if (n != packet_header) // if bit = 1, the next pixel repeated N times
                fread(&pixel, bytespp, 1, file);

            // loop n times
            for (i = 0; i < n + 1; i++)
			{
                if (n == packet_header) // if bit = 0, N individual pixels
                    fread(&pixel, bytespp, 1, file);

                if (bytespp == 4)
				{
                    pixels[(c + r * header.width) * bytespp + 0] =
                        pixel[2];
                    pixels[(c + r * header.width) * bytespp + 1] =
                        pixel[1];
                    pixels[(c + r * header.width) * bytespp + 2] =
                        pixel[0];
                    pixels[(c + r * header.width) * bytespp + 3] =
                        pixel[3];
                }
				else if (bytespp == 3)
				{
                    pixels[(c + r * header.width) * bytespp + 0] =
                        pixel[2];
                    pixels[(c + r * header.width) * bytespp + 1] =
                        pixel[1];
                    pixels[(c + r * header.width) * bytespp + 2] =
                        pixel[0];
                }
                // move to the next pixel
                c += 1;
                if (c >= header.width)
				{
                    c = 0;
                    r -= 1;
                }
            }
        }
    }
    // close the file
    fclose(file);

	// make a gl texture
	glGenTextures(1, &textureHandle);

	glBindTexture(GL_TEXTURE_2D, textureHandle);

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					 GL_LINEAR_MIPMAP_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, header.width, header.height,
                   GL_RGB, GL_UNSIGNED_BYTE, pixels);

	// delete the pixel data
	free(pixels);
}

GLuint TGA::getTextureHandle(void)
{
	return textureHandle;
}
