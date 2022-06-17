#include "hw1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disk.h"
#include "hw2.h"

void FileSysInit()
{
    DevCreateDisk();
    char *ptr = malloc(FS_DISK_CAPACITY);
    memset(ptr, 0, sizeof(ptr));
    for (int blk = 0; blk < FS_DISK_CAPACITY / BLOCK_SIZE; blk++)
    {
        DevWriteBlock(blk, ptr + BLOCK_SIZE * blk);
    }

    free(ptr);
}

void SetInodeBytemap(int inodeno)
{
    char *ptr = malloc(BLOCK_SIZE);
    DevReadBlock(1, ptr);
    ptr[inodeno] = 1;
    DevWriteBlock(1, ptr);
    free(ptr);
}

void ResetInodeBytemap(int inodeno)
{
    char *ptr = malloc(BLOCK_SIZE);
    DevReadBlock(1, ptr);
    ptr[inodeno] = 0;
    DevWriteBlock(1, ptr);
    free(ptr);
}

void SetBlockBytemap(int blkno)
{
    char *ptr = malloc(BLOCK_SIZE);
    DevReadBlock(2, ptr);
    ptr[blkno] = 1;
    DevWriteBlock(2, ptr);
    free(ptr);
}

void ResetBlockBytemap(int blkno)
{
    char *ptr = malloc(BLOCK_SIZE);
    DevReadBlock(2, ptr);
    ptr[blkno] = 0;
    DevWriteBlock(2, ptr);
    free(ptr);
}

void PutInode(int inodeno, Inode *pInode)   // 디스크에 inode를 저장한다.
{
    Inode *ptr = malloc(BLOCK_SIZE);
    int blknum = inodeno / 16 + 3;
    DevReadBlock(blknum, ptr);
    memcpy(ptr + inodeno % 16, pInode, sizeof(Inode));
    DevWriteBlock(blknum, ptr);
    free(ptr);
}

void GetInode(int inodeno, Inode *pInode)   // 지정한 주소로 inode를 가져온다.
{
    Inode *ptr = malloc(BLOCK_SIZE);
    int blknum = inodeno / 16 + 3;
    DevReadBlock(blknum, ptr);
    memcpy(pInode, ptr + inodeno % 16, sizeof(Inode));
    free(ptr);
}

int GetFreeInodeNum(void)
{
    char *ptr = malloc(BLOCK_SIZE);
    DevReadBlock(1, ptr);
    for (int i = 0; i < 64; i++)    // 최대 inode는 64개
    {
        if (ptr[i] == 0)
        {
            free(ptr);
            return i;
        }
    }
    free(ptr);
    return -1;
}

int GetFreeBlockNum(void)
{
    char *ptr = malloc(BLOCK_SIZE);
    DevReadBlock(2, ptr);
    for (int i = 7; i < BLOCK_SIZE; i++)
    {
        if (ptr[i] == 0)
        {
            free(ptr);
            return i;
        }
    }
    free(ptr);
    return -1;
}

void PutIndirectBlockEntry(int blkno, int index, int number)
{
    int *ptr = malloc(BLOCK_SIZE);
    DevReadBlock(blkno, ptr);
    *(ptr + index) = number;
    DevWriteBlock(blkno, ptr);
    free(ptr);
}

int GetIndirectBlockEntry(int blkno, int index)
{
    int *ptr = malloc(BLOCK_SIZE);
    DevReadBlock(blkno, ptr);
    int rt = *(ptr + index);
    free(ptr);
    return rt;
}

void RemoveIndirectBlockEntry(int blkno, int index)
{
    int *ptr = malloc(BLOCK_SIZE);
    DevReadBlock(blkno, ptr);
    *(ptr + index) = INVALID_ENTRY;
    DevWriteBlock(blkno, ptr);
    free(ptr);
}

void PutDirEntry(int blkno, int index, DirEntry *pEntry)    // 지정한 주소의 디렉토리 엔트리를 디스크에 넣는다.
{
    if(index >= 16) return;
    DirEntry *ptr = malloc(BLOCK_SIZE);
    DevReadBlock(blkno, ptr);
    memcpy(ptr + index, pEntry, sizeof(DirEntry));
    DevWriteBlock(blkno, ptr);
    free(ptr);
}

int GetDirEntry(int blkno, int index, DirEntry *pEntry) // 디렉토리 엔트리를 지정한 주소로 가져온다. invalid entry 일 시 -1을 반환한다.
{
    if(index >= 16) return INVALID_ENTRY;
    DirEntry *ptr = malloc(BLOCK_SIZE);
    DevReadBlock(blkno, ptr);
    if(ptr[index].inodeNum == INVALID_ENTRY){
        return INVALID_ENTRY;
    }
    memcpy(pEntry, ptr + index, sizeof(DirEntry));
    free(ptr);
    return 0;
}

void RemoveDirEntry(int blkno, int index)   // 디스크 내 특정 디렉토리 엔트리를 invalid entry로 바꾼다.
{
    if(index >= 16) return;
    DirEntry *ptr = malloc(BLOCK_SIZE);
    DevReadBlock(blkno, ptr);
    ptr[index].inodeNum = INVALID_ENTRY;
    DevWriteBlock(blkno, ptr);
    free(ptr);
}