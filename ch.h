#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <png.h>
#define abort_(...) {fprintf(stderr,__VA_ARGS__);abort();}
#define case(x) break;case x:
int wid,hei,wi4,csz;
png_bytep*rows,road;
void readpng(char*fame){
	FILE*fp=fopen(fame,"rb");
	if(!fp)abort_("%s??\n",fame);
	png_structp png_ptr=png_create_read_struct(PNG_LIBPNG_VER_STRING,0,0,0);
	if(!png_ptr)abort_("%s png_create_read_struct\n",fame);
	png_infop info_ptr=png_create_info_struct(png_ptr);
	if(!info_ptr)abort_("%s png_create_info_struct\n",fame);
	if(setjmp(png_jmpbuf(png_ptr)))abort_("%s init_io\n",fame);
	png_init_io(png_ptr,fp);
	png_read_info(png_ptr,info_ptr);
	wi4=png_get_image_width(png_ptr,info_ptr);
	hei=png_get_image_height(png_ptr,info_ptr);
	int e=png_get_color_type(png_ptr,info_ptr);
	csz=(e&PNG_COLOR_MASK_COLOR?3:1)+(e&PNG_COLOR_MASK_ALPHA?1:0);
	wid=wi4*csz;
	printf("%dbpp\n",csz);
	if(setjmp(png_jmpbuf(png_ptr)))abort_("%s read_image\n",fame);
	rows=malloc((wid+sizeof(png_bytep))*hei);
	road=((void*)rows)+sizeof(png_bytep)*hei;
	for(int y=0;y<hei;y++)rows[y]=road+(hei-y-1)*wid;
	png_read_image(png_ptr,rows);
	png_destroy_read_struct(&png_ptr,&info_ptr,0);
	fclose(fp);
}
int np2(int x)
{
	x--;
	x|=x>>1;
	x|=x>>2;
	x|=x>>4;
	x|=x>>8;
	return (x|x>>16)+1;
}