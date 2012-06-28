#include <stdint.h>
#include "ch.h"
typedef struct{
	uint16_t x,y,w,h;
	char*n;
	png_bytep s;
}spr;
typedef struct{
	uint16_t x,y,w,h;
	uint32_t a;
}rect;
rect*fr;
int frs=1;
int inrng(int x,int mn,int mx){return x>=mn&&x<mx;}
int rin(rect*a,rect*b)
{
	return a->x>=b->x&&a->y>=b->y&&a->x+a->w<=b->x+b->w&&a->y+a->h<=b->y+b->h;
}
int rlap(rect*a,rect*b)
{
    return (inrng(a->x,b->x,b->x+b->w)||inrng(b->x,a->x,a->x+a->w))&&
		(inrng(a->y,b->y,b->y+b->h)||inrng(b->y,a->y,a->y+a->h));
}
void mkrect(rect*r,uint16_t x,uint16_t y,uint16_t w,uint16_t h){
	r->x=x;
	r->y=y;
	r->w=w;
	r->h=h;
	r->a=w*h;
}
void mkfr(uint16_t x,uint16_t y,uint16_t w,uint16_t h){
	mkrect(fr+frs,x,y,w,h);
	for(int i=0;i<frs;i++){
		if(rin(fr+frs,fr+i))
			return;
	}
	frs++;
}
int sprcmp(void*a,void*b)
{
	return ((spr*)b)->w*((spr*)b)->h-((spr*)a)->w*((spr*)a)->h;
}
int main(int argc,char**argv){
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
	qsort(S,--argc,sizeof(spr),(__compar_fn_t)sprcmp);
	fr=malloc(sizeof(rect));
	mkrect(fr,0,0,Swid=np2(Swid),Shei=np2(Shei));
	for(int i=0;i<argc;i++){
		spr*s=S+i;
		int js=0,sw=Swid,sh=Shei;
		newjj:;
		int f,mna=INT_MAX;
		for(int j=0;j<frs;j++){
			if(fr[j].a<mna&&s->w<=fr[j].w&&s->h<=fr[j].h){
				mna=fr[j].a;
				f=j;
			}
		}
		if(mna==INT_MAX){
			Swid=sw;
			Shei=sh;
			int w=1,h=1;
			if(s->w>Swid)
				w+=s->w/Swid;
			if(s->h>Shei)
				h+=s->h/Shei;
			if((!w&&!h)||js){
				int js3=js&3;
				if(js3==2){
					w+=js>>2;
					h+=js>>2;
				}else{
					if(js3==3){
						js++;
						js3=0;
					}
					if((Shei>Swid)^(js3&1))
						w+=js>>2;
					else
						h+=js>>2;
				}
			}
			for(int i=0;i<2;i++)
				for(int j=0;j<frs;j++){
					int16_t*xw=(int16_t*)(fr+j);
					if(xw[i]+xw[i+2]>=(i?Shei:Swid)){
						xw[i+2]=(i?Shei*h:Swid*w)-xw[i];
						fr[j].a=fr[j].w*fr[j].h;
					}
				}
			Swid*=w;
			Shei*=h;
			js++;
			goto newjj;
		}
		s->x=fr[f].x;
		s->y=fr[f].y;
		for(int i=0;i<frs;i++){
			if(rlap((rect*)s,fr+i)){
				rect r=fr[i];
				fr[i--]=fr[--frs];
				fr=realloc(fr,(frs+4)*sizeof(rect));
				if(s->x>r.x)
					mkfr(r.x,r.y,s->x-r.x,r.h);
				if(s->x+s->w<r.x+r.w)
					mkfr(s->x+s->w,r.y,r.x+r.w-s->x-s->w,r.h);
				if(s->y>r.y)
					mkfr(r.x,r.y,r.w,s->y-r.y);
				if(s->y+s->h<r.y+r.h)
					mkfr(r.x,s->y+s->h,r.w,r.y+r.h-s->y-s->h);
			}
		}
	}
	printf("%dx%d frs=%d\n",Swid,Shei,frs);
	free(fr);
	unsigned char*Sdata=calloc(Swid*Shei*csz,1);
	for(int i=0;i<argc;i++){
		spr*s=S+i;
		for(int y=0;y<s->h;y++)
			for(int x=0;x<s->w;x++)
				memcpy(Sdata+(s->x+x+(s->y+y)*Swid)*csz,s->s+(x+y*s->w)*csz,csz);
	}
	FILE*f=fopen("sgen.c","w");
	fprintf(f,"#include <stdint.h>\nconst unsigned char S[%d]=\"",Swid*Shei*csz);
	for(int i=0;i<Swid*Shei*csz;i++)
		fprintf(f,"\\%o",Sdata[i]);
	fprintf(f,"\";const struct spr{uint16_t x,y,w,h;}spr[%d]={",argc);
	for(int i=0;i<argc;i++){
		fprintf(f,"{%d,%d,%d,%d},",S[i].x,S[i].y,S[i].w,S[i].h);
	}
	fputs("};",f);
	fclose(f);
	switch(Fmt){
	default:__builtin_unreachable();
	case(1)Fmt=GL_LUMINANCE;
	case(2)Fmt=GL_LUMINANCE_ALPHA;
	case(3)Fmt=GL_RGB;
	case(4)Fmt=GL_RGBA;
	}
	f=fopen("sgen.h","w");
	fprintf(f,"#include <stdint.h>\nextern const unsigned char S[%d];extern const struct spr{uint16_t x,y,w,h;} spr[%d];static const int Swid=%d,Shei=%d,Sfmt=%d",Swid*Shei*csz,argc,Swid,Shei,Fmt);
	for(int i=0;i<argc;i++)
		fprintf(f,",%s=%d",S[i].n,i);
	fputc(';',f);
}