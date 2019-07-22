#include <stdio.h>

#define LUNG 40

int main(void)
{
    int ct = 0;
    int ctLoad = 0;
    char a[LUNG] = {'0', '0', '1', '-', '0', '1', '0', '1', '0', '0', '0', '1', '1', '0', '\n', '0', '0', '1', '-', '0', '1', '0', '1', '0', '0', '0', '1', '1', '0', '\n', '\0'};
    int bitLoad[10] = {0};
    int res_gen = 0;
    int res_dw = 0;
    int res_wm = 0;

    while(a[ct] != '\0') //inizioWhileFineStringa
    {
        if(a[ct] == '-')
        {
            ct++;
            ctLoad = 0;

            while(a[ct] != '\n') //inizioWhileACapo
            {
                if(a[ct] == '0')
                    bitLoad[ctLoad] = 0;
                else    //elseIfTrattino
                    bitLoad[ctLoad] = 1;
                
                //incrementiWhileACapo
                ctLoad++;
                ct++;
            } //fineWhileACapo
            
            //elaborazione!

            printf("gen: %d, dw: %d, wm: %d\n", res_gen, res_dw, res_wm);
            printf("Load: ");

            for(int i = 0; i < 10; i++)
            {
                printf("%d ", bitLoad[i]);
            }

            printf("\n");

            ct++;
            ctLoad = 0;
        }
        else //inizioCalcoloRes
        {
            if(ctLoad == 0)
            {
                if(a[ctLoad] == '0')
                    res_gen = 0;
                else    
                    res_gen = 1;
            }            
            else if(ctLoad == 1) //elseDW
            {
                if(a[ctLoad] == '0')
                    res_dw = 0;
                else    
                    res_dw = 1;
            }
            else if(ctLoad == 2) //elseWM
            {               
                if(a[ctLoad] == '0')
                    res_wm = 0;
                else    
                    res_wm = 1;
            }

            ctLoad++;
        }

    
        ct++;      
    }

    return 0;   //fine
}