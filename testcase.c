#include <stdio.h>
#include "disk.h"
#include "hw1.h"
#include "hw2.h"
#include <string.h>

int main(void)
{
    CreateFileSystem();

    char name[30] = "/tmp";
    MakeDirectory(name);
    return 0;
}
