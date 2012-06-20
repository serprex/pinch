#include "ch.h"
int main(int argc,char**argv){
	FILE*f=fopen("tgen.c","w");
	fputs("const unsigned char ",f);
	int dim[argc*3-3>>1],*di=dim,id=-1;
	for(int i=1;i<argc;i++){
		if(argv[i][0]=='+'){
			continue;
		}
		char nm[strlen(argv[i])+9];
		sprintf(nm,"spr/%s.png",argv[i]);
		free(rows);
		readpng(nm);
		di[0]=np2(wi4);
		di[1]=np2(hei);
		switch(csz){
		default:__builtin_unreachable();
		case(1)di[2]=GL_LUMINANCE;
		case(2)di[2]=GL_LUMINANCE_ALPHA;
		case(3)di[2]=GL_RGB;
		case(4)di[2]=GL_RGBA;
		}
		if(i>1)putc(',',f);
		fprintf(f,"%s[%d]=\"",argv[i],di[0]*di[1]*csz);
		for(int y=hei-1;y>=0;y--){
			int x=0;
			for(;x<wid;x++)
				fprintf(f,"\\%o",road[y*wid+x]);
			for(;x<np2(wid);i++)
				fputs("\\0",f);
		}
		for(int i=0;i<(np2(hei)-hei)*wid;i++)
			fputs("\\0",f);
		putc('"',f);
		di+=3;
	}
	putc(';',f);
	fclose(f);
	f=fopen("tgen.h","w");
	fputs("extern const unsigned char ",f);
	di=dim;
	for(int i=1;i<argc;i++){
		if(argv[i][0]=='+'){
			continue;
		}
		if(i>1)putc(',',f);
		fprintf(f,"%s[%d]",argv[i],di[0]*di[1]*csz);
		di+=3;
	}
	fputs(";static const int ",f);
	di=dim;
	for(int i=1;i<argc;i++){
		id++;
		if(argv[i][0]=='+'){
			if(strlen(argv[i])>1)
				id+=atoi(argv[i]+1)-1;
			continue;
		}
		if(i>1)putc(',',f);
		char*a=argv[i];
		fprintf(f,"%sW=%d,%sH=%d,%sF=%d,%sT=%d",a,di[0],a,di[1],a,di[2],a,id);
		di+=3;
	}
	fprintf(f,";\n#define TLEN %d",id+1);
}
