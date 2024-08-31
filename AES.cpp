#include <stdio.h>
#include <math.h>
/*結果*/
unsigned char process[16];
/*keyexpansion*/
unsigned char tempex[4];
unsigned char temp[4];//Addroundkeyinkex
int keyexpensiontime=0;
int minorkeyexpensiontime=0;
/*subbyte*/
int length = 8;
int subbytetype=0;//1表正常sub 2表keyex內
/*rcon*/
unsigned char temprcon[10]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};
/*其他*/
int total_round=0;
/*參考值*/
static const unsigned char Plaintext[16]={
  0x50,0x55,0x46,0x2d,
  0x73,0x65,0x63,0x75,
  0x72,0x69,0x74,0x79,
  0x62,0x65,0x73,0x74
};
/*PUF-securitybest*/
unsigned char key[11][16]={
{0x05,0x06,0x09,0x02
,0x0e,0x08,0x01,0x07
,0x09,0x04,0x04,0x09
,0x02,0x06,0x0d,0x02},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}};
/*41068111
  77171043
  27400472
  11606711*/
static const unsigned char SBox[16][16] =
{
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};
/*function*/
 /*Subbytes*/
void Subbytes(){
  int temparray1[length/2],temparray2[length/2];
  int subtime=0;
  unsigned char temp=0;
  int row=0;
  int column=0;
  if(subbytetype==1){
    subtime=16;
  }
  else subtime=4;
  for(int time=0;time<subtime;time++){
    if(subbytetype==1)temp=process[time];
    else temp=tempex[time];
      for(int x=0;x<length;x++){
    if(x<=3){
      temparray1[(length/2)-x-1]=temp%2;
    }
    else{
      temparray2[(length)-x-1]=temp%2;
    }
    temp/=2;
    }
    for(int x=0;x<length/2;x++){
      row+=(temparray2[(length/2)-x-1]*pow(2,x));
      column+=(temparray1[(length/2)-x-1]*pow(2,x));
    }
if(subbytetype==1) process[time]=SBox[row][column];
else{tempex[time]=SBox[row][column];}
  row=0;
  column=0;
  }
}
 /*keyexpansion*/
void RotWord(){
    subbytetype=2;
    for(int x=1;x<=4;x++){
        int num=x*4-1;
        if(x!=1)  tempex[x-2]=key[keyexpensiontime][num];
        else tempex[3]=key[keyexpensiontime][num];
    }
}
void rcon(){
  temp[0]^=temprcon[keyexpensiontime];
}
void Addroundkeyinkex(){
    for(int y=0;y<4;y++){
        for(int x=0;x<4;x++){
            temp[x]=key[keyexpensiontime][minorkeyexpensiontime+(4*x)];
            if(y==0) rcon();
            tempex[x]=tempex[x]^temp[x];
            key[keyexpensiontime+1][minorkeyexpensiontime+(4*x)]=tempex[x];
        }
        minorkeyexpensiontime++;
    }
    minorkeyexpensiontime=0;
}
void keypansion(){
  for(int x=0;x<10;x++){
    RotWord();
    Subbytes();
    Addroundkeyinkex();
    keyexpensiontime++;
  }
  subbytetype=1;
}
 /*Addroundkey*/
void Addroundkey(){
  if(total_round==0){
      for(int x=0;x<16;x++){
    process[x]=Plaintext[x];
    }
  }
  for(int x=0;x<16;x++){
    process[x]=process[x]^key[total_round][x];
    }
}
 /*ShiftRows*/
void ShiftRows(){
  int tempspace=0;
  for(int x=0; x<4;x++){
    if(x==1){
      for(int y=4;y<16;y++){
        if(y%4!=3){
          tempspace=process[y];
          process[y]=process[y+1];
          process[y+1]=tempspace;
        }
      }
    }
    else if(x==2){
      for(int y=8;y<16;y++){
        if(y%4!=3){
          tempspace=process[y];
          process[y]=process[y+1];
          process[y+1]=tempspace;
        }
      }
    }
    else if(x==3){
      for(int y=12;y<16;y++){
        if(y%4!=3){
          tempspace=process[y];
          process[y]=process[y+1];
          process[y+1]=tempspace;
        }
      }
    }
  }
}
 /*mixcolumns*/
int GFMul(int a, int b) { // Galois Field (256) Multiplication of two Bytes
    int p = 0;

    for (int counter = 0; counter < 8; counter++) {
        if ((b & 1) != 0) {
            p ^= a;
        }

        bool hi_bit_set = (a & 0x80) != 0;
        a <<= 1;
        if (hi_bit_set) {
            a ^= 0x1B; /* x^8 + x^4 + x^3 + x + 1 */
        }
        b >>= 1;
    }

    return p;
}

void mixcolumns(){
    unsigned char mixtemp[4][4];
    for(int i = 0; i < 4; i ++){
        for(int j = 0; j < 4; j ++){
          if(i%4 == 0){
            mixtemp[i][j] = GFMul(0x02, process[j])^GFMul(0x03, process[4 + j])^process[8 + j]^process[12 + j];
          }
          if(i%4 == 1){
            mixtemp[i][j] = process[j]^GFMul(0x02, process[4 + j])^GFMul(0x03, process[8 + j])^process[12 + j];
          }
          if(i%4 == 2){
            mixtemp[i][j] = process[j]^process[4 + j]^GFMul(0x02, process[8 + j])^GFMul(0x03, process[12 + j]);
          }
          if(i%4 == 3){
            mixtemp[i][j] = GFMul(0x03, process[j])^process[4 + j]^process[8 + j]^GFMul(0x02, process[12 + j]);
          }
        }
    }
    for(int a = 0; a < 4; a ++){
        for(int b = 0; b < 4; b ++){
            process[4*a + b] = mixtemp[a][b];
        }
    }
}

void Debug(){
  for(int x=0;x<16;x++){
      printf("%02x",process[x]);
      if((x+1)%4==0){
        printf("\n");
      }
  }
  printf("\n");
}

int main(){
  keypansion();
  Addroundkey();
  //Debug();
  total_round++;
  while(total_round<10){
    Subbytes();
    ShiftRows();
    mixcolumns();
    //Debug();
    Addroundkey();
    //Debug();
    total_round++;
  }
  Subbytes();
  ShiftRows();
  Addroundkey();
  Debug();
}
