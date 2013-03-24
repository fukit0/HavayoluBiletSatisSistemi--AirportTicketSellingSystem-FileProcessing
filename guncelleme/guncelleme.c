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

typedef struct ucus UCUS;
typedef struct bilet BILET;

int ucusNoAl();
void binaryGoruntule(FILE *);
void dosyaAc();
void menuSecim(FILE *);
void ucusEkleme(FILE *);
void ucusSilme(FILE *);
void kalkisSaatiDegistir(FILE *);
void biletSatisi(FILE *);
void beklet(long int);
void yazdir(FILE *,int);
void baslikYazdir();
float biletFiyatHesapla(UCUS);


int main()
{
    system("color f5");

    dosyaAc();

    return 0;
}

void dosyaAc()
{
    FILE *fPtr;

    if((fPtr=fopen("ucus.dat","rb+"))==NULL)
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
    beklet(1000000000);
    printf("Bu program ile asagidaki islemleri yapabilirsiniz");

    while(secim!=5)
    {
        //system("cls");
        beklet(300000000);
        printf("\n--------------------------------------------------------------------------------\n");
        printf(" (1) Yeni bir ucusun eklenmesi\n"
               " (2) Bir ucusun kaydinin silinmesi\n"
               " (3) Bir ucusun kalkis zamaninin degistirilmesi\n"
               " (4) Bilet satin alma\n"
               " (5) Cikis\n");
        printf("Seciminizi giriniz: ");
        scanf("%d",&secim);
        printf("\n");

        switch(secim)
        {
            case 1: ucusEkleme(fPtr);break;
            case 2: ucusSilme(fPtr);break;
            case 3: kalkisSaatiDegistir(fPtr);break;
            case 4: biletSatisi(fPtr);break;
            case 5:
                printf("Program kapatiliyor...\n");
                beklet(1000000000);
                printf("Program kapatildi :)\n");
                break;
        }
    }
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

void ucusEkleme(FILE *fPtr)
{
    int ucusNo;
    UCUS ucus;

    ucusNo=ucusNoAl();

    fseek(fPtr,sizeof(UCUS)*(ucusNo-1),SEEK_SET);
    fread(&ucus,sizeof(UCUS),1,fPtr);

    if(ucus.ucusNo==ucusNo)
        printf("\t%d numarali ucus bulundugundan ekleme yapamazsiniz!\n",ucusNo);

    else if(ucus.ucusNo==0)
        {
            ucus.ucusNo=ucusNo;
            printf("Kalkis yeri: ");
            scanf("%s",ucus.nereden);
            printf("Varis yeri: ");
            scanf("%s",ucus.nereye);
            printf("Saati ve dakikayi giriniz: ");
            scanf("%d %d",&ucus.saat,&ucus.dak);
            printf("Yolcu kapasitesini giriniz: ");
            scanf("%d",&ucus.kapasite);
                ucus.bosKoltuk=ucus.kapasite;
            printf("Baslangic fiyatini giriniz: ");
            scanf("%f",&ucus.fiyat);

            fseek(fPtr,sizeof(UCUS)*(ucusNo-1),SEEK_SET);
            fwrite(&ucus,sizeof(UCUS),1,fPtr);
        }
}

void ucusSilme(FILE *fPtr)
{
    FILE *sPtr,*gPtr;
    UCUS ucus;
    BILET bilet;
    int ucusNo;

    ucusNo=ucusNoAl();

    fseek(fPtr,sizeof(UCUS)*(ucusNo-1),SEEK_SET);
    fread(&ucus,sizeof(UCUS),1,fPtr);

    if(ucus.ucusNo==0)
        printf("\t%d numarali ucus bulunmadigindan silme yapamazsiniz!\n",ucusNo);
    else
    {
        if((sPtr=fopen("bilet.txt","r"))==NULL)
            printf("Dosya acilamadi!\n");
        else
        {
            if((gPtr=fopen("gecici.txt","w"))==NULL)
                printf("Gecici dosya yaratilamadi!\n");
            else
            {
                fscanf(sPtr,"%d %s %f",&bilet.ucusNo,bilet.TC,&bilet.fiyat);
                while(!feof(sPtr))
                {
                    if(bilet.ucusNo!=ucusNo) // siradan erisimli dosyadaki silinmek istenen ucuslar yeni dosyada yer almayacak sekilde tasiniyor
                        fprintf(gPtr,"%d %s %.2f\n",bilet.ucusNo,bilet.TC,bilet.fiyat);

                    fscanf(sPtr,"%d %s %f",&bilet.ucusNo,bilet.TC,&bilet.fiyat);
                }
            }
            fclose(sPtr);
            fclose(gPtr);
            remove("bilet.txt");
            rename("gecici.txt","bilet.txt");
        }

         fseek(fPtr,sizeof(UCUS)*(ucusNo-1),SEEK_SET);		// dogrudan eriþimli dosyadaki silinen ucusa ait bilgiler sifirlaniyor
         ucus.ucusNo=0;
         strcpy(ucus.nereden,"");
         strcpy(ucus.nereye,"");
         ucus.saat=0;
         ucus.dak=0;
         ucus.kapasite=0;
         ucus.fiyat=0;
         fwrite(&ucus,sizeof(UCUS),1,fPtr);
    }
}

void kalkisSaatiDegistir(FILE *fPtr)
{
    UCUS ucus;
    int ucusNo;

    ucusNo=ucusNoAl();

    fseek(fPtr,sizeof(UCUS)*(ucusNo-1),SEEK_SET);
    fread(&ucus,sizeof(UCUS),1,fPtr);

    if(ucus.ucusNo==0)
        printf("\t%d numarali ucus bulunmadigindan degisiklik yapamazsiniz!\n",ucusNo);
    else
    {
        printf("Yeni kalkis saatini ve dakikasini giriniz: ");
        scanf("%d %d",&ucus.saat,&ucus.dak);
        fseek(fPtr,sizeof(UCUS)*(ucusNo-1),SEEK_SET);
        fwrite(&ucus,sizeof(UCUS),1,fPtr);
    }
}

void biletSatisi(FILE *fPtr)
{
    FILE *sPtr;
    UCUS ucus;
    int ucusNo;
    int biletSay;
    int sayac=0;
    int i;
    char sagSerbest;
    char TC[11];
    float fiyat;
    float toplamFiyat=0;

    ucusNo=ucusNoAl();

    fseek(fPtr,sizeof(UCUS)*(ucusNo-1),SEEK_SET);
    fread(&ucus,sizeof(UCUS),1,fPtr);

    if(ucus.ucusNo==0)
        printf("\t%d numarali ucus bulunmadigindan bilet satin alamazsiniz!\n",ucusNo);
    else
    {
        printf("Satin almak istediginiz bilet sayisini giriniz: ");
        scanf("%d",&biletSay);

        if(biletSay>ucus.bosKoltuk)
            printf("\tBu ucusta yeterli sayida bos koltuk bulunmamaktadir!\n");

        else
        {
            printf("\nKoltuk fiyatlari:\n");
            fiyat=biletFiyatHesapla(ucus);
            for(i=0;i<biletSay;i++)
            {

               if (fiyat != biletFiyatHesapla(ucus) ) {		// bilet fiyati degistigi anda o zamana kadarki biletler yazdiriliyor

                    printf("%d adet bilet %.2lf TL.\n", sayac, fiyat);
                    sayac = 0;

               }

               sayac++;
               toplamFiyat+=biletFiyatHesapla(ucus);
               fiyat=biletFiyatHesapla(ucus);
               ucus.bosKoltuk--;

               if (i == biletSay - 1) {   // eger son bilet ise dongu bittigi için o hali ile ekrana yazma isi yapiliyor

                    printf("%d adet bilet %.2lf TL.\n", sayac, fiyat);

               }

            }
            printf("---------------------------------\n");
            printf("    Toplam bilet fiyati: %8.2f\n",toplamFiyat);

            printf("\nOnayliyor musunuz? (e,E,h,H) ");
            fflush(stdin);
            scanf("%c",&sagSerbest);

            if(sagSerbest=='e' || sagSerbest=='E')
            {
                if((sPtr=fopen("bilet.txt","a"))==NULL)
                    printf("Dosya acilamadi!\n");
                else
                {
                    printf("\n   Yolcularin TC kimlik numaralarini giriniz...\n");
                    beklet(200000000);
                    ucus.bosKoltuk+=biletSay;
                    for(i=0;i<biletSay;i++)
                    {
                        printf("%4d. yolcunun TC kimlik numarasi: ",i+1);
                        scanf("%s",TC);
                        fprintf(sPtr,"%3d %11s %5.2f\n",ucusNo,TC,biletFiyatHesapla(ucus));
                        ucus.bosKoltuk--;
                    }
                    fclose(sPtr);

                    fseek(fPtr,sizeof(UCUS)*(ucusNo-1),SEEK_SET);
                    fwrite(&ucus,sizeof(UCUS),1,fPtr);
                }
            }
        }
    }
}

float biletFiyatHesapla(UCUS ucus)
{
    int i;
    float BKO;
    float fiyat;

    BKO=100-((float)100*(ucus.kapasite-ucus.bosKoltuk)/ucus.kapasite);
    fiyat=ucus.fiyat;

    for(i=0;i<10;i++)
    {
        if(BKO<=(float)((10-i)*10.0) && BKO>(float)((10-i-1)*10.0)) //dongunun her donusunde yeni araliklar hesaplaniyor
        {
            fiyat*=(float)(1.0+i/10.0); //kosulu saglayan katsayi hesaplanip fiyata yansitiliyor
            break;
        }
    }

    return fiyat;
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

void beklet(long int N)
{
    long int i;

    for(i=0;i<N;i++) {}
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
