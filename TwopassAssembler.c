#include <stdio.h>
#include <Math.h>

typedef struct                // For DC Variables
{
    char name[50];
    char value[50];
    int address;
} var;

typedef struct                //For DS and JMP
{
    char name[50];
    int address;
} var1;

char mnemonic[50], dest[10], source[50];
var a[50];
var1 c[50],ds[50];
int noVar = 0, noDS = 0,noJMP = 0; //No of each of them

void handleDC(FILE *fp)     //First pass of assembler, variables stored, DC DC JMP handled.
{
    fseek(fp,0,SEEK_SET);
    char ch;
    int counter = 0;//PC

    while((ch = fgetc(fp)) != EOF)
  {
    ungetc(ch, fp);
    fscanf(fp, "%s", mnemonic);

    if(strcmp(mnemonic,"HLT"))
        fscanf(fp,"%s",dest);
    if(strcmp(mnemonic,"JMP") && strcmp(mnemonic,"HLT"))
   {
    fscanf(fp,"%s",source);
     int j;
     for(j=0;mnemonic[j]!='\0';j++) //check if the line has a label.
     {
        if(mnemonic[j] == ':')
        {
            int lp;
            for(lp=0; lp<j; lp++)   //If it has label, change the values of dest, source, mnemonic.
              c[noJMP].name[lp] = mnemonic[lp];
            c[noJMP].address = counter;
            noJMP++;
            int zz;
            for(zz = 0;dest[zz]!='\0';zz++)
                mnemonic[zz] = dest[zz];
            for(zz ; mnemonic[zz]!='\0';zz++)
                mnemonic[zz] = '\0';
            for(zz = 0;source[zz]!='\0';zz++)
              dest[zz] = source[zz];
            fscanf(fp,"%s",source);
            break;
        }
    }
    if(!(strcmp(dest,"DC")))    //If DC, store variable name, value, address in structure.
    {
      int i;
      for(i=0; i<50 && mnemonic[i] != '\0'; i++)
      {
         a[noVar].name[i] = mnemonic[i];
      }
      for(i=0; i<50 && source[i] != '\0'; i++)
      {
         a[noVar].value[i] = source[i];
      }
       a[noVar].address = counter;
       noVar++;
    }

     else if(!(strcmp(dest,"DS")))
     {
      int i;
      for(i=0; i<50 && mnemonic[i] != '\0'; i++)
      {
         ds[noDS].name[i] = mnemonic[i];
      }

      int num  = 0; int j;                  //Converting source to number .
           for(j =0 ; source[j] != '\0';j++)
           {
            int dig = source[j] - 48 ;
            num = dig + num*pow(10,j);
           }
       ds[noDS].address = counter;
       noDS++;
       counter+=num*2-2;                    //Increasing the counter by that number.
     }
    else if((source[0] == '#' ||source[0]=='&')||( strcmp(source,"R0") &&strcmp(source,"R1")&& strcmp(source,"R2")&& strcmp(source,"R3") && strcmp(source,"R4") && strcmp(source,"R5") && strcmp(source,"R6") && strcmp(source,"R7") && strcmp(source,"*R0") && strcmp(source,"*R1") && strcmp(source,"*R2") && strcmp(source,"*R3") && strcmp(source,"*R4") && strcmp(source,"*R5") && strcmp(source,"*R6") && strcmp(source,"*R7") ))
        counter+=2;
    }//if
    if(!(strcmp(mnemonic,"JMP"))) counter +=2;
    counter +=2 ;
  }//while
}


