#include <stdlib.h>
#include "fs.h"
#include "disk.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define INODE_SIZE (sizeof(Inode))
#define NUM_INODE_PER_BLOCK (BLOCK_SIZE/INODE_SIZE)	// 512/64 = 8
#define TOTAL_INODE_NUM (NUM_INODE_PER_BLOCK*INODELIST_BLOCKS) 	// 8 * 4 = 32
#define TOTAL_BLOCK_NUM (FS_DISK_CAPACITY/BLOCK_SIZE)	//32
#define TOTAL_BLKPTR_NUM_PER_INDIRECT_BLOCK (BLOCK_SIZE/sizeof(int))
#define TOTAL_DIRENTRY_NUM_PER_BLOCK (BLOCK_SIZE/sizeof(DirEntry))





/*
 *  This functions is implemented in validate.o to validate raw data in disk
 */
extern void vPrintInodeBytemap(void);
extern void vPrintBlockBytemap(void);
extern char vGetInodeBytemapValue(int num);
extern char vGetBlockBytemapValue(int num);
extern void vGetInodeValue(int inodeNum, Inode* pInode);
extern int vGetIndirectBlockEntry(int blkno, int index);
extern void vGetDirEntry(int blkno, int index, DirEntry* pDirEntry);

void testcase1(void)
{
	int i = 0;
	
	printf(" ---- Testcase 1 ----\n");

	FileSysInit();
	for(i = 0;i < TOTAL_BLOCK_NUM;i++)
	{
		SetBlockBytemap(i);
	}
	
	for(i = 0;i < TOTAL_BLOCK_NUM;i++)
	{
		if(i%8 == 7)
		{
			ResetBlockBytemap(i);
			if(i == GetFreeBlockNum())
			{
				if(vGetBlockBytemapValue(i) == 0)
				{
					SetBlockBytemap(i);
				}
				else
				{
					printf("Testcase 1: Fail : Blockmap byte %d is not 0.\n", 
							GetFreeBlockNum());
					return;
				}
				
			}
			else
			{
				printf("Testcase 1: Fail : GetFreeBlockNum() is incorrect!\n");
				return;
			}
		}
	}

	FileSysInit();
	for(i = 0;i < TOTAL_INODE_NUM;i++)
	{
		SetInodeBytemap(i);
	}
	
	for(i = 0;i < TOTAL_INODE_NUM;i++)
	{
		if(i%8 == 7)
		{	
			ResetInodeBytemap(i);
			if(i == GetFreeInodeNum())
			{
				if(vGetInodeBytemapValue(i) == 0)
				{
					SetInodeBytemap(i);
				}
				else 
				{
					printf("Testcase 1: Fail : InodeBytemap byte %d is not 0.\n",
								GetFreeInodeNum());
					return;
				}
			}
			else
			{
				printf("Testcase 1: Fail : GetFreeInodeNum() is incorrect!\n");
				return;
			}
		}
	}	
	printf("TestCase 1 Complete!\n");
}



void testcase2(void)
{
    int i =0;

	printf(" ---- Testcase 2 ----\n");

    FileSysInit();
    Inode* pInode = (Inode*)malloc(sizeof(Inode));
    Inode* pTmpInode = (Inode*)malloc(sizeof(Inode));

    for(i = 0;i < TOTAL_INODE_NUM;i++)
    {
        if(i%2 == 1)
        {
            pInode->allocBlocks = i;
            pInode->dirBlockPtr[0] = i;
            pInode->dirBlockPtr[NUM_OF_DIRECT_BLOCK_PTR - 1] = i;

            PutInode(i,pInode);
            vGetInodeValue(i, pTmpInode); 
			
            if(pTmpInode->allocBlocks == pInode->allocBlocks 
				&& pTmpInode->dirBlockPtr[0] == pInode->dirBlockPtr[0]
				&& pTmpInode->dirBlockPtr[NUM_OF_DIRECT_BLOCK_PTR - 1] == 
					pInode->dirBlockPtr[NUM_OF_DIRECT_BLOCK_PTR - 1])
            {
                
            }
            else
            {
                printf("TestCase 2: PutInode Failed!\n");
                return;
            }
        }
    }


    for(i = 0;i < TOTAL_INODE_NUM;i++)
    {
        if(i%2 == 1)
        {
            GetInode(i,pInode);
            vGetInodeValue(i, pTmpInode); 
			
           if(pTmpInode->allocBlocks == pInode->allocBlocks
                && pTmpInode->dirBlockPtr[0] == pInode->dirBlockPtr[0]
                && pTmpInode->dirBlockPtr[NUM_OF_DIRECT_BLOCK_PTR - 1] == pInode->dirBlockPtr[NUM_OF_DIRECT_BLOCK_PTR - 1])
            {
            
            }
            else
            {
                printf("TestCase 2: GetInode Failed!\n");
				return;
            }
            
        }
    }
    printf("Testcase 2 Complete!\n");
}

