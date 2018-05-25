
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_DEPTH 10

typedef struct {
    int idx_start;
    int idx_finish;
    uint64_t *vec_ptr;
    int thresh_depth;
} vec_data;

char * tokeniz(char *str,uint64_t *vec, int *numval){
    
    *numval = 0;
    char *token;
    
    token = strtok(str,", ");
    
    while( token != NULL ) {
        sscanf(token,"%lld",&vec[*numval]);
        (*numval)++;
        token = strtok(NULL, ", ");
    }
    
    return token;
}

void quick_sort(void *data){
    
    vec_data * thread_data = (vec_data *) data;
    uint64_t * a = thread_data->vec_ptr;
    int left = thread_data->idx_start;
    int right = thread_data->idx_finish;
    
    int i, j;
    uint64_t y, x;
    
    i = left;
    j = right;
    x = a[(left + right) / 2];
    
    while(i <= j) {
        while(a[i] < x && i < right) {
            i++;
        }
        while(a[j] > x && j > left) {
            j--;
        }
        if(i <= j) {
            y = a[i];
            a[i] = a[j];
            a[j] = y;
            i++;
            j--;
        }
    }
    
    pthread_t job_data;
    
    if(j > left) {
        vec_data ta;
        ta.idx_start = left;
        ta.idx_finish = j;
        ta.vec_ptr = a;
        ta.thresh_depth = thread_data->thresh_depth+1;
        if (thread_data->thresh_depth < MAX_DEPTH) {
            pthread_create(&job_data, NULL, quick_sort, &ta);
        }
        else {
            quick_sort(&ta);
        }
    }
    if(i < right) {
        vec_data ta;
        ta.idx_start = i;
        ta.idx_finish = right;
        ta.vec_ptr = a;
        ta.thresh_depth = thread_data->thresh_depth+1;
        quick_sort(&ta);
    }
    if (thread_data->thresh_depth < MAX_DEPTH){
        pthread_join(job_data, NULL);
    }
}

int main() {
    
    int i;
    uint64_t val[1000000]; /*vetor de numeros a ser analisado */
    int num_val; /*numero de valores lidos do arquivo*/
    /* index do processo */
    
    char *s=malloc(sizeof(char)*1000000);
    
    fgets (s, 1000000, stdin);
    s[strlen(s)-1] = '\0';
    tokeniz(s,val,&num_val);
    
    //    for (i=0; i<num_val; i++) {
    //        printf("%lld ", val[i]);
    //    }
    
    vec_data ta;
    ta.idx_start = 0;
    ta.idx_finish = num_val-1;
    ta.vec_ptr = val;
    ta.thresh_depth = 0;
    quick_sort(&ta);
    
    //   printf("\n%d", num_val);
    //   printf("\n");
    
    for (i=0; i<num_val; i++) {
        printf("%lld", val[i]);
        if (i==(num_val-1)) {
            break;
        }
        printf(" ");
    }
    printf("\n");
    
    return 0;
}
