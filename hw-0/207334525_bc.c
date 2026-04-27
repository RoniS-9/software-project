#include <stdio.h>

int main(void) {
    int source, target;
    int decimal = 0;
    char c;

    printf("enter the source base:\n");
    scanf("%d", &source);
    if (source < 2 || source > 16) {
        printf("Invalid source base!");
        return 0;
    }

    printf("enter the target base:\n");
    scanf("%d", &target);
    if (target < 2 || target > 16) {
        printf("Invalid target base!");
        return 0;
    }

    printf("enter a number in base %d:\n", source);

    c = getchar();
    while (c == '\n' || c == ' ') {
        c = getchar();
    }

    int temp;
    /* conversion to decimal */
    while (c != '\n') {
        if (c >= '0' && c <= '9') {
            temp = c - '0';
        }
        else {
            temp = c - 'a' + 10;
        }
        if (temp < 0 || temp >= source) {
            printf("Invalid input number!");
            return 0;
        }
        decimal = decimal * source + temp;
        c = getchar();
    }

    /* find highest power of target base */
    int power = 1;
    while (decimal / power >= target) {
        power *= target;
    }

    /* extract digits from MSB to LSB */
    while (power > 0) {
        int digit = decimal / power;
        decimal = decimal % power;
        power = power / target;

        if (digit < 10)
            printf("%c", digit + '0');
        else
            printf("%c", digit - 10 + 'a');
    }


    /* convert to target base */
    // int reversed = 0;
    // while (decimal != 0) {
    //     reversed = (reversed * 10) + (decimal % 10);
    //     decimal = decimal / 10;
    // }

    // /* convert to target base */
    // int remainder = 0;
    // while (reversed != 0) {
    //     remainder = reversed % target;
    //     reversed = reversed / target;
    //     if (remainder >= 10) {
    //         remainder += 87;
    //     }
    //     else {
    //         remainder += 48;
    //     }
    //     printf("%c", remainder);
    // }
    printf("\n");
}