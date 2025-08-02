#include <stdio.h> 
#include <conio.h>

/*
3 / fizz
5 / buzz
both 3 & 5 fizzbuzz
else number
*/
int main(){
    int x, y, z;

    while(1){
        x = 0;
        y = 1;
        do{
            printf("%d\n", x);

            z = x + y;
            x = y;
            y = z;
        } while( x < 255);
    }

    return 0;
}


int main2(){
    int i;
    for (i=1;i<=100;i++)
    {
        if((i % 3 == 0) && (i % 5 == 0))
        {
            printf("fizz buzz\n");
        }
        else if (i % 3 == 0)
        {
            printf("fizz\n");
        }
        else if(i % 5 == 0)
        {
            printf("buzz\n");
        }
        else
        {
            printf("%d\n",i);
        }
        
    }
    // getch();
    return 0;
}
