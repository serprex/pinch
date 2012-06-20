#include "ch.h"
typedef struct{
	unsigned short x,y,w,h;
	char*n;
	png_bytep s;
}spr;
int sprcmp(void*a,void*b)
{
	return ((spr*)b)->h-((spr*)a)->h;
}
int main(int argc,char**argv){
	FILE*f=fopen("sgen.c","w");
	fputs("const unsigned char ",f);
	int Fmt=-1;
	spr S[argc-1];
	for(int i=1;i<argc;i++){
		char nm[strlen(argv[i])+9];
		sprintf(nm,"spr/%s.png",argv[i]);
		readpng(nm);
		S[i-1].x=0;
		S[i-1].y=0;
		S[i-1].w=wi4;
		S[i-1].h=hei;
		S[i-1].s=road;
		S[i-1].n=argv[i];
		int fmt=csz;
		if(Fmt==-1)Fmt=csz;
		else if(Fmt!=csz)abort_("%s conflicting formats %d %d\n",argv[i],Fmt,csz);
	}
	qsort(S,argc-1,sizeof(spr),(__compar_fn_t)sprcmp);
	int xacc=0;
	for(int i=0;i<argc-1;i++){
		S[i].x=xacc;
		xacc+=S[i].w;
	}
	int Swid=np2(xacc),Shei=np2(S[0].h);
	fprintf(f,"S[%d]=\"",Swid*Shei*csz);
	unsigned char Sdata[Swid*Shei*csz];
	memset(Sdata,0,sizeof(Sdata));
	for(int i=0;i<argc-1;i++){
		spr*s=S+i;
		printf("%s:%d,%d,%d,%d\n",S[i].n,S[i].x,S[i].y,S[i].w,S[i].h);
		for(int y=s->h-1;y>=0;y--)
			for(int x=0;x<s->w;x++)
				memcpy(Sdata+(s->x+x+(s->y+y)*Swid)*csz,s->s+(x+y*s->w)*csz,csz);
	}
	for(int i=0;i<sizeof(Sdata);i++)
		fprintf(f,"\\%o",Sdata[i]);
	fputs("\";",f);
	fclose(f);
	switch(Fmt){
	default:__builtin_unreachable();
	case(1)Fmt=GL_LUMINANCE;
	case(2)Fmt=GL_LUMINANCE_ALPHA;
	case(3)Fmt=GL_RGB;
	case(4)Fmt=GL_RGBA;
	}
	f=fopen("sgen.h","w");
	fprintf(f,"extern const unsigned char S[%d];static const int Swid=%d,Shei=%d,Sfmt=%d",Swid*Shei*csz,Swid,Shei,csz);
	for(int i=0;i<argc-1;i++){
		spr*s=S+i;
		fprintf(f,",%sx=%d,%sy=%d,%sw=%d,%sh=%d",s->n,s->x,s->n,s->y,s->n,s->w,s->n,s->h);
	}
	fputc(';',f);
}