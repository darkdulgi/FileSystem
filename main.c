#include <stdio.h>

#include "disk.h"
#include "fs.h"

int main() {
    int blknum, inodenum;
    Inode inode, *pInode;
    FileSysInit();
    inodenum = GetFreeInodeNum();

    

    return 0;
}