void testcase3(void)
{
	int i, j;

	printf(" ---- Testcase 3 ----\n");

	FileSysInit();
	for(i = 0;i < TOTAL_BLOCK_NUM;i++)
	{
		if(i%8 == 7)
		{
			for (j = 0;j < TOTAL_BLKPTR_NUM_PER_INDIRECT_BLOCK;j++)
			{
				PutIndirectBlockEntry(i, j, j);
				if (j == GetIndirectBlockEntry(i, j))
				{
					if (j != vGetIndirectBlockEntry(i, j)) 
					{
						printf("Testcase 3: Fail : Indirect block %d, The value of entry %d is %d.\n", i, j, vGetIndirectBlockEntry(i, j));
						return;	
					}
				}
				else
				{
					printf("Testcase 3: Fail: Put/GetIndirectBlockEntry() is incorrect!\n");
					return;
				}
				
			}
			for (j = 0;j < TOTAL_BLKPTR_NUM_PER_INDIRECT_BLOCK;j++)
			{
				RemoveIndirectBlockEntry(i, j);
				if (vGetIndirectBlockEntry(i, j) != INVALID_ENTRY) 
				{
					printf("Testcase 3: Fail : RemoveIndirectBlockEntry() is incorrect!\n");
					return ;
				}
			}
		}
	}
    printf("Testcase 3 Complete!\n");
}


void testcase4(void)
{
	int i;
	int j;
	DirEntry dirEntry;
	DirEntry tmpDirEntry;

	printf(" ---- Testcase 4 ----\n");

	FileSysInit();
	for(i = 0;i < TOTAL_BLOCK_NUM;i++)
	{
		if(i%8 == 7)
		{
			for (j = 0;j < TOTAL_DIRENTRY_NUM_PER_BLOCK;j++)
			{
				dirEntry.inodeNum = i*j;
				memset(dirEntry.name, 0, MAX_NAME_LEN);
				memset(tmpDirEntry.name, 0, MAX_NAME_LEN);
				sprintf(dirEntry.name, "/etc/dev%d", i*j);
				PutDirEntry(i, j, &dirEntry);
				if (GetDirEntry(i, j, &tmpDirEntry) < 0) 
				{
					printf("Testcase 4: Fail: block %d, index %d not found\n", i, j);
					return;
				}
				if (tmpDirEntry.inodeNum != dirEntry.inodeNum || 
						strcmp(tmpDirEntry.name, dirEntry.name) != 0) 
				{
					printf("Testcase 4: Fail: block %d, index %d is incorrect!\n", i, j);
					return;
				}

				memset(tmpDirEntry.name, 0, MAX_NAME_LEN);
				vGetDirEntry(i, j, &tmpDirEntry);
				if (tmpDirEntry.inodeNum != dirEntry.inodeNum || 
						strcmp(tmpDirEntry.name, dirEntry.name) != 0) 
				{
					printf("Testcase 4: Fail: block %d, index %d, DirEntry does not exist in disk!\n", i, j);
					return;
				}
			}
		}
	}

	for(i = 0;i < TOTAL_BLOCK_NUM;i++)
	{
		if(i%8 == 7)
		{
			for (j = 0;j < TOTAL_DIRENTRY_NUM_PER_BLOCK;j++)
			{
				memset(tmpDirEntry.name, 0, MAX_NAME_LEN);
				RemoveDirEntry(i, j);
				if (GetDirEntry(i, j, &tmpDirEntry) > 0) 
				{
					printf("Testcase 4: Fail: block %d, index %d is not removed\n", i, j);
					return;
				}
				
				if (tmpDirEntry.inodeNum != INVALID_ENTRY) 
				{
					printf("Testcase 4: Fail: inode number of removed direntry is valid.\n");
					return;
				}		
				
				memset(tmpDirEntry.name, 0, MAX_NAME_LEN);
				vGetDirEntry(i, j, &tmpDirEntry);
				if (tmpDirEntry.inodeNum != INVALID_ENTRY) 
				{
					printf("Testcase 4: Fail: inode number of removed direntry is valid.\n");
					return;
				}	
			}
		}
	}
    printf("Testcase 4 Complete!\n");
}

int main(int argc, char* argv[])
{
	int tcNum;
	if(argc != 2)
	{
		perror("Input TestCase Number!");
		exit(0);
	}
	
	tcNum = atoi(argv[1]);
	
	switch(tcNum)
	{
		case 1:
			testcase1();
			break;
		case 2:
			testcase2();
			break;
		case 3:
			testcase3();
			break;
		case 4:
			testcase4();
			break;
	}
	
	return 0;
}

