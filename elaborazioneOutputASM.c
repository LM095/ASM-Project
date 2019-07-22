#include <stdio.h>

#define LUNG 10

int main(void)
{    
    char int_gen = '1';
    char int_dw = '0';
    char int_wm = '1';
    int ct = 0;
    int flag = 0;

    int consumoTot = 250;

    char output[LUNG] = {0};

    output[ct] = int_gen;
    output[++ct] = int_dw;
    output[++ct] = int_wm;
    output[++ct] = '-';

    if(consumoTot == 0)
    {
        output[++ct] = '0';
        output[++ct] = '0';
    }
    if(consumoTot > 0 && consumoTot <= 150)
    {
        output[++ct] = 'F';
        output[++ct] = '1';
    }
    if(consumoTot > 150 && consumoTot <= 300)
    {
        output[++ct] = 'F';
        output[++ct] = '2';
    }
    if(consumoTot > 300 && consumoTot <= 450)
    {
        output[++ct] = 'F';
        output[++ct] = '3';
    }
    if(consumoTot > 450)
    {
        output[++ct] = 'O';
        output[++ct] = 'L';
    }   
    
    output[++ct] = '\n';

    if(flag == 1)
        output[++ct] = '\0';

    for(int i = 0; i<LUNG; i++)
        printf("%c", output[i]);

    return 0;
}