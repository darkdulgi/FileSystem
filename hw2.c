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

int MakeDirectory(char *name) // 실패시 -1, 성공적으로 생성 시 0 반환
{   
    char tname[30];     // 경로의 깊이 계산
    strcpy(tname, name);
    char *temp = strtok(tname, "/");
    int depth = -1;
    while(temp != NULL){
        temp = strtok(NULL, "/");
        depth++;
    }

    int newinodenum = GetFreeInodeNum();
    int newblocknum = GetFreeBlockNum();
    Inode *pinode = malloc(sizeof(Inode));
    DirEntry *pdirentry = malloc(sizeof(DirEntry));
    GetInode(0, pinode);

    int here = 0;
    char *nameread = strtok(name, "/");
    while (nameread != NULL)
    {
        int curblocknum = pinode->dirBlockPtr[0];
        
        if (here < depth)
        {
            int dirFind = 0;
            for (int i = 0; i < 16; i++) {
                if (GetDirEntry(curblocknum, i, pdirentry) == INVALID_ENTRY) continue;
                if (strcmp(pdirentry->name, nameread) == 0){
                    dirFind = 1;
                    break;
                }
            }

            if(dirFind == 0){   // 상위 디렉토리가 존재하지 않음
                return -1;
            }
            GetInode(pdirentry->inodeNum, pinode);
        }
        else {
            GetDirEntry(curblocknum, 0, pdirentry)
            int parentinodenum = pdirentry->inodeNum;
            for (int i = 1; i < 16; i++) {
                if (GetDirEntry(curblocknum, i, pdirentry) == INVALID_ENTRY){  // 비어있는 엔트리에서 새 다랙토리 생성
                    strcpy(pdirentry->name, nameread);
                    pdirentry->inodeNum = newinodenum;
                    PutDirEntry(curblocknum, i, pdirentry);

                    SetBlockBytemap(newblocknum);
                    SetInodeBytemap(newinodenum);

                    GetInode(newinodenum, pinode);
                    pinode->dirBlockPtr[0] = newblocknum;
                    pinode->allocBlocks = 1;
                    pinode->type = FILE_TYPE_DIR;
                    pinode->size = 512;
                    PutInode(newinodenum, pinode);

                    DirEntry *ptr = malloc(BLOCK_SIZE);
                    strcpy(ptr[0].name, ".");
                    ptr[0].inodeNum = newinodenum;
                    strcpy(ptr[1].name, "..");
                    ptr[1].inodeNum = parentinodenum;
                    for (int i = 2; i < 16; i++)
                       ptr[i].inodeNum = INVALID_ENTRY;
                    DevWriteBlock(newblocknum, ptr);
                    free(ptr);
                    break;
                }
                else if(strcmp(pdirentry->name, nameread) == 0){    // 같은 이름의 디렉토리가 존재함
                    return -1;
                }
                if(i == 15) {    // 디렉토리가 꽉 차있음
                    return -1; 
                }
            }
        }
        
        here++;
        nameread = strtok(NULL, "/");
    }
    
    
    FileSysInfo *info = malloc(BLOCK_SIZE); // 디렉토리 생성을 마친 후 FileInfo 수정
    DevReadBlock(0, info);
    info->numAllocBlocks++;
    info->numFreeBlocks--;
    info->numAllocInodes++;
    DevWriteBlock(0, info);
    free(info);
    
    return 0;
}

int RemoveDirectory(char *name) // 실패시 -1, 성공적으로 제거 시 0 반환
{
    char tname[30];     // 경로의 깊이 계산
    strcpy(tname, name);
    char *temp = strtok(tname, "/");
    int depth = -1;
    while(temp != NULL){
        temp = strtok(NULL, "/");
        depth++;
    }

    Inode *pinode = malloc(sizeof(Inode));
    DirEntry *pdirentry = malloc(sizeof(DirEntry));
    GetInode(0, pinode);

    int here = 0;
    char *nameread = strtok(name, "/");
    while (nameread != NULL)
    {
        int curblocknum = pinode->dirBlockPtr[0];
        
        if (here < depth)
        {
            int dirFind = 0;
            for (int i = 0; i < 16; i++) {
                if (GetDirEntry(curblocknum, i, pdirentry) == INVALID_ENTRY) continue;
                if (strcmp(pdirentry->name, nameread) == 0){
                    dirFind = 1;
                    break;
                }
            }

            if(dirFind == 0){   // 상위 디렉토리가 존재하지 않음
                return -1;
            }
            GetInode(pdirentry->inodeNum, pinode);
        }
        else {
            GetDirEntry(curblocknum, 0, pdirentry)
            int parentinodenum = pdirentry->inodeNum;
            for (int i = 1; i < 16; i++) {
                
                
                if(i == 15) {    
                    return -1; 
                }
            }
        }
        
        here++;
        nameread = strtok(NULL, "/");
    }
    
    
    FileSysInfo *info = malloc(BLOCK_SIZE); // 디렉토리 생성을 마친 후 FileInfo 수정
    DevReadBlock(0, info);
    info->numAllocBlocks--;
    info->numFreeBlocks++;
    info->numAllocInodes--;
    DevWriteBlock(0, info);
    free(info);
    
    return 0;
}

void CreateFileSystem(void)
{
    FileSysInit();

    DirEntry *ptr = malloc(BLOCK_SIZE); // 루트 블록(7)의 디렉토리 엔트리 정의
    strcpy(ptr[0].name, ".");
    ptr[0].inodeNum = 0;
    for (int i = 1; i < 16; i++)
        ptr[i].inodeNum = INVALID_ENTRY;
    DevWriteBlock(7, ptr);
    free(ptr);

    FileSysInfo *info = malloc(BLOCK_SIZE);
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

    DevWriteBlock(0, info);
    free(info);

    SetBlockBytemap(7);
    SetInodeBytemap(0);


    Inode *pinode = malloc(sizeof(Inode)); // 루트 inode 정의
    GetInode(0, pinode);

    pinode->dirBlockPtr[0] = 7;
    pinode->allocBlocks = 1;
    pinode->type = FILE_TYPE_DIR;
    pinode->size = 512;
    
    PutInode(0, pinode);
    free(pinode); 
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
