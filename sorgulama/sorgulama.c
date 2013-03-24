#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_UCUS_SAY 1000
#define MAX_DIZI_BOYUT 50

struct ucus {
    int ucusNo;
    char nereden[21];
    char nereye[21];
    int saat;
    int dak;
    int kapasite;
    int bosKoltuk;
    float fiyat;
};

struct bilet {
    int ucusNo;
    char TC[11];
    float fiyat;
};

struct doluluk {
    int ucusNo;
    int bosKoltuk;
    float BKO;
};

typedef struct ucus UCUS;
typedef struct bilet BILET;
typedef struct doluluk DOLULUK;

int ucusNoAl();
void binaryGoruntule(FILE *);
void dosyaAc();
void ucusGoruntule(FILE *);
void ucusaAitBilgiler(FILE *);
void sehreAitUcuslar(FILE *);
void dolulugaGoreYazdir(FILE *);
void biletListele(FILE *);
void sirala(struct doluluk *,int);
void baslikYazdir();
void yazdir(FILE *,int);
void menuSecim(FILE *);
void beklet();

int main()
{
    system("color f5");

	dosyaAc();

    return 0;
}

void dosyaAc()
{
    FILE *fPtr;

    if((fPtr=fopen("ucus.dat","rb"))==NULL)
        printf("Dosya acilamadi!\n");
    else
    {
        binaryGoruntule(fPtr);
		menuSecim(fPtr);

        fclose(fPtr);
    }
}

void menuSecim(FILE *fPtr)
{
    int secim=0;

    printf("Program yukleniyor...\n");
    beklet();
    printf("Bu program ile asagidaki islemleri yapabilirsiniz");

    while(secim!=6)
    {
        //system("cls");
        printf("\n--------------------------------------------------------------------------------\n");
        printf(" (1) Bir ucusun bilgilerinin listelenmesi\n"
               " (2) Bir ucusun bilgilerinin ve o ucusa iliskin satilan biletlerin listelenmesi\n"
               " (3) Bir yerden kalkan ucuslarin bilgilerinin listelenmesi\n"
               " (4) Koltuk doluluk orani %%50' nin altinda olan ucuslarin listelenmesi\n"
               " (5) Bir yolcunun biletlerinin listelenmesi\n"
               " (6) Cikis\n");
        printf("Seciminizi giriniz: ");
        scanf("%d",&secim);
        printf("\n");

        switch(secim)
        {
            case 1: ucusGoruntule(fPtr);break;
            case 2: ucusaAitBilgiler(fPtr);break;
            case 3: sehreAitUcuslar(fPtr);break;
            case 4: dolulugaGoreYazdir(fPtr);break;
            case 5: biletListele(fPtr);break;
            case 6:
                printf("Program kapatiliyor...\n");
                beklet();
                printf("Program kapatildi :) \n");
                break;
        }
    }
}

void beklet()
{
    long int i;

    for(i=0;i<1000000000;i++) {}
}

void binaryGoruntule(FILE *fPtr)
{
    UCUS ucus;
    int i;

    rewind(fPtr);

    printf("\n");

    baslikYazdir();

    for(i=0;i<MAX_UCUS_SAY;i++)
    {
        fread(&ucus,sizeof(UCUS),1,fPtr);
        if(ucus.ucusNo!=0)
            yazdir(fPtr,ucus.ucusNo);
    }

    printf("\n");
}

int ucusNoAl()
{
    int ucusNo;

    do
    {
        printf("Ucus numarasini giriniz: ");
        scanf("%d",&ucusNo);
    }while(ucusNo<1 || ucusNo>1000);

    return ucusNo;
}

void ucusGoruntule(FILE *fPtr)
{
    UCUS ucus;
    int ucusNo;

    ucusNo=ucusNoAl();

    fseek(fPtr,sizeof(UCUS)*(ucusNo-1),SEEK_SET);
    fread(&ucus,sizeof(UCUS),1,fPtr);

    if(ucus.ucusNo==0)
        printf("\tBoyle bir ucus yoktur!\n");
    else
        baslikYazdir();
        yazdir(fPtr,ucusNo);
}

void ucusaAitBilgiler(FILE *fPtr)
{
    UCUS ucus;
    BILET bilet;
    int ucusNo;
    int bulundu=0;
    int biletSay=0;
    float toplamFiyat=0;
    int baslikKontrol=0;

    FILE *sPtr;

    ucusNo=ucusNoAl();

    fseek(fPtr,sizeof(UCUS)*(ucusNo-1),SEEK_SET);
    fread(&ucus,sizeof(UCUS),1,fPtr);

    if(ucus.ucusNo==0)
        printf("Boyle bir ucus yoktur!\n");
    else
    {
        baslikYazdir();
        yazdir(fPtr,ucusNo);
        if((sPtr=fopen("bilet.txt","r"))==NULL)
            printf("Dosya acilamadi!\n");
        else
        {
            fscanf(sPtr,"%d %s %f",&bilet.ucusNo,bilet.TC,&bilet.fiyat);

            while(!feof(sPtr))
            {
                if(ucusNo==bilet.ucusNo)
                {
                    baslikKontrol++;
                    if(baslikKontrol==1)
                            printf("\nTC Kimlik No\tFiyat\n------------\t-----\n");
                    printf("%12s\t%5.2f\n",bilet.TC,bilet.fiyat);
                    toplamFiyat+=bilet.fiyat;
                    biletSay++;
                    bulundu=1;
                }
                fscanf(sPtr,"%d %s %f",&bilet.ucusNo,bilet.TC,&bilet.fiyat);
            }

            if(!bulundu)
                printf("\n\tBu ucus icin bilet satilmamis!\n");
            else
            {
                printf("--------------------------------\n");
                printf("Toplam bilet sayisi: %d\n"
                       "Toplam kazanc: %.2f TL\n"
                       "Ortalama bilet fiyati: %.2f TL\n",
                       biletSay,toplamFiyat,toplamFiyat/biletSay);
            }

            fclose(sPtr);
        }
    }
}

