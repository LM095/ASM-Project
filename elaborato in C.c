#include <stdio.h>
#include <stdlib.h>

#define NUMLOAD 10
#define NUMRES 3
#define NUMINT 3
#define C_FORNO 200
#define C_FRIGO 30
#define C_ASPIRAPOLVERE 120
#define C_PHON 100
#define C_DW 200
#define C_WM 180
#define C_LAMPADE60 24
#define C_LAMPADE100 40
#define C_HIFI 20
#define C_TV 40

#define FASCIA1 150
#define FASCIA2 300
#define FASCIA3 450

int main (void)
{
    int bitRes[NUMRES] = {0};   //res_gen, res_dw, res_wm
    int bitLoad[NUMLOAD] = {0}; 
    int consumiLoad[NUMLOAD] = {C_FORNO, C_FRIGO, C_ASPIRAPOLVERE, C_PHON, C_DW, C_WM, C_LAMPADE60, C_LAMPADE100, C_HIFI, C_TV};
    int consumoTot = 0;
    int avanti = 0;
    int ctOL = 0;
    int bitInt[NUMINT] = {0};   //int_gen, int_dw, int_wm
    int flagTurnOn = 0;     //se è = 1 allora la macchina è accesa e funzionante
    int flagTurnOffDW = 0;  //se è = 1 allora int_dw è appena stato spento nel ciclo corrente
    int flagTurnOffWM = 0;  //se è = 1 allora int_wm è appena stato spento nel ciclo corrente

    do
    {
        avanti = 0;
        consumoTot = 0;
        flagTurnOffDW = 0;
        flagTurnOffWM = 0;

        /*questa parte di input NON è richiesta in ASM*/
        /*INIZIO*/
        /*for(int i = 0; i<NUMRES; i++)
        {
            printf("Inserisci il %i' bit di RES_%s: ", i+1, (i == 0) ? "GEN" : (i == 1) ? "DW" : "WM");
            scanf("%i", &bitRes[i]);
        }

        printf("\n");

        for(int i = 0; i<NUMLOAD; i++)
        {
            printf("Inserisci il %i' bit di LOAD: ", i+1);
            scanf("%i", &bitLoad[i]);
        }*/

        printf("Inserisci 3 bit di RES: ");
        scanf("%i %i %i", &bitRes[0], &bitRes[1], &bitRes[2]);

        printf("Inserisci 10 bit di LOAD: ");
        scanf("%i %i %i %i %i %i %i %i %i %i", &bitLoad[0], &bitLoad[1], &bitLoad[2], &bitLoad[3], &bitLoad[4], &bitLoad[5], &bitLoad[6], &bitLoad[7], &bitLoad[8], &bitLoad[9]);
        /*FINE*/

        //CASO ACCENSIONE MACCHINA (RES_GEN)
        if(bitRes[0] == 1 && flagTurnOn == 0)   //res_gen
        {
            flagTurnOn = 1;
            bitInt[0] = 1;  //int_gen
            bitInt[1] = 1;  //int_dw
            bitInt[2] = 1;  //int_wm
        }

        //CASO RIACCENSIONE DI RES_DW
        if(bitRes[1] == 1)   //res_dw
            bitInt[1] = 1;   //int_dw

        //CASO RIACCENSIONE DI RES_WM
        if(bitRes[2] == 1)   //res_wm
            bitInt[2] = 1;   //int_wm

        //CASO MACCHINA SPENTA
        if(bitInt[0] == 0)  //int_gen
        {
            consumoTot = 0;
            bitInt[0] = 0;  //int_gen
            bitInt[1] = 0;  //int_dw
            bitInt[2] = 0;  //int_wm
        }
        //CASO MACCHINA CORRETTAMENTE ACCESA
        else
        {
            for(int i = 0; i<NUMLOAD; i++)
            {
                if(i == 4)
                    consumoTot = consumoTot + (bitLoad[i] * consumiLoad[i] * bitInt[1]);  //int_dw
                else if(i == 5)
                    consumoTot = consumoTot + (bitLoad[i] * consumiLoad[i] * bitInt[2]);  //int_wm
                else
                    consumoTot = consumoTot + (bitLoad[i] * consumiLoad[i]);
            }            

            //CONTEGGIO OL
            if(consumoTot > 450)
            {
                ctOL++;

                if(ctOL == 4)
                {
                    bitInt[1] = 0;   //int_dw
                    flagTurnOffDW = 1;
                }
                if(ctOL == 5)
                {
                    bitInt[2] = 0;   //int_wm
                    flagTurnOffWM = 1;
                }
                if(ctOL == 6)
                {
                    bitInt[0] = 0;   //int_gen
                    flagTurnOn = 0;
                    consumoTot = 0;
                }
            }
            else  // se non siamo più in OL allora azzera il contatore
                ctOL = 0;

            //SISTEMAZIONE consumoTot TOGLIENDO I CONSUMI NON RICHIESTI
            if(bitInt[1] == 0 && flagTurnOffDW == 1 && bitLoad[4] == 1)   //int_dw
                consumoTot -= consumiLoad[4];
            if(bitInt[2] == 0 && flagTurnOffWM == 1 && bitLoad[5] == 1)  //int_wm
                consumoTot -= consumiLoad[5];
        }

        //STAMPA INT
        printf("%i%i%i-", bitInt[0], bitInt[1], bitInt[2]);

        //STAMPA FASCIA DI CONSUMO
        if(consumoTot == 0)
            printf("00");
        if(consumoTot > 0 && consumoTot <= 150)
            printf("F1");
        if(consumoTot > 150 && consumoTot <= 300)
            printf("F2");
        if(consumoTot > 300 && consumoTot <= 450)
            printf("F3");
        if(consumoTot > 450)
            printf("OL");

        printf("\nconsumo tot: %i\n", consumoTot);

        printf("\nVuoi andare avanti con la simulazione? Si = 0; No = 1 ");
        scanf("%i", &avanti);
    }
    while(avanti == 0);

    return 0;
}


