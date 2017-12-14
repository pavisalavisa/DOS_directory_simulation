#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFF_SIZE 1024
#define NAME_TOO_LONG 7
#define RESULT_OK 0
#define EXIT_SIGNAL 5
#define NULL_POINTER -1
//binarno stablo je zato jer ima dva pokazivaca a to nije isto kao i binarno stablo pretrazivanja


struct _treeNode;
typedef struct _treeNode* Position;
typedef struct _treeNode{
    char name[BUFF_SIZE];
    Position next;
    Position child;
}_TREENODE;

int dir(Position);//ispis DOS style naredbe
int md(Position ,char*);
Position cd(char*,Position,Position);
Position cdDots(Position);
Position createNode();
Position find(Position,char*);
int insertUserData(Position,char*);
int push(Position,Position);
Position pop(Position);
int printMenu();
int printHelp();
int resolveChoice(char*,Position*,Position);
int printCurrentPath(const Position,const Position);
Position findLast(Position);
Position findPrevious(Position,char*);
int deleteNode(Position,char*);
int removeNode(Position,char*);
int numberOfElements(Position);
char** toArray(Position);

int main(int argc,char**argv)
{
    Position root=NULL,stack=NULL,currentDir=NULL;
    root=createNode();
    strcpy(root->name,"Root");
    stack=createNode();
    currentDir=root;
    printMenu();
    int choiceOption;
    char choice[64];
    do
    {
        printCurrentPath(currentDir,stack);
        gets(choice);
        if((choiceOption=resolveChoice(choice,&currentDir,stack))==0)
            printf("I don't understand.\nTry again.\n");
        if(choiceOption==NAME_TOO_LONG)
            printf("Name too long, try a shorter one.\n\n");
    }while(choiceOption!=EXIT_SIGNAL);

    return RESULT_OK;
}

int md(Position p,char* dirName)
{
    Position q=NULL,tmp=NULL;
    if(p==NULL)return -1;

    q=createNode();
    if(q==NULL)return -2;

    insertUserData(q,dirName);

    if(p->child==NULL)
    {
        p->child=q;
        return RESULT_OK;
    }
    tmp=p->child;
    while(tmp->next!=NULL)
        tmp=tmp->next;
    tmp->next=q;
    return RESULT_OK;

}

Position cd(char* fName,Position p,Position stack)
{
    Position found=NULL;

    if(p==NULL)return p;
    if(p->child==NULL) return p;

    found=find(p,fName);
    if(found==NULL)return p;
    push(stack,p);
    return found;
}

int dir(Position p)
{
    Position tmp=p;
    if(tmp==NULL) return NULL_POINTER;
    printf("\nDirectory of %s:\n",tmp->name);
    tmp=tmp->child;
    while(tmp!=NULL)
    {
        printf("\n %s",tmp->name);
        tmp=tmp->next;
    }
    printf("\n");
    return RESULT_OK;
}

Position cdDots(Position stack)
{
    if(stack->next==NULL)
        return NULL;
    return pop(stack);//vracanje na prethodnu razinu
}


Position createNode()
{
    Position p=NULL;
    p=(Position)malloc(sizeof(_TREENODE));
    if(p==NULL)return p;
    p->next=NULL;
    p->child=NULL;
    return p;
}


Position find(Position root,char* fName)
{
    Position tmp=NULL;
    if (root==NULL)return NULL;
    if (root->child==NULL)return NULL;
    if(strcmp(root->child->name,fName)==0)return root->child;
    tmp=root->child;
    while(tmp->next!=NULL&&strcmp(tmp->next->name,fName)!=0)
        tmp=tmp->next;
    if(tmp->next==NULL)return NULL;
    return tmp->next;
}

int insertUserData(Position root,char* dirName)
{
    if(root==NULL)return NULL_POINTER;
    strcpy(root->name,dirName);
    return RESULT_OK;
}

int push(Position stack, Position node)
{
    if(stack==NULL)return NULL_POINTER;
    if(node==NULL)return NULL_POINTER;
    node->next=stack->next;
    stack->next=node;
    return RESULT_OK;
}

