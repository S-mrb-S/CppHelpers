// main.cpp

#include <stdio.h>
#include <stdlib.h>

typedef int Int;
typedef void fn;

#define null ((fn *)0)
#define MAX_ALLOCATIONS 100

typedef struct
{
    fn *pointers[MAX_ALLOCATIONS];
    Int count;
} MemoryManager;

#define ALLOCATE(manager, type, size) allocate_memory(manager, sizeof(type) * (size))

#define Int(name, size) Int *name = (Int *)ALLOCATE(&manager, Int, size)
#define Double(name, size) double *name = (double *)ALLOCATE(&manager, double, size)

#define FREE_ALL(manager) free_all(manager)

fn *allocate_memory(MemoryManager *manager, size_t size)
{
    if (manager->count >= MAX_ALLOCATIONS)
    {
        fprintf(stderr, "Max allocations reached\n");
        return null;
    }
    fn *ptr = malloc(size);
    if (ptr != null)
    {
        manager->pointers[manager->count++] = ptr;
    }
    return ptr;
}

fn free_all(MemoryManager *manager)
{
    for (Int i = 0; i < manager->count; i++)
    {
        free(manager->pointers[i]);
    }
    manager->count = 0; // Reset count after freeing
}

fn error(const char *msg)
{
    perror(msg);
    exit(1);
}

fn panic(const char *message)
{
    fprintf(stderr, "Panic: %sn", message);
    exit(EXIT_FAILURE); // یا می‌توانید از abort() استفاده کنید
}

Int main()
{
    MemoryManager manager = {.count = 0};

    Int(arr1, 10);
    Int(arr2, 5);
    Double(arr3, 3);

    if (arr1 == null || arr2 == null || arr3 == null)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        FREE_ALL(&manager);
        return EXIT_FAILURE;
    }

    for (Int i = 0; i < 10; i++)
    {
        arr1[i] = i * 10;
        printf("%d ", arr1[i]);
    }
    printf("\n");

    FREE_ALL(&manager);

    return EXIT_SUCCESS;
}
