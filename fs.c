#include "fs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disk.h"

void FileSysInit() {
    DevCreateDisk();
    char* ptr = malloc(FS_DISK_CAPACITY);
    for (int i = 0; i <= BLOCK_SIZE * 7; i++) {
        *(ptr + i) = 0;
    }

    for (int blk = 0; blk < FS_DISK_CAPACITY / BLOCK_SIZE; blk++) {
        DevWriteBlock(blk, ptr + BLOCK_SIZE * blk);
    }
    free(ptr);
}

void SetInodeBytemap(int inodeno) {
    DevOpenDisk();
    char* ptr = malloc(BLOCK_SIZE);
    DevReadBlock(1, ptr);
    *(ptr + inodeno) = 1;
    DevWriteBlock(1, ptr);
    free(ptr);
}

void ResetInodeBytemap(int inodeno) {
    DevOpenDisk();
    char* ptr = malloc(BLOCK_SIZE);
    DevReadBlock(1, ptr);
    *(ptr + inodeno) = 0;
    DevWriteBlock(1, ptr);
    free(ptr);
}

void SetBlockBytemap(int blkno) {
    DevOpenDisk();
    char* ptr = malloc(BLOCK_SIZE);
    DevReadBlock(2, ptr);
    *(ptr + blkno) = 1;
    DevWriteBlock(2, ptr);
    free(ptr);
}

void ResetBlockBytemap(int blkno) {
    DevOpenDisk();
    char* ptr = malloc(BLOCK_SIZE);
    DevReadBlock(2, ptr);
    *(ptr + blkno) = 0;
    DevWriteBlock(2, ptr);
    free(ptr);
}

void PutInode(int inodeno, Inode* pInode) {
    DevOpenDisk();
    char* ptr = malloc(BLOCK_SIZE);
    int blknum = inodeno / 16 + 3;
    DevReadBlock(blknum, ptr);
    memcpy(ptr + inodeno % 16 * 32, pInode, sizeof(Inode));
    DevWriteBlock(blknum, ptr);
    free(ptr);
}

void GetInode(int inodeno, Inode* pInode) {
    DevOpenDisk();
    char* ptr = malloc(BLOCK_SIZE);
    int blknum = inodeno / 16 + 3;
    DevReadBlock(blknum, ptr);
    memcpy(pInode, ptr + inodeno % 16 * 32, sizeof(Inode));
    free(ptr);
}

int GetFreeInodeNum(void) {
    DevOpenDisk();
    char* ptr = malloc(BLOCK_SIZE);
    DevReadBlock(1, ptr);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (*(ptr + i) == 0) {
            free(ptr);
            return i;
        }
    }
    free(ptr);
    return -1;
}

int GetFreeBlockNum(void) {
    DevOpenDisk();
    char* ptr = malloc(BLOCK_SIZE);
    DevReadBlock(2, ptr);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (*(ptr + i) == 0) {
            free(ptr);
            return i;
        }
    }
    free(ptr);
    return -1;
}

void PutIndirectBlockEntry(int blkno, int index, int number) {
    DevOpenDisk();
    int* ptr = malloc(BLOCK_SIZE);
    DevReadBlock(blkno, (char*)ptr);
    *(ptr + index) = number;
    DevWriteBlock(blkno, (char*)ptr);
    free(ptr);
}

int GetIndirectBlockEntry(int blkno, int index) {
    DevOpenDisk();
    int* ptr = malloc(BLOCK_SIZE);
    DevReadBlock(blkno, (char*)ptr);
    int rt = *(ptr + index);
    free(ptr);
    return rt;
}

void RemoveIndirectBlockEntry(int blkno, int index){
    DevOpenDisk();
    int* ptr = malloc(BLOCK_SIZE);
    DevReadBlock(blkno, (char*)ptr);
    *(ptr + index) = INVALID_ENTRY;
    DevWriteBlock(blkno, (char*)ptr);
    free(ptr);
}

void PutDirEntry(int blkno, int index, DirEntry* pEntry) {
    DevOpenDisk();
    DirEntry* ptr = malloc(BLOCK_SIZE);
    DevReadBlock(blkno, (char*)ptr);
    memcpy(ptr + index, pEntry, sizeof(DirEntry));
    DevWriteBlock(blkno, ptr);
    free(ptr);
}

int GetDirEntry(int blkno, int index, DirEntry* pEntry) {
    DevOpenDisk();
    DirEntry* ptr = malloc(BLOCK_SIZE);
    DevReadBlock(blkno, (char*)ptr);
    int rt = (ptr + index)->inodeNum == -1 ? -1 : 1;
    memcpy(pEntry, ptr + index, sizeof(DirEntry));
    free(ptr);
    return rt;
}

void RemoveDirEntry(int blkno, int index){
    DevOpenDisk();
    DirEntry* ptr = malloc(BLOCK_SIZE);
    DevReadBlock(blkno, (char*)ptr);
    (ptr + index) -> inodeNum = -1;
    DevWriteBlock(blkno, ptr);
    free(ptr);
}