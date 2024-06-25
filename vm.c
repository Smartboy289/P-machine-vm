//Eric Camilo
//COP 3402 0001
#include <stdio.h>

int base(int *pas, int BP, int L);

//Struct to represent the instruction line vm reads[OP|L|M]
typedef struct instruction{
    int op;
    int L;
    int M;
}instruction;

//argc to count input, argv as pointer to arguments in input
int main(int argc, char **argv){
    int bp, sp, pc;
    const int ARRAY_SIZE = 500;

    //opens file inputted in read mode
    FILE *file = fopen(argv[1],"r");
    int pas[ARRAY_SIZE];
    int index=0;

    //make pas default values all zero
    for(int i=0;i<ARRAY_SIZE;i++){
        pas[i] = 0;
    }
    //loop going through file and adding each num to pas
    while( !feof(file))
    {
        fscanf(file, "%d %d %d",&pas[index],&pas[index+1],&pas[index+2]);
        index+=3;
    }

    //set bp to next available spot in stack after text
    bp = index;
    sp = bp-1;
    pc=0;

    //setting up table for the output
    printf("\t\t\tPC\tBP\tSP\tstack\nInitial values:\t%d\t%d\t%d\n\n",pc,bp,sp);
    instruction IR;
    int HALT=1;

    while(HALT)
    {
        IR.op = pas[pc];
        IR.L= pas[pc+1];
        IR.M = pas[pc+2];
        pc = pc+3;
        switch(IR.op)
        {
            //instruction for LIT
            case 1:
                sp = sp + 1;
                pas[sp] = IR.M;
                printf("\tLIT");
                break;

            //instructions for OP depending on M
            case 2:
                switch (IR.M)
                {
                    //RTN
                    case 0:
                        sp = bp-1;
                        bp = pas[sp + 2];
                        pc = pas[sp+3];
                        printf("\tRTN");
                        break;
                    //ADD
                    case 1:
                        pas[sp-1] = pas[sp-1] + pas[sp];
                        sp = sp-1;
                        printf("\tADD");
                        break;
                    //SUB
                    case 2:
                        pas[sp-1] = pas[sp-1] - pas[sp];
                        sp = sp-1;
                        printf("\tSUB");
                        break;
                    //MUL
                    case 3:
                        pas[sp-1] = pas[sp-1] * pas[sp];
                        sp = sp-1;
                        printf("\tMUL");
                        break;
                    //DIV
                    case 4:
                        pas[sp-1] = pas[sp-1] / pas[sp];
                        sp = sp-1;
                        printf("\tDIV");
                        break;
                    //EQL
                    case 5:
                        pas[sp-1] = pas[sp-1] == pas[sp];
                        sp = sp-1;
                        printf("\tEQL");
                        break;
                    //NEQ
                    case 6:
                        pas[sp-1] = pas[sp-1] != pas[sp];
                        sp = sp-1;
                        printf("\tNEQ");
                        break;
                    //LSS
                    case 7:
                        pas[sp-1] = pas[sp-1] < pas[sp];
                        sp = sp-1;
                        printf("\tLSS");
                        break;
                    //LEQ
                    case 8:
                        pas[sp-1] = pas[sp-1] <= pas[sp];
                        sp = sp-1;
                        printf("\tLEQ");
                        break;
                    //GTR
                    case 9:
                        pas[sp-1] = pas[sp-1] > pas[sp];
                        sp = sp-1;
                        printf("\tGTR");
                        break;
                    //GEQ
                    case 10:
                        pas[sp-1] = pas[sp-1] >= pas[sp];
                        sp = sp-1;
                        printf("\tGEQ");
                        break;
                    default:
                        break;
                }
                break;
            
            //LOD
            case 3:
                sp = sp+1;
                pas[sp] = pas[base(pas,bp,IR.L)+IR.M];
                printf("\tLOD");
                break;
            //instruction for STO
            case 4:
                pas[base(pas,bp,IR.L)+IR.M] = pas[sp];
                sp = sp - 1;
                printf("\tSTO");
                break;
            //instruction for CAL
            case 5:
                pas[sp+1] = base(pas,bp, IR.L);
                pas[sp+2] = bp;
                pas[sp+3] = pc;
                bp = sp  + 1;
                pc = IR.M;
                printf("\tCAL");
                break;
            //instruction for INC
            case 6:
                sp = sp + IR.M;
                printf("\tINC");
                break; 
            //instruction for JMP           
            case 7:
                pc = IR.M;
                printf("\tJMP");
                break;
            //instruction for JPC
            case 8:
                if (pas[sp] == 0)
                {
                    pc = IR.M;
                }
                sp = sp-1;
                printf("\tJPC");
                break;
            //instruction for WRT, RED, or HLT
            case 9:
                //WRT
                if(IR.M==1)
                {
                    printf("Output result is: %d\n",pas[sp]);
                    sp = sp-1;
                    printf("\tWRT");
                }
                //RED
                else if(IR.M==2)
                {
                    sp = sp + 1;
                    printf("Please Enter an Integer: ");
                    scanf("%d",&pas[sp]);
                    printf("\tRED");
                }
                //HLT
                else if(IR.M==3)
                {
                    HALT=0;
                    printf("\tHLT");
                }
                    
                break;
            default:
                break;
        }
        //prints L code, M, the PC, BP, and SP
        printf(" %d\t%d\t%d\t%d\t%d\t",IR.L,IR.M,pc,bp,sp);

        //prints out the state of the stack to terminal
        for(int i = index; i<=sp;i++)
        {
            if(i!= index && (bp == i))
                printf("| ");
            printf("%d ",pas[i]);
        }
        printf("\n");
    }
}


/**********************************************/
/* Find base L levels down  */
/*  */
/**********************************************/
 
int base(int *pas, int BP, int L)
{
    int arb = BP; // arb = activation record base
    while ( L > 0)     //find base L levels down
    {
        arb = pas[arb];
        L--;
    }
    return arb;
}