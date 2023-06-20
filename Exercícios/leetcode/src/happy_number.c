#include "happy_number.h"

int sum_digits(int n) {
    int sum = 0;
    while (n > 0) {
        sum += (n % 10) * (n % 10);
        n /= 10;
    }
    return sum;
}

bool isHappy(int n){

    int sum = sum_digits(n);
    int comp = sum;
    short int count = 0;

    while (sum != 1 && count < 10000) {
        int aux = sum_digits(sum);
        if (aux == comp) return 0;
        sum = aux;
        count++;
    }

    return (sum == 1);
}