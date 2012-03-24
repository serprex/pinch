#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <png.h>
#define abort_(...) {fprintf(stderr,__VA_ARGS__);abort();}
#define case(x) break;case x:
int wid,hei,wi4;
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
	wid=wi4<<2;
	hei=png_get_image_height(png_ptr,info_ptr);
	int e;
	if((e=png_get_color_type(png_ptr,info_ptr))!=PNG_COLOR_TYPE_RGBA)
	{
		if(e==PNG_COLOR_TYPE_RGB)
			png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);
		else
			abort_("%s %d not rgba\n",fame,e);
	}
	png_read_update_info(png_ptr,info_ptr);
	if(setjmp(png_jmpbuf(png_ptr)))abort_("%s read_image",fame);
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
int main(int argc,char**argv){
	FILE*f=fopen("bgen.c","w");
	fputs("const unsigned char ",f);
	for(int i=1;i<argc;i++){
		char nm[strlen(argv[i])+9];
		sprintf(nm,"spr/%s.png",argv[i]);
		free(rows);
		readpng(nm);
		if(i>1)putc(',',f);
		fprintf(f,"%sB[%d]=\"",argv[i],wid*hei>>5);
		for(int y=0;y<hei;y++){
			for(int x=0;x<wid;x+=32){
				fprintf(f,"\\%o",
				!road[y*wid+x]|
				!road[y*wid+x+4]<<1|
				!road[y*wid+x+8]<<2|
				!road[y*wid+x+12]<<3|
				!road[y*wid+x+16]<<4|
				!road[y*wid+x+20]<<5|
				!road[y*wid+x+24]<<6|
				!road[y*wid+x+28]<<7);
			}
		}
		putc('"',f);
	}
	putc(';',f);
	fclose(f);
	f=fopen("bgen.h","w");
	fputs("extern const unsigned char ",f);
	for(int i=1;i<argc;i++){
		if(i>1)putc(',',f);
		fprintf(f,"%sB[%d]",argv[i],wid*hei>>5);
	}
	putc(';',f);
}