Position pop(Position stack)
{
    if(stack==NULL)return NULL;
    Position tmp=NULL;
    tmp=stack->next;
    stack->next=tmp->next;
    return tmp;
}

int printMenu()
{
    printf("***DOS directory simulation***\n");
    printf("***Use help if you get stuck!\n");
    return RESULT_OK;
}

int printHelp()
{
    printf("\n***Help***\n");
    printf("cd <directory name> - choose directory from current directory\n");
    printf("md <new name> - create new directory in current directory\n");
    printf("cd.. - get back do prior directory\n");
    printf("exit- exits this simulation\n");
    return RESULT_OK;
}

int resolveChoice(char* choice,Position* currentDir,Position stack)
{
    char firstArgument[8],secondArgument[32];
    sscanf(choice,"%s %s",firstArgument,secondArgument);
    if(strlen(choice)>40)
        return NAME_TOO_LONG;
    if(strcmp(firstArgument,"md")==0)
    {
        md(*currentDir,secondArgument);
        return 1;
    }
    if(!strcmp(firstArgument,"cd"))
    {
        *currentDir=cd(secondArgument,*currentDir,stack);
        return 2;
    }if(!strcmp(firstArgument,"cd.."))
    {
        Position tmp=NULL;
        if((tmp=cdDots(stack))==NULL)
            return 3;
        *currentDir=tmp;
        return 3;
    }if(!strcmp(firstArgument,"dir"))
    {
        dir(*currentDir);
        return 4;
    }
    if(!strcmp(firstArgument,"exit"))
    {

        return 5;
    }
    if(!strcmp(firstArgument,"help"))
    {
        printHelp();
        return 6;
    }
    return 0;
}

int printCurrentPath(const Position currentDir,const Position stack)
{
    if(currentDir==NULL)return NULL_POINTER;
    if(stack==NULL)return NULL_POINTER;
    printf("C:\\Root");

    if(strcmp(currentDir->name,"Root")==0)
    {
        printf(">");
        return RESULT_OK;
    }
    char** stackArray=toArray(stack);
    int i;
    for(i=numberOfElements(stack)-1;i>0;i--)
    {
        printf("\\%s",stackArray[i]);
    }
    printf("\\%s>",currentDir->name);
    return RESULT_OK;
}


Position findLast(Position node)
{
    if(node==NULL)return NULL;
    Position iteratorNode=node;
    while(iteratorNode->next!=NULL)
        iteratorNode=iteratorNode->next;
    return iteratorNode;
}

Position findPrevious(Position node,char* name)
{
    if(node==NULL)return NULL;
    Position iteratorNode=node;
    while(iteratorNode->next!=NULL&&strcmp(iteratorNode->next->name,name)!=0)
    {
        iteratorNode=iteratorNode->next;
    }
    if(iteratorNode==NULL)return NULL;
    return iteratorNode;
}

int deleteNode(Position node,char* name)
{
    if(node==NULL)return NULL_POINTER;
    if(name==NULL)return NULL_POINTER;
    Position prevNode=findPrevious(node,name);
    if(prevNode==NULL)return NULL_POINTER;
    Position tmp=prevNode->next;
    prevNode->next=NULL;
    free(tmp);
    return 0;
}

int numberOfElements(Position node)
{
    if(node==NULL)return NULL_POINTER;

    int i=0;
    Position tmp=node;
    while(tmp->next!=NULL)
    {
        i++;
        tmp=tmp->next;
    }
    return i;
}

char** toArray(Position node)
{
    if(node==NULL)return NULL;
    int n=numberOfElements(node),i;
    char** array=(char**)malloc(n*sizeof(char*));
    char* tmpName=NULL;
    Position tmpNode=node;
    for(i=0;i<n;i++)
    {
        tmpName=(char*)malloc(strlen(tmpNode->next->name)*sizeof(char));
        strcpy(tmpName,tmpNode->next->name);
        array[i]=tmpName;
    }
    return array;
}
