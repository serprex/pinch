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
	if(!fp)abort_("%s??",fame);
	png_structp png_ptr=png_create_read_struct(PNG_LIBPNG_VER_STRING,0,0,0);
	if(!png_ptr)abort_("%s png_create_read_struct",fame);
	png_infop info_ptr=png_create_info_struct(png_ptr);
	if(!info_ptr)abort_("%s png_create_info_struct",fame);
	if(setjmp(png_jmpbuf(png_ptr)))abort_("%s init_io",fame);
	png_init_io(png_ptr,fp);
	png_read_info(png_ptr,info_ptr);
	wi4=png_get_image_width(png_ptr,info_ptr);
	wid=wi4<<2;
	hei=png_get_image_height(png_ptr,info_ptr);
	if(png_get_color_type(png_ptr,info_ptr)!=PNG_COLOR_TYPE_RGBA)abort_("%s %d not rgba",fame,png_get_color_type(png_ptr,info_ptr));
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
int bpp(int m){
	switch(m){
	case(GL_RGBA)return 4;
	case(GL_RGB)return 3;
	case(GL_LUMINANCE_ALPHA)return 2;
	case(GL_LUMINANCE)return 1;
	}
}
int main(int argc,char**argv){
	FILE*f=fopen("tgen.c","w");
	fputs("const unsigned char ",f);
	int dim[argc*3-3>>1],*di=dim,id=-1;
	for(int i=1;i<argc;i+=2){
		if(argv[i][0]=='+'){
			i--;
			continue;
		}
		char nm[strlen(argv[i])+9];
		switch(*argv[i+1]){
		case('R')di[2]=GL_RGBA;
		case('r')di[2]=GL_RGB;
		case('G')di[2]=GL_LUMINANCE_ALPHA;
		case('g')di[2]=GL_LUMINANCE;
		}
		sprintf(nm,"spr/%s.png",argv[i]);
		free(rows);
		readpng(nm);
		di[0]=np2(wi4);
		di[1]=np2(hei);
		if(i>1)putc(',',f);
		fprintf(f,"%s[%d]=\"",argv[i],di[0]*di[1]*bpp(di[2]));
		for(int y=hei-1;y>=0;y--){
			for(int x=0;x<wid;x+=4){
				switch(*argv[i+1]){
				case('R')fprintf(f,"\\%o\\%o\\%o\\%o",road[y*wid+x],road[y*wid+x+1],road[y*wid+x+2],road[y*wid+x+3]);
				case('r')fprintf(f,"\\%o\\%o\\%o",road[y*wid+x],road[y*wid+x+1],road[y*wid+x+2]);
				case('G')fprintf(f,"\\%o\\%o",road[y*wid+x],road[y*wid+x+3]);
				case('g')fprintf(f,"\\%o",road[y*wid+x]);
				}
			}
			for(int i=0;i<(np2(wi4)-wi4)*bpp(di[2]);i++)
				fputs("\\0",f);
		}
		for(int i=0;i<(np2(hei)-hei)*wi4*bpp(di[2]);i++)
			fputs("\\0",f);
		putc('"',f);
		di+=3;
	}
	putc(';',f);
	fclose(f);
	f=fopen("tgen.h","w");
	fputs("extern const unsigned char ",f);
	di=dim;
	for(int i=1;i<argc;i+=2){
		if(argv[i][0]=='+'){
			i--;
			continue;
		}
		if(i>1)putc(',',f);
		fprintf(f,"%s[%d]",argv[i],di[0]*di[1]*bpp(di[2]));
		di+=3;
	}
	fputs(";static const int ",f);
	di=dim;
	for(int i=1;i<argc;i+=2){
		id++;
		if(argv[i][0]=='+'){
			i--;
			if(strlen(argv[i])>1)
				id+=atoi(argv[i]+1)-1;
			continue;
		}
		if(i>1)putc(',',f);
		char*a=argv[i];
		fprintf(f,"%sW=%d,%sH=%d,%sF=%d,%sT=%d,%sB=%d",a,di[0],a,di[1],a,di[2],a,id,a,bpp(di[2]));
		di+=3;
	}
	fprintf(f,";\n#define TLEN %d",id+1);
}