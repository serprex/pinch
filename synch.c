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
int inrng(int x, int mn, int mx)
{
	return x>=mn&&x<mx;
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
int sprcmp(void*a,void*b)
{
	return ((spr*)b)->w*((spr*)b)->h-((spr*)a)->w*((spr*)a)->h;
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
	mkrect(fr,0,0,Swid=np2(Swid),Shei=np2(Shei));
	for(int i=0;i<argc-1;i++){
		spr*s=S+i;
		newjj:;
		int f,mna=INT_MAX,mno=INT_MAX;
		for(int j=0;j<frs;j++){
			int o=0;
			for(int k=0;k<frs;k++){
				o+=rlap(fr+j,fr+k);
			}
			if(o<mno&&fr[j].a<mna&&s->w<fr[j].w&&s->h<fr[j].h){
				mno=o;
				mna=fr[j].a;
				f=j;
			}
		}
		if(mna==INT_MAX){
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
		for(int i=0;i<frs;i++){
			if(rlap((rect*)s,fr+i)){
				rect r=fr[i];
				fr[i--]=fr[--frs];
				fr=realloc(fr,(frs+4)*sizeof(rect));
				if(s->x>r.x){
					mkrect(fr+frs++,r.x,r.y,s->x-r.x,r.h);
				}
				if(s->x+s->w<r.x+r.w){
					mkrect(fr+frs++,s->x+s->w,r.y,r.x+r.w-s->x-s->w,r.h);
				}
				if(s->y>r.y){
					mkrect(fr+frs++,r.x,r.y,r.w,s->y-r.y);
				}
				if(s->y+s->h<r.y+r.h){
					mkrect(fr+frs++,r.x,s->y+s->h,r.w,r.y+r.h-s->y-s->h);
				}
			}
		}
	}
	for(;;){
		Swid>>=1;
		for(int i=0;i<frs;i++)
			if(fr[i].x+fr[i].w>Swid){
				Swid<<=1;
				goto fw;
			}
	}
	fw:
	for(;;){
		Shei>>=1;
		for(int i=0;i<argc-1;i++)
			if(S[i].y+S[i].h>Shei){
				Shei<<=1;
				goto fh;
			}
	}
	fh:
	fprintf(f,"S[%d]=\"",Swid*Shei*csz);
	printf("%dx%d frs=%d\n",Swid,Shei,frs);
	free(fr);
	unsigned char*Sdata=calloc(Swid*Shei*csz,1);
	for(int i=0;i<argc-1;i++){
		spr*s=S+i;
		for(int y=s->h-1;y>=0;y--)
			for(int x=0;x<s->w;x++)
				memcpy(Sdata+(s->x+x+(s->y+y)*Swid)*csz,s->s+(x+y*s->w)*csz,csz);
	}
	for(int i=0;i<Swid*Shei*csz;i++)
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
	fprintf(f,"extern const unsigned char S[%d];static const int Swid=%d,Shei=%d,Sfmt=%d",Swid*Shei*csz,Swid,Shei,Fmt);
	for(int i=0;i<argc-1;i++){
		spr*s=S+i;
		fprintf(f,",%sx=%d,%sy=%d,%sw=%d,%sh=%d",s->n,s->x,s->n,s->y,s->n,s->w,s->n,s->h);
	}
	fputc(';',f);
}