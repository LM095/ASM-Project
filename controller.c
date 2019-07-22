#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>

/* Inserite eventuali extern modules qui */

/* ************************************* */

enum { MAXLINES = 400 };
enum { LIN_LEN = 15 };
enum { LOUT_LEN = 8 };

long long current_timestamp() {
    struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	/* te.tv_nsec nanoseconds divide by 1000 to get microseconds*/
	long long nanoseconds = tp.tv_sec*1000LL + tp.tv_nsec; // caculate nanoseconds
    return nanoseconds;
}


int main(int argc, char *argv[]) {
    int i = 0;
    char bufferin[MAXLINES*LIN_LEN+1] ;
    char line[1024];
    long long tic_c, toc_c, tic_asm, toc_asm;

    char bufferout_c[MAXLINES*LOUT_LEN+1] = "" ;
    char bufferout_asm[MAXLINES*LOUT_LEN+1] = "" ;

    FILE *inputFile = fopen(argv[1], "r");

    if(argc != 3)
    {
        fprintf(stderr, "Syntax ./test <input_file> <output_file>\n");
        exit(1);
    }

    if (inputFile == 0)
    {
        fprintf(stderr, "failed to open the input file. Syntax ./test <input_file> <output_file>\n");
        exit(1);
    }

    while (i < MAXLINES && fgets(line, sizeof(line), inputFile))
    {
        i = i + 1;
        strcat( bufferin, line) ;
    }

    bufferin[MAXLINES*LIN_LEN] = '\0' ;

    fclose(inputFile);


    /* ELABORAZIONE in C */
    tic_c = current_timestamp();

    /* è possibile inserire qui il codice per l'elaborazione in C (non richiesto per l'elaborato) */
    /* questo pezzo di codice è solo una base di partenza per fare dei check sui dati */
    /*
    int c = 0;
    char tmpout[LOUT_LEN];
    strcpy( tmpout, "000-00\n");
    while ( bufferin[c] != '\0') {
      printf( "%d\n", bufferin[c]);
      strcat( bufferout_asm, tmpout);
      c = c + LIN_LEN ;
    }
    */

    toc_c = current_timestamp();

  	long long c_time_in_nanos = toc_c - tic_c;

    /* FINE ELABORAZIONE C */


    /* INIZIO ELABORAZIONE ASM */

    tic_asm = current_timestamp();

    /* Assembly inline:

    Inserite qui il vostro blocco di codice assembly inline o richiamo a funzioni assembly.
    Il blocco di codice prende come input 'bufferin' e deve restituire una variabile stringa 'bufferout_asm' che verrà poi salvata su file. */

    /*
        RIEPILOGO REGISTRI:
            esi = contiene array di input
            edi = contiene array di output
    */

    __asm__ (
            ".section .bss;"
                "int_gen: .byte 0;"
                "int_dw: .byte 0;"
                "int_wm: .byte 0;"
                "res_gen: .byte 0;"
                "res_dw: .byte 0;"
                "res_wm: .byte 0;"
                "flagTurnOn: .byte 0;"
                "flagTurnOffDW: .byte 0;"
                "flagTurnOffWM: .byte 0;"
                "consumoTot: .long 0;"
                "ctOL: .byte 0;"
                "flagFineStringa: .byte 0;"

            ".section .data;"
                "consumiLoad: .long 200,30,120,100,200,180,24,40,20,40;"
                "bitLoad: .fill 10, 1, 0;"           //byte

            ".section .text;"
            ".global controllerASM;"

            "controllerASM:;"

                //Azzeramenti registri
                "xorl %%eax, %%eax;"
                "xorl %%ebx, %%ebx;"
                "xorl %%ecx, %%ecx;"
                "xorl %%edx, %%edx;"

                "inizioWhileFineStringa:;"
                    "movl $0, flagTurnOffDW;"     /*azzero flagTurnOffDW*/
                    "movl $0, flagTurnOffWM;"     /*azzero flagTurnOffWM*/

                    "cmpb $1, flagFineStringa;"
                    "je fine;"                    /*se flagFineStringa = 1 allora usciamo dal ciclo*/

                    //verificaTrattino

                    /*entro nel ciclo while*/
                    "cmpb $45, (%%esi);"        /*45 è il trattino - */
                    "jne inizioCalcoloRes;"     /*andiamo a riga 48*/

                    /*se è il trattino*/
                    "inc %%esi;"
                    "xorl %%ecx, %%ecx;"        /*azzero registro contatore ctLoad*/
                    "jmp inizioWhileACapo;"

                    /*------------------- DETERMINAZIONE RES ------------------- */

                    "inizioCalcoloRes:;"                /*caso esaminazione primi 3 bit di res riga 48*/
                        "cmpl $0, %%ecx;"               /*ctLoad riga 50*/
                        "jne elseDW;"                   /*se non entro nell'if faccio caso DW con load = 1 riga 58*/

                        /*sono dentro all'if con ctLoad = 0*/
                        "cmpb $48, (%%esi);"            /*se l'elemento della cella è 48 (1)*/
                        "jne res_gen1;"                 /*salto dove devo mettere res_gen = 1*/

                        "movb $0, res_gen;"             /*metto 0 in res_gen riga 53*/
                        "jmp fineCalcoloRes;"           /*usciamo dal calcolo del res se siamo entrati nel primo if riga 71*/

                        /*nel caso in cui il primo bit esaminato sia 1*/
                        "res_gen1:;"
                            "movb $1, res_gen;"         /*metto 1 in res_gen riga 55*/
                            "jmp fineCalcoloRes;"       /*usciamo dal calcolo del res se siamo entrati nel primo if riga 71*/

                        /*sono dentro all'if con ctLoad = 1*/
                        "elseDW:;"                      /*riga 57*/
                            "cmpl $1, %%ecx;"            /*guardo se ctLoad == 1*/
                            "jne elseWM;"               /*se non è uguale allora salto a elseWM riga 64*/

                        /*se è uguale allora */
                            "cmpb $48, (%%esi);"
                            "jne res_dw1;"              /*salto dove devo mettere res_dw = 1*/

                            "movb $0, res_dw;"          /*metto 0 in res_dw riga 59*/
                            "jmp fineCalcoloRes;"

                            "res_dw1:;"
                                "movb $1, res_dw;"
                                "jmp fineCalcoloRes;"

                            /*sono dentro all'if con ctLoad = 2*/
                            "elseWM:;" /*riga 64*/
                                "cmpl $2, %%ecx;"
                                "jne fineCalcoloRes;"

                                /*se è uguale allora */
                                "cmpb $48, (%%esi);"
                                "jne res_wm1;"           /*salto dove devo mettere res_wm = 1*/

                                "movb $0, res_wm;"       /*metto 0 in res_wm riga 67*/
                                "jmp fineCalcoloRes;"

                                "res_wm1:;"
                                    "movb $1, res_wm;"
                                    "jmp fineCalcoloRes;"

                    "fineCalcoloRes:;"
                        "inc %%ecx;" /*incremento ct riga 72*/
                        "inc %%esi;"
                        "jmp inizioWhileFineStringa;"

                    /* ------------ DETERMINAZIONE LOAD ------------*/

                    "inizioWhileACapo:;"     //jump qui dopo aver verificato di aver trovato trattino
                        "cmpb $0, (%%esi);"
                        "je fineWhileACapo;"
                                              
                        "cmpb $10, (%%esi);"    /*10 è \n in ascii*/
                        "je fineWhileACapo;"

                        /*se sono diversi allora: riga 24*/
                        "cmpb $48, (%%esi);"             /*48 è 0 in ascii*/
                        "jne elseIfTrattino;"            /*riga 26*/
                        "movb $0, bitLoad(%%ecx);"       /*se è = 0*/
                        "jmp incrementiWhileACapo;"

                        "elseIfTrattino:"
                          "movb $1, bitLoad(%%ecx);"

                        "incrementiWhileACapo:;"
                        "inc %%ecx;"        /*incremento ctLoad riga 29*/
                        "inc %%esi;"        /*incremento ct vettore riga 30*/
                        "jmp inizioWhileACapo;"

                    "fineWhileACapo:;"
                        "inc %%esi;"            /*incremento ct riga 46*/
                        "xorl %%ecx, %%ecx;"    /*azzero ctLoad riga 47*/

                    /*-------------------INIZIO ELABORAZIONE------------------- */

                        /* da riga 66 a 72 - CASO ACCENSIONE MACCHINA (RES_GEN) */
                        "cmpb $1, res_gen;"
                        "jne fineAccMacch;"         /* se res_gen non è = 1 allora vado alla fine dell'if */

                        //accensione macchina
                        "cmpb $0, flagTurnOn;"
                        "jne fineAccMacch;"         /* se flagTurnOn non è = 1 allora vado alla fine dell'if */

                        "movb $1, flagTurnOn;"
                        "movb $1, int_gen;"
                        "movb $1, int_dw;"
                        "movb $1, int_wm;"

                        "fineAccMacch:;"

                        /* da riga 74 a 76 - CASO RIACCENSIONE DI RES_DW */
                        "cmpb $1, res_dw;"
                        "jne fineRiaccResDw;"

                        "movb $1, int_dw;"    /* ramo if */
                        "fineRiaccResDw:;"    /* ramo else */

                        /* da riga 78 a 80 - CASO RIACCENSIONE DI RES_WM */
                        "cmpb $1, res_wm;"
                        "jne fineRiaccResWm;"

                        "movb $1, int_wm;"    /* ramo if */
                        "fineRiaccResWm:;"     /* ramo else */

                        /* riga 82 CASO MACCHINA SPENTA */

                        "cmpb $0, int_gen;"
                        "jne macchCorrAccesa;" /* si salta a riga 91, se int_gen non è uguale a 0, allora macchina correttamente accesa */

                        "movl $0, consumoTot;"
                        "movb $0, int_gen;"
                        "movb $0, int_dw;"
                        "movb $0, int_wm;"
                        "jmp vettoreFinale;"

                        "macchCorrAccesa:;" /* else con il ciclo for calcola consumi riga 93 */

                        "movl $10, %%ebx;" /* indice dove mi devo fermare */
                        "movl $0, %%ecx;" /* mettiamo 0 nell'indice, i=0 del ciclo for */

                        /*-------CICLO FOR PER CALCOLO CONSUMO-------*/
                        "movl $0, consumoTot;"              /*azzeramento consumo Totale*/
                        "xorl %%eax, %%eax;"

                        "inizioConteggioFor:;"  /*riga 93*/
                            "cmpl %%ebx, %%ecx;"
                            "je fineConteggioFor;"
                            "movb bitLoad(%%ecx), %%al;"  /*metto il bitLoad in eax per la moltiplicazione*/

                            "cmp $4, %%ecx;"
                            "jne indiceCinque;"

                            /* moltiplicazioni*/
                            "mulb consumiLoad(, %%ecx, 4);"
                            "mulb int_dw;"
                            "jmp incrementiFor;"

                            "indiceCinque:;"
                                "cmp $5, %%ecx;"
                                "jne else;"
                                /* moltiplicazioni*/
                                "mulb consumiLoad(, %%ecx, 4);"
                                "mulb int_wm;"
                                "jmp incrementiFor;"

                                "else:;"
                                    /*moltiplicazioni*/
                                    "mulb consumiLoad(, %%ecx, 4);"  //si può fare?

                                "incrementiFor:;"
                                    "addw %%ax, consumoTot;"   /*sommiamo consumoTot con la moltiplicazione*/
                                    "inc %%ecx;"
                                    "jmp inizioConteggioFor;"

                        "fineConteggioFor:;"        // ebx, ecx = 10
                        "xorl %%ecx, %%ecx;"        /*azzeramento ecx*/
                        "xorl %%ebx, %%ebx;"        /*azzeramento ebx*/
                        "xorl %%eax, %%eax;"        /*azzeramento eax*/

                        /*------- CONTEGGIO OL ------*/

                        "cmpl $450, consumoTot;"    /* ovvero consumoTot - 450, devo usare cmpl perchè 'l' indica di tenere i segni delle operazioni*/
                        "jle fineConteggioOl;"      /* ATTENZIONE CAMBIATO IN JLE PRIMA ERA JGE - per saltare, consumoTot deve essere <= di 450, quindi 450 >= consumoTot*/
                        "addb $1, ctOL;"            /* usiamo ecx come conteggio cicli OL */

                        "cmpb $4, ctOL;"
                        "jne conteggioOlWM;"
                        "movb $0, int_dw;"
                        "movb $1, flagTurnOffDW;"
                        "jmp sistemazioneConsumo;"

                        "conteggioOlWM:;"
                        "cmpb $5, ctOL;"
                        "jne conteggioOlGen;"
                        "movb $0, int_wm;"
                        "movb $1, flagTurnOffWM;"
                        "jmp sistemazioneConsumo;"

                        "conteggioOlGen:;"
                        "cmpb $6, ctOL;"
                        "jne sistemazioneConsumo;"
                        "movb $0, int_gen;"
                        "movb $0, flagTurnOn;"
                        "movl $0, consumoTot;"
                        "jmp sistemazioneConsumo;"

                        "fineConteggioOl:;"
                            "movb $0, ctOL;"        /* se non siamo in OL allora azzero ctOL */

                        /*------- SISTEMAZIONE CONSUMO TOT -------*/
                        /* righe 128 - 132 */

                        "sistemazioneConsumo:;"
                            /*controllo DW*/
                            "cmpb $0, int_dw;"
                            "jne controlloWM;"

                            "cmpb $1, flagTurnOffDW;"
                            "jne controlloWM;"

                            "xorl %%ebx, %%ebx;"
                            "movl $4, %%ebx;"
                            "cmpb $1, bitLoad(%%ebx);"
                            "jne controlloWM;"

                            "movl consumiLoad(, %%ebx, 4), %%ecx;"   /*usiamo ecx come supporto*/
                            "subl %%ecx, consumoTot;"

                            /*controllo WM*/
                            "controlloWM:;"
                            "cmpb $0, int_wm;"
                            "jne vettoreFinale;"

                            "cmpb $1, flagTurnOffWM;"
                            "jne vettoreFinale;"

                            "xorl %%ebx, %%ebx;"
                            "movl $5, %%ebx;"
                            "cmpb $1, bitLoad(%%ebx);"
                            "jne vettoreFinale;"

                            "movl consumiLoad(, %%ebx, 4), %%ecx;"   /*usiamo ecx come supporto*/
                            "subl %%ecx, consumoTot;"

                        /*------------ INSERIMENTO IN ARRAY OUTPUT -----------*/
                        "vettoreFinale:;"
                            "xorl %%ecx, %%ecx;"      /*azzero ecx per sicurezza*/

                            "movb int_gen, %%cl;"     /*uso ecx come supporto*/
                            "addb $48, %%cl;"         /*aggiungo 48 per uniformare ad ASCII*/
                            "movb %%cl, (%%edi);"
                            "inc %%edi;"

                            "movb int_dw, %%cl;"
                            "addb $48, %%cl;"          /*aggiungo 48 per uniformare ad ASCII*/
                            "movb %%cl, (%%edi);"
                            "inc %%edi;"

                            "movb int_wm, %%cl;"
                            "addb $48, %%cl;"          /*aggiungo 48 per uniformare ad ASCII*/
                            "movb %%cl, (%%edi);"
                            "inc %%edi;"

                            "movb $45, (%%edi);"       /*metto - (trattino)*/
                            "inc %%edi;"

                            /*FASCIA 00*/
                            "cmpl $0, consumoTot;"
                            "je vettore00;"

                            /*FASCIA F1*/
                            "cmpl $150, consumoTot;"
                            "jle vettoreF1;"

                            /*FASCIA F2*/
                            "cmpl $300, consumoTot;"
                            "jle vettoreF2;"

                            /*FASCIA F3*/
                            "cmpl $450, consumoTot;"
                            "jle vettoreF3;"

                            /*FASCIA OL*/
                            "jmp vettoreOL;"

                            "vettore00:;"
                                "movb $48, (%%edi);"    /*metto 0*/
                                "inc %%edi;"
                                "movb $48, (%%edi);"    /*metto 0*/
                                "inc %%edi;"
                                "jmp aggiuntaACapo;"

                            "vettoreF1:;"
                                "movb $70, (%%edi);"    /*metto F*/
                                "inc %%edi;"
                                "movb $49, (%%edi);"    /*metto 1*/
                                "inc %%edi;"
                                "jmp aggiuntaACapo;"

                            "vettoreF2:;"
                                "movb $70, (%%edi);"    /*metto F*/
                                "inc %%edi;"
                                "movb $50, (%%edi);"    /*metto 2*/
                                "inc %%edi;"
                                "jmp aggiuntaACapo;"

                            "vettoreF3:;"
                                "movb $70, (%%edi);"    /*metto F*/
                                "inc %%edi;"
                                "movb $51, (%%edi);"    /*metto 3*/
                                "inc %%edi;"
                                "jmp aggiuntaACapo;"

                            "vettoreOL:;"
                                "movb $79, (%%edi);"    /*metto O*/
                                "inc %%edi;"
                                "movb $76, (%%edi);"    /*metto L*/
                                "inc %%edi;"
                                "jmp aggiuntaACapo;"

                            "aggiuntaACapo:;"
                                "xorl %%ecx, %%ecx;"
                                "movb $10, (%%edi);"
                                "inc %%edi;"
                                "cmpb $0, (%%esi);"
                                "je cambioFlag;"
                                "jmp inizioWhileFineStringa;"

                                "cambioFlag:;"
                                "movb $1, flagFineStringa;"
                                
                                "jmp inizioWhileFineStringa;"

                "fine:;"
                "movb $0, (%%edi);"    /*mette \0 */

                :                                        /*output*/
                : "S"(bufferin), "D"(bufferout_asm)      /*input*/
                : "%eax", "%ebx", "%ecx", "%edx"         /*reg modificati*/
            );

    toc_asm = current_timestamp();

  	long long asm_time_in_nanos = toc_asm - tic_asm;

    /* FINE ELABORAZIONE ASM */


    printf("C time elapsed: %lld ns\n", c_time_in_nanos);
    printf("ASM time elapsed: %lld ns\n", asm_time_in_nanos);

    /* Salvataggio dei risultati ASM */
  	FILE *outputFile;
    outputFile = fopen (argv[2], "w");
    fprintf (outputFile, "%s", bufferout_asm);
    fclose (outputFile);

    return 0;
}
