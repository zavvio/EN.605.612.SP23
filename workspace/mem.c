#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define num_of_array 100
#define array_size_crash 133427199 // malloc with this size yields seg-fault
#define array_size_100MB 104857600
#define array_size_small 10

int global_init_var = 12345;
int global_var;

int funcA(int i);
void getmem();

int main(int argc, char *argv[])
{
    static int static_main_init_var = 13579;
    static int static_main_var;
    int main_var, i;
    char *arrays[num_of_array];
    printf("\n>>>>> Part II.a - memory addresses usage. <<<<<\n");

    printf("[0x%08X] - (stack) main_var\n", (unsigned int) &main_var);
    printf("[0x%08X] - (stack) arrays\n", (unsigned int) &arrays[0]);
    funcA(0);

    arrays[0] = (char *)malloc(array_size_small);
    memset(arrays[0], 0, array_size_small);
    printf("[0x%08X] - (heap) *arrays\n", (unsigned int) arrays[0]);
    free(arrays[0]);

    printf("[0x%08X] - (bss) global_var\n", (unsigned int) &global_var);
    funcA(2);
    printf("[0x%08X] - (bss) static_main_var\n", (unsigned int) &static_main_var);

    funcA(1);
    printf("[0x%08X] - (data) static_main_init_var\n", (unsigned int) &static_main_init_var);
    printf("[0x%08X] - (data) global_init_var\n", (unsigned int) &global_init_var);

    printf("[0x%08X] - (text) getmem\n", (unsigned int) getmem);
    printf("[0x%08X] - (text) funcA\n", (unsigned int) funcA);
    printf("[0x%08X] - (text) main\n", (unsigned int) main);

    main_var = num_of_array;
    if (argc > 1)
    {
        main_var = atoi (argv[1]);
        if (main_var < 0) main_var = 0;
        if (main_var > num_of_array) main_var = num_of_array;
    }
    memset(arrays, 0, sizeof(arrays));
    printf("\n>>>>> Part II.b - attempt to allocate %d 100MB memory block. <<<<<\n", main_var);
    for (i = 0; i < main_var; i++)
    {
        getmem();
        arrays[i] = (char *)malloc(array_size_100MB);
        memset(arrays[i], 0, array_size_100MB);
        printf("[0x%08X] - allocated 100MB block number %d\n", (unsigned int) arrays[i], i+1);
        sleep(3);
    }
    printf("Enter endless loop, press <q> to exit\n");
    while (getchar() != 'q')
    {
        sleep(10);
    }
    for (i = 0; i < main_var; i++) // unreachable code
    {
        free(arrays[i]);
    }
    return 0;
}

int funcA(int i)
{
    static int static_funcA_init_var = 24680;
    static int static_funcA_var;
    int funcA_var;
    switch(i)
    {
    case 1:
        printf("[0x%08X] - (data) static_funcA_init_var\n", (unsigned int) &static_funcA_init_var);
        break;
    case 2:
        printf("[0x%08X] - (bss) static_funcA_var\n", (unsigned int) &static_funcA_var);
        break;
    default:
        printf("[0x%08X] - (stack) funcA_var\n", (unsigned int) &funcA_var);
        break;
    }
    return 0;
}

void getmem()
{
    unsigned int pagesize, total, free, largest, cached;
    FILE *f = fopen("/proc/meminfo", "r");
    if(!f) return;
    if(fscanf(f, "%u %u %u %u %u", &pagesize, &total, &free, &largest, &cached) != 5) {
        fprintf(stderr, "fscanf of meminfo failed\n");
        exit(1);
    }
    fclose(f);
    printf("Free RAM: %u bytes; %d more 100MB memory block(s) possible.\n", free * pagesize, free * pagesize / array_size_100MB);
}