void sehreAitUcuslar(FILE *fPtr)
{
    UCUS ucus;
    int i;
    char sehir[21];

    rewind(fPtr);

    printf("Hangi sehirden kalkan ucuslari goruntulemek istiyorsunuz: ");
    scanf("%s",sehir);

    baslikYazdir();
    for(i=0;i<MAX_UCUS_SAY;i++)
    {
        fread(&ucus,sizeof(UCUS),1,fPtr);
        if(!strcmp(sehir,ucus.nereden))
            yazdir(fPtr,ucus.ucusNo);
    }
}

void dolulugaGoreYazdir(FILE *fPtr)
{
    UCUS ucus;
    int i,j=0;
    DOLULUK doluluk[MAX_DIZI_BOYUT];

    rewind(fPtr);

    for(i=0;i<MAX_UCUS_SAY;i++)
    {
        fread(&ucus,sizeof(UCUS),1,fPtr);
        if(ucus.ucusNo!=0 && ((float)100*(ucus.kapasite-ucus.bosKoltuk)/ucus.kapasite)<50)
        {
            doluluk[j].ucusNo=ucus.ucusNo;
            doluluk[j].bosKoltuk=ucus.bosKoltuk;
            doluluk[j].BKO=(float)100*(ucus.kapasite-ucus.bosKoltuk)/ucus.kapasite;
            j++;
        }
    }

    sirala(doluluk,j);
    baslikYazdir();
    for(i=0;i<j;i++)
    {
        fseek(fPtr,sizeof(UCUS)*(doluluk[i].ucusNo-1),SEEK_SET);
        fread(&ucus,sizeof(UCUS),1,fPtr);

        yazdir(fPtr,ucus.ucusNo);
    }
}

void biletListele(FILE *fPtr)
{
    UCUS ucus;
    BILET bilet;
    char TC[11];

    FILE *sPtr;

    if((sPtr=fopen("bilet.txt","r"))==NULL)
        printf("Dosya acilamadi!\n");
    else
    {
        printf("Sorgulamak istediginiz TC kimlik numarasi: ");
        scanf("%s",TC);

        fscanf(sPtr,"%d %s %f",&bilet.ucusNo,bilet.TC,&bilet.fiyat);
        while(!feof(sPtr))
        {
            if(strstr(TC,bilet.TC))
            {
                fseek(fPtr,sizeof(UCUS)*(bilet.ucusNo-1),SEEK_SET);
                fread(&ucus,sizeof(UCUS),1,fPtr);
                printf("\nUcus No  Kalkis Yeri  Varis Yeri  Zamani  Fiyati\n"
                       "-------  -----------  ----------  ------  ------\n");
                printf("%-7d  %-11s  %-11s  %02d:%02d  %6.2f\n",
                       ucus.ucusNo,ucus.nereden,ucus.nereye,ucus.saat,ucus.dak,bilet.fiyat);
            }
            fscanf(sPtr,"%d %s %f",&bilet.ucusNo,bilet.TC,&bilet.fiyat);
        }
    }
}

void sirala(DOLULUK *dizi,int boyut)
{
    int i,j;
    DOLULUK gecici;

    for(i=0;i<boyut;i++)
        for(j=1;j<boyut-1;j++)
        {
            if(dizi[j-1].BKO>dizi[j].BKO)
            {
                    gecici=dizi[j-1];
                    dizi[j-1]=dizi[j];
                    dizi[j]=gecici;
            }

            if(dizi[j-1].BKO==dizi[j].BKO && dizi[j-1].bosKoltuk<dizi[j].bosKoltuk)
            {
                    gecici=dizi[j-1];
                    dizi[j-1]=dizi[j];
                    dizi[j]=gecici;
            }
        }
}

void baslikYazdir()
{
    //system("cls");
    printf("\nUcus No  Kalkis Yeri  Varis Yeri  Zamani  Kapasite  Bos Koltuk  Doluluk %%\n"
           "-------  -----------  ----------  ------  --------  ----------  ---------\n");
}

void yazdir(FILE *fPtr,int ucusNo)
{
    UCUS ucus;

    fseek(fPtr,sizeof(UCUS)*(ucusNo-1),SEEK_SET);
    fread(&ucus,sizeof(UCUS),1,fPtr);

    printf("%-7d  %-11s  %-11s  %02d:%02d  %8d  %10d  %9.2f\n",
           ucus.ucusNo,ucus.nereden,ucus.nereye,ucus.saat,ucus.dak,ucus.kapasite,ucus.bosKoltuk,(float)100*(ucus.kapasite-ucus.bosKoltuk)/ucus.kapasite);
}
