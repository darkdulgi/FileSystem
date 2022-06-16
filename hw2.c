#include <stdio.h>
#include <stdlib.h>
#include "disk.h"
#include "hw1.h"
#include "hw2.h"
#include <string.h>

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
    int newinodenum = GetFreeInodeNum();
    int newblocknum = GetFreeBlockNum();
    Inode* pinode;
    GetInode(0,pinode);

    char *nameread = strtok(name, "/");
    while (nameread != NULL){
        int curblocknum = pinode->dirBlockPtr[0];
        

        
        nameread = strtok(NULL, "/");
    }
    return 0;
}

int RemoveDirectory(char *name)
{
}

void CreateFileSystem(void)
{
    FileSysInit();
    
    DirEntry *ptr = malloc(BLOCK_SIZE);
    strcpy(ptr[0].name, ".");
    ptr[0].inodeNum = 0;
    for(int i = 1; i < 16; i++) ptr[i].inodeNum = INVALID_ENTRY;
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
    info->dataRegionBlock = 7;
    
    info->numAllocBlocks++;
    info->numFreeBlocks--;
    info->numAllocInodes++;

    DevWriteBlock(0,info);
    
    SetBlockBytemap(7);
    SetInodeBytemap(0);
    
    Inode* pinode;
    GetInode(0, pinode);
    pinode->dirBlockPtr[0] = 7;
    pinode->allocBlocks = 1;
    pinode->size = 512;
    PutInode(0, pinode);

    free(info);
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
