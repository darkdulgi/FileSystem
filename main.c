#include <stdio.h>

#include "disk.h"
#include "fs.h"

int main() {
    int blknum, inodenum;
    Inode inode, *pInode;
    FileSysInit();
    inodenum = GetFreeInodeNum();

    SetInodeBytemap(inodenum);
    blknum = GetFreeBlockNum();
    SetBlockBytemap(blknum);
    inode.size = 0;
    inode.allocBlocks = 0;

    //...

    PutInode(inodenum, &inode);
    pInode = malloc(sizeof(Inode));
    GetInode(inodenum, pInode);
    printf("Complete\n");
    return 0;
}
