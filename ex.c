#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include "inc/btree.h"
#include "inc/bt.h"
#include "soundex.h"
#include <math.h>
#include <time.h>
#include <locale.h>
#define MAX_WORD 50
#define MAX_MEAN 15000

void readfile(BTA**root){
  
        *root=btcrt("dic",0,0);

    FILE *fin;
    long count=0;

    fin=fopen("anhviet.txt","r");
    if(!fin){
        printf("khong mo dc file hoac file khong ton tai!!!\n");
    }else printf("mo file thanh cong\n");

    char word[MAX_WORD],meaning[MAX_MEAN];
    char tmp[100];
    meaning[0]='\0';

    while(!feof(fin)){
        fgets(tmp,100,fin);
        while(tmp[0]!='@'){
            strcat(meaning,tmp);//noi2 sau vs nhau
            if(feof(fin))break;
            fgets(tmp,100,fin);
        }
       
        btins(*root,word,meaning,MAX_MEAN*sizeof(char));
       
        // truyen du lien vao sau word
        if(tmp[0]=='@'){
            strcpy(word,tmp);//copy sau tmp sang word
            meaning[0]='\0';
            //tach sau word tai ki tu '/'dau tien
            int i,j,m=0,n;
            n=strlen(word);
            for(i=0;i<n;i++)
                if(word[i]=='/')break;
            //phan sau'/' se chuyen vo meaning
            for(j=i;j<=n;j++){
                meaning[m]=word[j];
                m++;
            }word[i-1]='\0';

            for(i=1;i<=n;i++){
               word[i-1]=word[i];
            }
        }
    }
fclose(fin);
}

void search(BTA*root){
    char word[MAX_WORD],meaning[MAX_MEAN];
    int size=500;
    int chon=1;
    while(10){
    printf("nhap tu can tim :\n");
    __fpurge(stdin);
    gets(word);
    if(btsel(root,word,meaning,MAX_MEAN*sizeof(char), &size)){
        printf("khong co tu nay (uwu) \n");
    }else printf("nghia:%s\n",meaning );
    printf("ban muon tra them tu khong yes=1 no= 0\n");
    scanf("%d",&chon);
    
    if(chon==0)break;
    
    while(chon != 1 && chon != 0){
        printf("Nhap lai\n");
        scanf("%d", &chon);
    }
    }

}

void del(BTA*root){
    char word[MAX_WORD],meaning[MAX_MEAN];
    int size=500;
    int chon =1;
    while(chon==1){
        printf("nhap tu can xoa :\n");
         __fpurge(stdin);
            gets(word);
        if(btsel(root,word,meaning,MAX_MEAN*sizeof(char), &size)){
        printf("khong co tu nay trong tu dien (-.-)\n");
        }else btdel(root,word);
        printf("ban co muon xoa them  tu ko?? co=1 ko= 0\n");
        scanf("%d",&chon);
        if(chon==0)break;
        while(chon!=1 && chon !=0){
            printf("nhap lai\n");
            scanf("%d",&chon);
        }
    }
}

void add(BTA*root){
    char word[MAX_WORD],meaning[MAX_MEAN];
    int size=500;
    int chon =1;
    while(chon==1){
        printf("nhap tu muon them:\n");
        scanf("%s",word);
        if(btsel(root,word,meaning,MAX_MEAN*sizeof(char), &size)){
            printf("nhap nghia cua tu moi them:\n");
             __fpurge(stdin);
            gets(meaning);
            btins(root,word,meaning,MAX_MEAN*sizeof(char));
        }else printf("da co tu nay roi (-.-)\n");
        printf("ban muon them tu khong?? co=1 ko=0\n");
        scanf("%d",&chon);
        if(chon==0)break;
        while(chon!=0 && chon!=1){
            printf("nhap lai\n");
            scanf("%d",&chon);
        }
    }
}

void creatSoundexTree(BTA *Dic, BTA **soundexTree){
    char word[MAX_WORD],code[1000];
    int rsize=1000;
    char sou[6];
    *soundexTree=btcrt("soundexTree",0,0);
    btsel(Dic,"",code,1000*sizeof(char),&rsize);
    while(btseln(Dic,word,code,1000*sizeof(char),&rsize)==0){
        SoundEx(sou,word,5,1);
        btins(*soundexTree,word,sou,6*sizeof(char));
       
    }
}
//goi y ra
int suggestion(BTA *soundexTree, char *word, char s[5][MAX_WORD]){
    int rsize=1000,i=0;
    char en[MAX_WORD],sou[5],code[5];
    SoundEx(code,word,4,1);
    btsel(soundexTree,"@",sou,5*sizeof(char),&rsize);
    while(btseln(soundexTree,en,sou,5*sizeof(char),&rsize)==0&&i<5){
        if(strcmp(code,sou)==0)
            strcpy(s[i++],en);  
    }
    return i;
}

void Find(BTA *soundexTree){
    char word[MAX_WORD], suggest[5][MAX_WORD];
    int chon=1;
    while(chon==1){
        printf("---nhap tu can goi y:\n"); 
         __fpurge(stdin);
            gets(word);
        int k=suggestion(soundexTree,word,suggest);
        printf("---so tu la: %d\n",k);
        for(int i=0;i<k;i++) 
            printf("%s\n",suggest[i]);
         printf("ban muon tra them goi y tu khong?? co=1 ko=0\n");
        scanf("%d",&chon);
        if(chon==0)break;
        while(chon!=0 && chon!=1){
            printf("nhap lai\n");
            scanf("%d",&chon);
        }
    }
}

int main(){
    //doc file
    char pb[]="dic.pb";
    btinit();
    BTA *dic;
    dic=btopn(pb,0,0);
    if(dic=NULL){
        dic=btcrt(pb,0,0);
    }
    readfile(&dic);

    BTA*SDT;
    btinit();
    SDT=btopn("soundexTree",0,0);
    if(!SDT){
        creatSoundexTree(dic,&SDT);
    }

     int chon;
    do
    {
        
        printf("\t\t\t ============MENU================================\n");
        printf("\t\t\t |1.Tra tu                                      |\n");
        printf("\t\t\t |2.Xoa tu                                      |\n");
        printf("\t\t\t |3.Them tu                                     |\n");
        printf("\t\t\t |4.goi y tu                                    |\n");
        printf("\t\t\t |5.THoat                                       |\n");
        printf("\t\t\t ================================================\n");
        printf("Ban chon: ");
        scanf("%d", &chon);
         switch(chon){
            case 1:
            search(dic);
            break;

            case 2:
            del(dic);
            break;

            case 3:
            add(dic);
            break;

            case 4:
             Find(SDT);
             break;

            case 5:
            printf("ket thuc chuong trinh\n");
            break;

            default:
            printf("Ban chon sai. Moi ban chon lai MENU!\n");
            break;

         }
    } while(chon!=5);
   

    btcls(dic);
    btcls(SDT);
//gcc -o ex1 ex.c lib/libbt.a


}
