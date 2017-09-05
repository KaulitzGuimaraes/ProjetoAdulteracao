//Nome: Thiago Henrique Viotto   RA: 187583
//Nome: Vitória Maria Guimarães  RA: 188530

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <math.h>
#define MAX1 100
#define MAX2 200
#define BRANCO 0
#define PRETO 1
#define MAX 400

//registro medida
typedef struct Medida{
    double Atenua;
    double Veloc;
} Medida;

//registro amostra
typedef struct Amostra{
    Medida *med[MAX1];
    int top;  //numero de elementos de medida
    int ID;  //identificador
} Amostra;

//registro ConjAmostra
typedef struct ConjAmostras{
    Amostra *amo[MAX2];
    long top;  //numero de elementos de amostra
} ConjAmostras;

//Registro Imagem
typedef struct Imagem{
    int LI;//linha
    int CO;//colunas
    int m[MAX][MAX];
}Imagem;

//Construtor limite
typedef struct Limite{
        float a[5];  //valor minimo de cada percentual de amostra
        float v[5];  //valor maximo de cada percentual de amostra
}Limite;

//construtor de medida
Medida *newMedida(float Atenua, float Veloc){
    Medida*m = (Medida*)malloc(sizeof(Medida));  //aloca dinamicamente
    m->Atenua = Atenua;
    m->Veloc = Veloc;

    return m;
}

//construtor de amostras
Amostra *newAmostr(char*linha1, char*linha2){
    Amostra*a = (Amostra*)malloc(sizeof(Amostra));
    float atenua[5],veloc[5];
    sscanf(linha1,"%d;%f;%f;%f;%f;%f\n",&a->ID,&atenua[0],&atenua[1],&atenua[2],&atenua[3],&atenua[4]);
    sscanf(linha2,"%d;%f;%f;%f;%f;%f\n",&a->ID,&veloc[0],&veloc[1],&veloc[2],&veloc[3],&veloc[4]);
    a->top=0;
    while(a->top<5){
        a->med[a->top]=newMedida(atenua[a->top],veloc[a->top]);
        (a->top)++;
    }
    return a;
}

//construtor de conjAmostras
ConjAmostras *ConjAmostr(char*fileAtenua,char*fileVeloc){
    char linha[1000],linha2[1000];
    ConjAmostras*c = (ConjAmostras*)malloc(sizeof(ConjAmostras));
    FILE *f,*f2;
    f = fopen(fileAtenua,"r");
    f2 = fopen(fileVeloc,"r");
    c->top = -1;
    while((fscanf(f,"%s",linha)!= EOF) && (fscanf(f2,"%s",linha2)!= EOF)){  //le as linhas e cria amostras
        if(c->top>=0){
            c->amo[c->top]=newAmostr(linha,linha2);
        }
        c->top++;
    }
    return c;
}

//Imprime Medida
void printMedida (Medida *medida){
	printf("%.2f ; %.2f ",medida->Atenua, medida->Veloc);
}

//Imprime Amostra
void printAmostra (Amostra *a){
	int i=0;
	printf("%d ",a->ID);
	while(i<5){
		printMedida(a->med[i]);
        i++;
	}
	printf("\n");
}

//Imprime ConjAmodtras
void printConjAmostras (ConjAmostras *set){
	int i=0;
	printf("ID;             3%%;            6%%;            10%%;            15%%;            20%%\n");
	while(i<set->top){
		printAmostra(set->amo[i]);
		i++;
	}
}

//Imprime a Media
void printamedia(float media){
	printf("%.2f\n",media);
}

//Imprime Desvio padrao
void printdesvio(float media){
	printf("%.2f\n",media);
}

float obtemMedidaA (Amostra *a,int id){
    return a->med[id]->Atenua;
}

float obtemMedidaV (Amostra *a,int id){
	return a->med[id]->Veloc;
}

//Calculo da media
void CalculaMedia(ConjAmostras*c){
	int i=0,j=0;
	float mediaA[5]={0,0,0,0,0},mediaV[5]={0,0,0,0,0};
	int porcentagem[5]={3,6,10,15,20};
	while(i<c->top){
		while(j<5){
			mediaA[j]=mediaA[j]+obtemMedidaA(c->amo[i],j);
			mediaV[j]=mediaV[j]+obtemMedidaV(c->amo[i],j);
			j++;
		}
		j=0;
		i++;
	}
	i=0;
	printf("\nMedias:\n\n");
	while(i<5){
		printf("%d %% : \n",porcentagem[i]);
		mediaA[i]=(mediaA[i])/(c->top);
		mediaV[i]=(mediaV[i])/(c->top);
		printf("Atenua : \n");
		printamedia(mediaA[i]);
		printf("Veloc : \n");
		printamedia(mediaV[i]);
		printf("==============\n");
		i++;
	}
	CalculaDesvio(mediaA,mediaV,c);
}

//desvio padrao
void CalculaDesvio(float mediaA[], float mediaV[],ConjAmostras *c){
	int i=0,j=0;
	float desvioA[5]={0,0,0,0,0},desvioV[5]={0,0,0,0,0};
	int porcentagem[5]={3,6,10,15,20};
	while(i<c->top){
		while(j<5){
			desvioA[j]=desvioA[j]+pow(obtemMedidaA(c->amo[i],j)-mediaA[j],2);
			desvioV[j]=desvioV[j]+pow(obtemMedidaV(c->amo[i],j)-mediaV[j],2);
			j++;
		}
		j=0;
		i++;
	}
	i=0;
	printf("Desvio Padrao:\n\n");
	while(i<5){
        desvioA[i] = sqrt(desvioA[i]/c->top);
        desvioV[i] = sqrt(desvioV[i]/c->top);
		printf("%d %% : \n",porcentagem[i]);
		printf("Atenua : \n");
		printdesvio(desvioA[i]);
		printf("Veloc : \n");
		printdesvio(desvioV[i]);
		printf("==============\n");
		i++;
	}
}