int main()
{
    printf("Enter file name:");   //opening file for reading and writing.
    char fileName[50];
    scanf("%s",fileName);


    FILE *fp = fopen(fileName,"r");
    handleDC(fp);
    fseek(fp,0,SEEK_SET);

    FILE *b = fopen("BinaryCode.txt","w");

    char ch;
while((ch = fgetc(fp)) != EOF )
{
    ungetc(ch, fp);

    int abc = fscanf(fp, "%s", mnemonic);
    if(abc!=-1)
 {
    if(strcmp(mnemonic,"HLT"))
        fscanf(fp,"%s",dest);
    if(strcmp(mnemonic,"JMP") && strcmp(mnemonic,"HLT"))
        fscanf(fp,"%s",source);

    int z;
    for(z=0 ; mnemonic[z]!='\0'; z++)   //Checking if name has label, and taking the value of mnemonic, source and dest accordingly.
    {
        if(mnemonic[z] == ':')
        {
            int zz;
            for(zz = 0;dest[zz]!='\0';zz++)
                mnemonic[zz] = dest[zz];
            for(zz ; mnemonic[zz]!='\0';zz++)
                mnemonic[zz] = '\0';
            for(zz = 0;source[zz]!='\0';zz++)
              dest[zz] = source[zz];
            fscanf(fp,"%s",source);
            break;
        }
    }

    if(!(strcmp(dest,"DC")))
        fprintf(b,"x%s\n",source);
    else if(!(strcmp(dest,"DS")))
    {
           int num  = 0; int j;
           for(j =0 ; source[j] != '\0';j++)    //Printing 0s num lines.
           {
            int dig = source[j] - 48 ;
            num = dig + num*pow(10,j);
           }
        for(j=0; j<num ; j++ )
             fprintf(b,"0000000000000000\n");
    }

else                //Printing mnemonic opcodes.
{

   if(!strcmp(mnemonic,"NEG"))      fprintf(b, "000000");
   else if(!strcmp(mnemonic,"AND")) fprintf(b, "000001");
   else if(!strcmp(mnemonic,"XOR")) fprintf(b, "000010");
   else if(!strcmp(mnemonic,"OR ")) fprintf(b, "000011");
   else if(!strcmp(mnemonic,"LSR")) fprintf(b, "000100");
   else if(!strcmp(mnemonic,"LSL")) fprintf(b, "000101");
   else if(!strcmp(mnemonic,"HLT")) fprintf(b, "0010100000000000\n");
   else if(!strcmp(mnemonic,"MOV")) fprintf(b, "001101");
   else if(!strcmp(mnemonic,"JMP"))
   {
       fprintf(b, "0011110000000000\n");
       int x;
       for(x=0 ; x<noJMP ; x++)
         {
             if(!(strcmp(dest,c[x].name)))
            {
                fprintf(b,"i%d\n",c[x].address);
                break;
            }
          }
   }
   else if(!strcmp(mnemonic,"ADD")) fprintf(b, "010000");
   else if(!strcmp(mnemonic,"SUB")) fprintf(b, "010001");

 if(strcmp(mnemonic,"HLT") && strcmp(mnemonic,"JMP"))
{
     if(!strcmp(dest,"*R0,")) fprintf(b, "00001");          //Printing dest opcode.
   else if(!strcmp(dest,"*R1,")) fprintf(b, "00101");
   else if(!strcmp(dest,"*R2,")) fprintf(b, "01001");
   else if(!strcmp(dest,"*R3,")) fprintf(b, "01101");
   else if(!strcmp(dest,"*R4,")) fprintf(b, "10001");
   else if(!strcmp(dest,"*R5,")) fprintf(b, "10101");
   else if(!strcmp(dest,"*R6,")) fprintf(b, "11001");
   else if(!strcmp(dest,"*R7,")) fprintf(b, "11101");
   else if(!strcmp(dest,"R0,"))  fprintf(b, "00000");
   else if(!strcmp(dest,"R1,")) fprintf(b, "00100");
   else if(!strcmp(dest,"R2,")) fprintf(b, "01000");
   else if(!strcmp(dest,"R3,")) fprintf(b, "01100");
   else if(!strcmp(dest,"R4,")) fprintf(b, "10000");
   else if(!strcmp(dest,"R5,")) fprintf(b, "10100");
   else if(!strcmp(dest,"R6,")) fprintf(b, "11000");
   else if(!strcmp(dest,"R7,")) fprintf(b, "11100");


     if(!strcmp(source,"*R0")) fprintf(b, "00001");         //Printing source opcode.
   else if(!strcmp(source,"*R1")) fprintf(b, "00101");
   else if(!strcmp(source,"*R2")) fprintf(b, "01001");
   else if(!strcmp(source,"*R3")) fprintf(b, "01101");
   else if(!strcmp(source,"*R4")) fprintf(b, "10001");
   else if(!strcmp(source,"*R5")) fprintf(b, "10101");
   else if(!strcmp(source,"*R6")) fprintf(b, "11001");
   else if(!strcmp(source,"*R7")) fprintf(b, "11101");
   else if(!strcmp(source,"R0"))  fprintf(b, "00000");
   else if(!strcmp(source,"R1")) fprintf(b, "00100");
   else if(!strcmp(source,"R2")) fprintf(b, "01000");
   else if(!strcmp(source,"R3")) fprintf(b, "01100");
   else if(!strcmp(source,"R4")) fprintf(b, "10000");
   else if(!strcmp(source,"R5")) fprintf(b, "10100");
   else if(!strcmp(source,"R6")) fprintf(b, "11000");
   else if(!strcmp(source,"R7")) fprintf(b, "11100");

   else if(source[0] == '#')
   {
        fprintf(b,"00010\nx");
        int z;
        for(z=0 ;source[z]!='\0';z++);
        for(z=1 ; source[z]!='\0'; z++)
         fprintf(b, "%c",source[z]);
   }
    else                //handling if source is a DC or DS value or address
    {
        fprintf(b,"00010\n");
        int x;int flag = 0;
        int ad = 0; char newSource[50];
        if(source[0] == '&')
            ad = 1;
        int lp ; int lp1;
        for(lp = ad,lp1 = 0 ;source[lp]!='\0';lp++,lp1++)
            newSource[lp1] = source[lp];

            newSource[lp1] = newSource[lp1+1] = '\0';

        for(x = 0;x<noVar ; x++)
        {
          if(!(strcmp(newSource,a[x].name)))
            {
              if(ad==0)   fprintf(b,"x%s",a[x].value);
              else         fprintf(b,"i%d",a[x].address);

                flag = 1;
                break;
            }
        }
        if(flag == 0)
        {
         for(x=0 ; x<noDS ; x++)
         {
             if(!(strcmp(newSource,ds[x].name)))
            {
                fprintf(b,"i%d",ds[x].address);
                flag = 1;
                break;
            }
          }
        }
}//if closes

   fprintf(b, "\n");
  }
}//else closes
}}//while closes
 fclose(fp);
 fclose(b);

 toHexPrint();

  return 0;
}

 void toHexPrint()                          //Printing the final code on terminal in Hexadecimal from Binary
{
    FILE *fp  = fopen("BinaryCode.txt","r");
    char line[25];
    char ch;
while((ch = fgetc(fp)) != EOF )
{
    ungetc(ch, fp);
    int a = 0; int l = 1; int base;
    int abc = fscanf(fp,"%s",line);
    int hex = 0;
if(abc!=-1)
{
    if(line[0] == 'x') //If hex
    {
        hex = 1; int lp=0;int lp1 = 0;
        for(lp=1;line[lp]!='\0';lp++);
        lp-=1;
        for(lp1=0;lp1<4-lp;lp1++)
            printf("%c",'0');
        for(lp1=1;line[lp1]!='\0';lp1++)
            printf("%c",line[lp1]);
    }
    else if(line[0] == 'i')  base=10;   //If integer
    else
    {
        base=2;l=0;
    }

    for(;line[l]!='\0' && hex == 0;l++)
            a = a * base + line[l] - 48;
    if(hex == 0)
    printf("%04x\n",a);
}}
   return 0;
}

