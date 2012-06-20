#include "ch.h"
int main(int argc,char**argv){
	FILE*f=fopen("bgen.c","w");
	fputs("const unsigned char ",f);
	for(int i=1;i<argc;i++){
		char nm[strlen(argv[i])+9];
		sprintf(nm,"spr/%s.png",argv[i]);
		free(rows);
		readpng(nm);
		if(i>1)putc(',',f);
		fprintf(f,"%sB[%d]=\"",argv[i],wi4*hei>>3);
		for(int y=0;y<hei;y++){
			for(int x=0;x<wid;x+=csz*8){
				fprintf(f,"\\%o",
				!road[y*wid+x]|
				!road[y*wid+x+csz]<<1|
				!road[y*wid+x+csz*2]<<2|
				!road[y*wid+x+csz*3]<<3|
				!road[y*wid+x+csz*4]<<4|
				!road[y*wid+x+csz*5]<<5|
				!road[y*wid+x+csz*6]<<6|
				!road[y*wid+x+csz*7]<<7);
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
		fprintf(f,"%sB[%d]",argv[i],wi4*hei>>3);
	}
	putc(';',f);
}
