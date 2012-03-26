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
