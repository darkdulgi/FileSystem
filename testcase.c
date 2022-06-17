#include <stdio.h>
#include "disk.h"
#include "hw1.h"
#include "hw2.h"
#include <string.h>

int main(void)
{
    CreateFileSystem();
    printf("test start\n");

    while (1) {
        char name[30];
        scanf("%s",name);

        if (MakeDirectory(name) != 0)
        {
            printf("making directory failed\n");
        }
        else
        {
            printf("Success\n");
        }
    }
    return 0;
}
