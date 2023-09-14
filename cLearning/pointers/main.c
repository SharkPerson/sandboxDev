#include <stdio.h>
#include <stdlib.h>

typedef enum {FALSE, TRUE} BOOL;

void pointerQuestion1() {
    const char* outputString = "";
    outputString = "Pointer: Show the basic declaration of a pointer :\n\
----------------------------------------------------------\n\
Here m = 10, n and o are two integer variable and *z is an integer\n";
    printf("%s", outputString);
    int m = 10;
    int n = 20;
    int o = 30;
    int *z = &m;
    printf("z stores the address of m = 0x%u\n", z);
    printf("*z stores the value of m = %u\n", *z); 
    printf("&m stores the address of m = 0x%u\n", &m); 
    printf("&n stores the address of n = 0x%u\n", &n); 
    printf("&o stores the address of o = 0x%u\n", &o); 
    printf("&z stores the address of z = 0x%u\n", &z); 
}

void pointerQuestion4() {
    // Cannot just create a random pointer, it has to be allocated in memory.
    int *numOne = malloc(sizeof(int));
    int *numTwo = malloc(sizeof(int));

    *numOne = 5;
    *numTwo = 6;

    int sum = *numOne + *numTwo;
    printf("The sum is: %d\n", sum); 
    free(numOne);
    free(numTwo);
}

int q5AddTwoNumbers(int *n1, int *n2) {
    return *n1 + *n2;    
}

void pointerQuestion5() {
    int numOne = 5;
    int numTwo = 6;

    int sum = q5AddTwoNumbers(&numOne, &numTwo);
    printf("The sum is: %d\n", sum); 
}

BOOL isLetter(const char inputChar) {
    // Check if the input char is in range of the ASCII characters for letters.
    // A = 65, Z = 90, a = 97, z = 122
    int c = (int)inputChar;
    if ((c < 65 || c > 90) && (c < 97 || c > 122)) {
        return FALSE;
    }
    return TRUE;
}

BOOL isVowel(const char c) {
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
        return TRUE;
    }
    return FALSE;
}

void pointerQuestion13(const char* inputString) {
    int vowelCount = 0;
    int consonantCount = 0;
    for(int i = 0; i < sizeof(inputString) - 3; i++) {
        //printf("%c\n", inputString[i]);
        if (isLetter(inputString[i]) == FALSE){
            return; 
        }
        if (isVowel(inputString[i]) == TRUE) {
            vowelCount++;
        }
        else {
            consonantCount++;
        }
    }
    printf("Vowel count = %d\n", vowelCount);
    printf("Consonant count = %d\n", consonantCount);
}

int main(void) {
    //pointerQuestion1();
    pointerQuestion4();
    pointerQuestion5();
    const char* newString = "Hello";
    pointerQuestion13(newString);
    return 0;
}


