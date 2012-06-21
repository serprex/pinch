#include "ch.h"
typedef struct{
	unsigned short x,y,w,h;
	char*n;
	png_bytep s;
}spr;
typedef struct{
	unsigned short x,y,w,h;
	unsigned a;
}rect;
rect*fr;
int frs=1;
int sprcmp(void*a,void*b)
{
	return ((spr*)b)->h-((spr*)a)->h;
}
int main(int argc,char**argv){
	FILE*f=fopen("sgen.c","w");
	fputs("const unsigned char ",f);
	int Fmt=-1;
	spr S[argc-1];
	unsigned Swid=1,Shei=1;
	for(int i=1;i<argc;i++){
		char nm[strlen(argv[i])+9];
		sprintf(nm,"spr/%s.png",argv[i]);
		readpng(nm);
		if(Fmt!=csz){
			if(Fmt==-1)Fmt=csz;
			else abort_("%s conflicting formats %d %d\n",argv[i],Fmt,csz);
		}
		if(wi4>Swid)Swid=wi4;
		if(hei>Shei)Shei=hei;
		spr*s=S+i-1;
		s->x=0;
		s->y=0;
		s->w=wi4;
		s->h=hei;
		s->s=road;
		s->n=argv[i];
	}
	qsort(S,argc-1,sizeof(spr),(__compar_fn_t)sprcmp);
	fr=malloc(sizeof(rect));
	fr->x=0;
	fr->y=0;
	fr->w=Swid=np2(Swid);
	fr->h=Shei=np2(Shei);
	fr->a=fr->w*fr->h;
	for(int i=0;i<argc-1;i++){
		spr*s=S+i;
		newjj:;
		int f=-1,mna=Swid*Shei;
		for(int j=0;j<frs;j++){
			if(fr[j].a<mna&&s->w<fr[j].w&&s->h<fr[j].h&&s->w*s->h<=fr[j].a){
				mna=fr[j].a;
				f=j;
			}
		}
		if(f==-1){
			if(Swid<Shei){
				for(int j=0;j<frs;j++)
					if(fr[j].x+fr[j].w==Swid){
						fr[j].w+=Swid;
						fr[j].a=fr[j].w*fr[j].h;
					}
				Swid<<=1;
			}
			else{
				for(int j=0;j<frs;j++)
					if(fr[j].y+fr[f].h==Shei){
						fr[j].h+=Shei;
						fr[j].a=fr[j].w*fr[j].h;
					}
				Shei<<=1;
			}
			goto newjj;
		}
		s->x=fr[f].x;
		s->y=fr[f].y;
		if(s->w==fr[f].w||s->h==fr[f].h){
			if(s->w==fr[f].w&&s->h==fr[f].h){
				memmove(fr+f,fr+f+1,(frs-f-1)*sizeof(rect));
				frs--;
			}else if(s->w==fr[f].w){
				fr[f].y+=s->h;
				fr[f].h-=s->h;
				fr[f].a=fr[f].w*fr[f].h;
			}else{
				fr[f].x+=s->w;
				fr[f].w-=s->w;
				fr[f].a=fr[f].w*fr[f].h;
			}
		}else{
			fr=realloc(fr,(++frs)*sizeof(rect));
			fr[frs-1].x=fr[f].x;
			fr[frs-1].y=fr[f].y+s->h;
			fr[frs-1].w=s->w;
			fr[frs-1].h=fr[f].h-s->h;
			fr[frs-1].a=fr[frs-1].w*fr[frs-1].h;
			fr[f].x+=s->w;
			fr[f].w-=s->w;
			fr[f].a=fr[f].w*fr[f].h;
		}
	}
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