//cria imagem
Imagem * novaImagem(int LI,int CO){
    Imagem *x = (Imagem *)malloc(sizeof(Imagem));
    x->LI=LI;
    x->CO=CO;
    int li, col;
    for(li=0;li<LI;li++){
        for(col=0;col<CO;col++){
            x->m[li][col]=BRANCO;
        }
    }
    return x;
}

//atribui a cor ao pixel
void setPixel(Imagem *img,int li,int col,int cor){

  if(li>=0&&li<img->LI&&col>0&&col<img->CO){

        img->m[li][col]=cor;
        img->m[li+1][col]=cor;
        img->m[li][col+1]=cor;
        img->m[li+1][col+1]=cor;
        if(li>0 && col>0){
        img->m[li-1][col]=cor;
        img->m[li][col-1]=cor;
        img->m[li-1][col-1]=cor;}
    }
}

//Salva imagem em um arquivo pbm.
void salvaImagem(Imagem *x, char * fname){
    FILE *f;
    int li,col;
    f = fopen(fname,"w");
    fprintf(f,"P1\n");
    fprintf(f,"%d %d\n",x->CO,x->LI);
    for (li=x->LI-1; li>=0;li--) {
        for (col=0; col<x->CO;col++) {
            fprintf(f,"%d ",x->m[li][col]);
        }
        fprintf(f,"\n");
    }
    printf("Gravou %s\n",fname);
    fclose(f);
}

//acha o minimo de cada percentual de amostra
Limite *novoLimiteVMin(ConjAmostras *x){
    Limite*l = (Limite*)malloc(sizeof(Limite));
    int i=0,j=0;
    float lim1,aux=10000;

    return l;
}

//acha o maximo de cada percentual de amostra
Limite *novoLimiteMin(ConjAmostras *x){
    Limite*l = (Limite*)malloc(sizeof(Limite));
    int i=0,j=0;
    float lim1,aux=10000;
    while(j<5){
        while(i<x->top){
            lim1=obtemMedidaA(x->amo[i],j);
            if(lim1<aux)
                aux= lim1;
            i++;
        }
    l->a[j]=aux;
    i=0;
    aux=10000;
    j++;
    }
    i=0;
    j=0;
    aux=10000;
      while(j<5){
        while(i<x->top){
            lim1=obtemMedidaV(x->amo[i],j);
            if(lim1<aux)
                aux= lim1;
            i++;
        }
    l->v[j]=aux;
    i=0;
    aux=10000;
    j++;
    }

    return l;
}

//acha o maximo de cada percentual de amostra
Limite *novoLimiteMax(ConjAmostras *x){
Limite*l = (Limite*)malloc(sizeof(Limite));
    int i=0,j=0;
    float lim1,aux=-1000;
    while(j<5){
        while(i<x->top){
            lim1=obtemMedidaA(x->amo[i],j);
            if(lim1>aux)
                aux= lim1;
            i++;
        }
    l->a[j]=aux;
    i=0;
    aux=-1000;
    j++;
    }
    j=0;
    i=0;
    aux=-1000;
    while(j<5){
            while(i<x->top){
                lim1=obtemMedidaV(x->amo[i],j);
                if(lim1>aux)
                    aux= lim1;
                i++;
            }
        l->v[j]=aux;
        i=0;
        aux=-1000;
        j++;
    }
    return l;
}

//Estrutura da imagem
void novaEstrutura(ConjAmostras*c, int p){
    Limite*lmax = (Limite*)malloc(sizeof(Limite));
    Limite*lmin = (Limite*)malloc(sizeof(Limite));
    lmax = novoLimiteMax(c);
    lmin= novoLimiteMin(c);
    Imagem *img = (Imagem*)malloc(sizeof(Imagem));
    img = novaImagem(MAX,MAX);
    int i;
    float x; //valor do obtem medida da atenua
    float y; //valor do obtem medida do Veloc
    float mediaX, mediaY;
    for(i=0; i<c->top; i++){
        x = obtemMedidaA(c->amo[i],p);
        y = obtemMedidaV(c->amo[i],p);
        mediaX = ((x-lmin->a[p])/ (lmax->a[p]-lmin->a[p]));
        mediaY = ((y-lmin->v[p])/ (lmax->v[p]-lmin->v[p]));
        mediaX*=390;
        mediaY*=390;
        setPixel(img,(int)mediaX,(int)mediaY,PRETO);

    }
    PlotaGrafico(img,p);
}
void PlotaGrafico (Imagem *x, int p){
FILE *f1;
if(p==0){
    f1=fopen("Grafico3.pbm","w");
    salvaImagem(x,"Grafico3.pbm");
}

if(p==1){
    f1=fopen("Grafico6.pbm","w");
    salvaImagem(x,"Grafico6.pbm");
}

if(p==2){
     f1=fopen("Grafico10.pbm","w");
     salvaImagem(x,"Grafico10.pbm");
}

if(p==3){
    f1=fopen("Grafico15.pbm","w");
    salvaImagem(x,"Grafico15.pbm");

}

if(p==4){
    f1=fopen("Grafico20.pbm","w");
    salvaImagem(x,"Grafico20.pbm");

}
}

int main(){
    ConjAmostras*conjunto = (ConjAmostras*)malloc(sizeof(ConjAmostras));
    conjunto = ConjAmostr("Atenua.txt","Veloc.txt");
	printConjAmostras (conjunto);
	CalculaMedia(conjunto);
	int i=0;
	while(i<5){
      novaEstrutura(conjunto,i);
        i++;
	}
return 0;
}
