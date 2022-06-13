#include <stdio.h>
#include <stdlib.h>
#include "disk.h"
#include "hw1.h"
#include "hw2.h"

FileDescTable *pFileDescTable;
FileSysInfo *pFileSysInfo;

int OpenFile(const char *name, OpenFlag flag)
{
}

int WriteFile(int fileDesc, char *pBuffer, int length)
{
}

int ReadFile(int fileDesc, char *pBuffer, int length)
{
}

int CloseFile(int fileDesc)
{
}

int RemoveFile(char *name)
{
}

int MakeDirectory(char *name)
{
}

int RemoveDirectory(char *name)
{
}

void CreateFileSystem(void)
{
    FileSysInit();
    DirEntry *ptr = malloc(BLOCK_SIZE);
    ptr[0].name = ".";
    ptr[0].inodeNum = 0;
    DevWriteBlock(7, ptr);
    free(ptr);

    FileSysInfo* info = malloc(BLOCK_SIZE);
    info->blocks = 512;
    info->rootInodeNum = 0;
    info->diskCapacity = 262144;
    info->numAllocBlocks = 0;
    info->numFreeBlocks = 505;
    info->numAllocInodes = 0;
    info->blockBytemapBlock = 2;
    info->inodeBytemapBlock = 1;
    info->inodeListBlock = 3;

    info->numAllocBlocks++;
    info->numFreeBlocks--;
    info->numAllocInodes++;

    DevWriteBlock(0,info);
    free(info);

    SetBlockBytemap(7);
    SetInodeBytemap(0);

    Inode inode;
    GetInode(0, &inode);
    inode.dirBlockPtr[0] = 7;
    inode.allocBlocks = 1;
    inode.size = 512;
    PutInode(0, &inode);

}
void OpenFileSystem(void)
{
}

void CloseFileSystem(void)
{
}

Directory *OpenDirectory(char *name)
{
}

FileInfo *ReadDirectory(Directory *pDir)
{
}

int CloseDirectory(Directory *pDir)
{
}
