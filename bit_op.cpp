#include <cstdlib>
#include <stdio.h>

void conv(int n)
{
    char buffer [50];
    itoa(n,buffer,10);
    printf("\n\nchar: %c\n",n + 97);
    printf("decimal: %s\n",buffer);
    itoa(n,buffer,16);
    printf("hexadecimal: %s\n",buffer);
    itoa(n,buffer,2);
    printf("binary: %s\n",buffer);
    int m = 1 << n;
    itoa(m,buffer,2);
    printf("1 << %d: %s (%d)\n",n,buffer,m);
}

void masking(int ch, char* rem)
{
    char buffer3 [50];
    itoa(ch,buffer3,2);
    printf("%s --> %c --> %d %s\n", buffer3, ch, ch, rem);
}

int main()
{
    char ch = 'a';
    char c = ch - 'a';
    int r = 1 << c;
    
    conv((int)ch);
    conv(r);

    char alp[] = {'a', 'm', 'a', 'n', 'k'};

    for (char t : alp)
    {
        conv((int)(t - 'a'));
    }

    printf("\n\n");
    char buffer2 [50];
    int mask = 0;
    for (int i = 0; i < 5; i++)
    {
        itoa(i,buffer2,2);
        printf("binary: %s\n",buffer2);
        mask |= 1 << (alp[i] - 'a');
        itoa(mask,buffer2,2);
        printf("1 << %d: %s (%d)\n\n", i, buffer2, mask);
    }

    printf("\n\n");
    int num1 = 'w' - 'a';
    int num2 = 'x' - 'a';
    masking('w' - 'a', ", num");
    masking('x' - 'a', ", num");    
    masking(num1 | num2, ", OR");
    masking(num1 & num2, ", AND");
    masking(1 << num1, ",num1");
    masking(1 << num2, ",num2");
    mask = 0;
    masking(mask, ",mask");
    mask |= 1 << num1;
    masking(mask, ", << num1");
    mask |= 1 << num2;
    masking(mask, ", << num2");

    return EXIT_SUCCESS;
}