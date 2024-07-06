#include <stdio.h>

#define MAXSIZE 1024
int n = 10;
int H[MAXSIZE] = {NULL,1,2,3,5,4,8,9,7,6,10};

void push_heap(int H[], int x){
    int i, j;
    if(++n < MAXSIZE){
        H[n] = x; i = n; j = i/2;
        while(j > 0 && x < H[j]){
            H[i] = H[j]; i = j; j = i/2;
        }
        H[i] = x;
    }
}

void delete_min(int H[]){
    int i, j, t;
    if(n >= 1){
        H[1] = H[n--]; i = 1;
        while((j = i*2) <= n){
            if(j < n && H[j] > H[j+1]) j++;
            if(H[i] <= H[j]) break;
            else{
                t = H[i]; H[i] = H[j]; H[j] = t;
            }
            i = j;
        }
    }
}

int main(){
    int h[] = {9,8,7,6,5,4,3,2,1};

    // for(int i = 0; i < 9; i++){
    //     push_heap(H, h[i]);
    // }
    
    delete_min(H);
    delete_min(H);

    for(int j = 0; j < 12; j++){
        printf("%d:%d, ", j, H[j]);
    }

    push_heap(H, 2);

    for(int j = 0; j < 12; j++){
        printf("%d:%d, ", j, H[j]);
    }
    printf("\n");
}