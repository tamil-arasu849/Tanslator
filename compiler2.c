#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

//variable to check the no of braces
int noOfCurlyBraces=0; 
int noOfBrackets=0;
int noOfDoubleQuotes=0;
const int noOfBlankSpaces=4;

//variable to save the no. of lines in the 'ubi' code
int noOfLines=1;

int noOfvariablesDeclared=0;

//global variables to store '.ubi' file contents
char ubiCode[20000];
int ubiIndex=0;

//global variables to store the contentes of '.C. file
char cCode[30000];
int cIndex=0;

//character array to store the declared and defined variables
char variableArray[2000][200];

//keywords in the ubi language
char *store="store"; //start
char *checkif="checkif";
char *notif="notif";
char *loop="loop";
char *display="display"; //end

//keywords in the c language
char *Int="int ";
char *If="if";
char *Else="else";
char *While="while";
char *Printf="printf";
char *Void="void ";
char *Main="main()";
char *Include="#include<stdio.h>\n";
char *Conio="#include<conio.h>\n\n";

//function declarations
void checkFileName(char*); //function to check whether the file format is '.ubi' or not
void compiler();
char* tokenise(); //function to return the tokens
void spaceRemover(); //function to remove the spaces
void storeFun();
char *createCFile(char*); //function to create the name for a 'c' file
void addIntoCCode(char*);
void checkifFun();
void terminate();
void skipAndAddNewLine();
void notifFun();
void loopFun();
void variablesFun(char*);
void displayFun();
int checkIfKeyword(char*);
void addBlankSpaces(); //function to add the blank spaces into the 'C' code
void addIfExpressionIntoC(); //function that adds the if statement expression into C code
void addIntoDisplay(); // function that adds the contents of a display function into 'C' code
void declareVariable(char*);
int isVariableNameDeclared(char*);
void defineVariable(char*);
int isVariableNameDefined(char*);
char* removeQuotes(char*);
void addWhileExpressionIntoC();

char* removeQuotes(char* removeQuotesName)
{
    int length;
    int i=2;
    char *temp=(char*)malloc(sizeof(char*)*40);
    length=strlen(removeQuotesName);

    temp[0]=removeQuotesName[1];
    while(removeQuotesName[i]!='"')
    {
        temp[i-1]=removeQuotesName[i];
        i++;
    }

    temp[i-1]='\0';
    return temp;
}

void variablesFun(char* variablesFunName)
{
    char buffer[200];
    int i=0;
    i=isVariableNameDeclared(variablesFunName);

    if(i==1)
    {
        addIntoCCode(variablesFunName);
        
        if(ubiCode[ubiIndex]=='+' && ubiCode[ubiIndex+1]=='+')
        {
            strcpy(buffer, tokenise());
            addIntoCCode(buffer);
            strcpy(buffer, tokenise());
            addIntoCCode(buffer);

            if(ubiCode[ubiIndex]==';')
            {
                strcpy(buffer, tokenise());
                addIntoCCode(buffer);
                defineVariable(variablesFunName);
            }

            else if(ubiCode[ubiIndex]=='\n')
            {
                printf("\nLine %d : Newline is not alloewd after '%s'.\nCompilation terminated <----->\n",noOfLines, buffer);
                terminate();
            }

            else
            {
                printf("\nLine %d : ';' is expected after '%s'.\nCompilation terminated <----->\n",noOfLines, buffer);
                terminate();
            }
        }

        else if(ubiCode[ubiIndex]=='-' && ubiCode[ubiIndex+1]=='-')
        {
            strcpy(buffer, tokenise());
            addIntoCCode(buffer);
            strcpy(buffer, tokenise());
            addIntoCCode(buffer);

            if(ubiCode[ubiIndex]==';')
            {
                strcpy(buffer, tokenise());
                addIntoCCode(buffer);
                defineVariable(variablesFunName);
            }

            else if(ubiCode[ubiIndex]=='\n')
            {
                printf("\nLine %d : Newline is not alloewd after '%s'.\nCompilation terminated <----->\n",noOfLines, buffer);
                terminate();
            }

            else
            {
                printf("\nLine %d : ';' is expected after '%s'.\nCompilation terminated <----->\n",noOfLines, buffer);
                terminate();
            }
        }

        else if(ubiCode[ubiIndex]=='=')
        {
            strcpy(buffer, tokenise());
            addIntoCCode(buffer);
            strcpy(buffer, tokenise());

            while((buffer[0]>='a' && buffer[0]<='z') || (buffer[0]>='A' && buffer[0]<='Z') || buffer[0]=='_' || (buffer[0]>='0' && buffer[0]<='9') || buffer[0]=='(' || buffer[0]==')' || buffer[0]=='-' || buffer[0]=='+' || buffer[0]=='*' || buffer[0]=='/' || buffer[0]=='%')
            {
                if(buffer[0]=='_' || (buffer[0]>='a' && buffer[0]<='z') || (buffer[0]>='A' && buffer[0]<='Z'))
                {
                    isVariableNameDefined(buffer);
                }
                
                addIntoCCode(buffer);
                strcpy(buffer, tokenise());
            }

            if(buffer[0]==';')
            {
                addIntoCCode(buffer);
                defineVariable(variablesFunName);
            }

            else if(buffer[0]=='\n')
            {
                printf("\nLine %d : Newline is not allowed here.\nCompilation terminated <----->\n", noOfLines);
                terminate();
            }

            else
            {
                printf("\nLine %d : '%s' is not allowed here.\nCompilation terminated <----->\n",noOfLines, buffer);
                terminate();
            }
        }
    }

    else
    {
        printf("\nLine %d : '%s' is not declared.\nCompilation terminated <----->\n",noOfLines, variablesFunName);
        terminate();
    }

    skipAndAddNewLine();
}

void declareVariable(char* declareVariableName)
{
    int i;
    checkIfKeyword(declareVariableName);
    i=isVariableNameDeclared(declareVariableName);

    if(i==1)
    {
        printf("\nLine %d : '%s' is already declared.\nCompilation terminated <----->\n",noOfLines, declareVariableName);
        terminate();
    }

    else
    {
        noOfvariablesDeclared++;
        strcpy(variableArray[2*noOfvariablesDeclared-2], declareVariableName);
        strcpy(variableArray[2*noOfvariablesDeclared-1], "0");
    }
}

void defineVariable(char* declareVariableName)
{
    int j;
    j=isVariableNameDeclared(declareVariableName);

    if(j==1)
    {
        for(int i=0; i<2*noOfvariablesDeclared; i=i+2)
        {
            if(!strcmp(variableArray[i], declareVariableName))
            {
                strcpy(variableArray[i+1], "1");
            }
        }
    }

    else
    {
        printf("\nLine %d : '%s' is not declared.\nCompilation terminated <----->\n",noOfLines, declareVariableName);
        terminate();
    }
}

int isVariableNameDeclared(char* isVariableNameDeclaredName)
{
    for(int i=0;i<2*noOfvariablesDeclared; i=i+2)
    {
        if(!strcmp(variableArray[i], isVariableNameDeclaredName))
        {
            return 1;
        }
    }
    return 0;
}

int isVariableNameDefined(char* isVariableNameDefinedName)
{
    int j;
    j=isVariableNameDeclared(isVariableNameDefinedName);

    if(j==1)
    {
        for(int i=0; i<2*noOfvariablesDeclared; i=i+2)
        {
            if(!strcmp(variableArray[i], isVariableNameDefinedName) && !strcmp(variableArray[i+1], "1"))
            {
                return 1;
            }
        }

        printf("\nLine %d : '%s' is not defined.\nCompilation terminated <----->\n",noOfLines, isVariableNameDefinedName);
        terminate();
    }

    else
    {
        printf("\nLine %d : '%s' is not declared.\nCompilation terminated <----->\n",noOfLines, isVariableNameDefinedName);
        terminate();
    }
    
}

void skipAndAddNewLine()
{
    char buffer[200];

    while(ubiCode[ubiIndex]=='\n')
    {
        strcpy(buffer, tokenise());
    }
    addIntoCCode("\n");
}

char *createCFile(char* ubiFileName)
{
    int lengthOfUbiFile=strlen(ubiFileName);
    char* cFileName=(char*)malloc(sizeof(char*)*lengthOfUbiFile);
    strcpy(cFileName, ubiFileName);
    cFileName[lengthOfUbiFile-2]='\0';
    cFileName[lengthOfUbiFile-3]='c';

    return cFileName;
}

void addIntoCCode(char* addIntoCCodeBuffer)
{
    int addIntoCCodeIndex=0;
    while(addIntoCCodeBuffer[addIntoCCodeIndex]!='\0')
    {
        cCode[cIndex]=addIntoCCodeBuffer[addIntoCCodeIndex];
        cIndex++;
        addIntoCCodeIndex++;
    }
}

void terminate()
{
    exit(0);
}

//INDIRECT--> This function is called by the function 'displayFun()'
void addIntoDisplay()
{
    char buffer[200];
    strcpy(buffer, tokenise());

    if(!strcmp(buffer, "\""))
    {
        addIntoCCode(buffer);
        noOfDoubleQuotes=1;

        while (noOfDoubleQuotes!=0)
        {
            strcpy(buffer, tokenise());
            if(!strcmp(buffer, "\""))
            {
                noOfDoubleQuotes=0;
            }
            addIntoCCode(buffer);
        }

        strcpy(buffer, tokenise());

        while (strcmp(buffer, ";") && noOfBrackets!=0 && strcmp(buffer, "\n"))
        {
            if(buffer[0]>='0' && buffer[0]<='9')
            {
                addIntoCCode(buffer);
            }

            if(!strcmp(buffer, ","))
            {
                addIntoCCode(buffer);
            }

            if(buffer[0]=='-' || buffer[0]=='+' || buffer[0]=='*' || buffer[0]=='/')
            {
                addIntoCCode(buffer);
            }

            if((buffer[0]>='a' && buffer[0]<='z') || (buffer[0]>='A' && buffer[0]<='Z') || buffer[0]=='_')
            {
                isVariableNameDefined(buffer);
                addIntoCCode(buffer);
            }

            if(!strcmp(buffer, ")"))
            {
                addIntoCCode(buffer);
                noOfBrackets=0;
                break;
            }
            strcpy(buffer, tokenise());
        }

        strcpy(buffer, tokenise());
        if(!strcmp(buffer, ";") && noOfBrackets==0)
        {
            addIntoCCode(buffer);
            addIntoCCode("\n");
        }

        else if(strcmp(buffer, ";") && noOfBrackets==0)
        {
            printf("\nLine %d : ';' required.\nCompilation terminated <----->\n", noOfLines);
            terminate();
        }
    }

    else
    {
        printf("\nLine %d : \" required.\nCompilation terminated <----->\n", noOfLines);
        terminate();
    }
}

//INDIRECT--> This function is called by 'loopFun()'
void addWhileExpressionIntoC()
{
    char buffer[200];
    if(ubiCode[ubiIndex]==')')
    {
        strcpy(buffer, tokenise());
        addIntoCCode(buffer);
        noOfBrackets--;
    }

    while(noOfBrackets!=0 && ubiCode[ubiIndex]!='{')
    {
        if(ubiCode[ubiIndex]=='<' || ubiCode[ubiIndex]=='>' || (ubiCode[ubiIndex]=='<' && ubiCode[ubiIndex+1]=='=') || (ubiCode[ubiIndex]=='>' && ubiCode[ubiIndex+1]=='=') || (ubiCode[ubiIndex]=='=' && ubiCode[ubiIndex+1]=='=') || ubiCode[ubiIndex]=='!' || (ubiCode[ubiIndex]=='|' && ubiCode[ubiIndex+1]=='|') || (ubiCode[ubiIndex]=='&' && ubiCode[ubiIndex+1]=='&') || ubiCode[ubiIndex]=='&' || ubiCode[ubiIndex]=='|' || ubiCode[ubiIndex]=='^' || (ubiCode[ubiIndex]=='!' && ubiCode[ubiIndex]=='='))
        {
            strcpy(buffer, tokenise());
            addIntoCCode(buffer);
        }

        else if(ubiCode[ubiIndex]=='(')
        {
            strcpy(buffer, tokenise());
            addIntoCCode(buffer);
            noOfBrackets++;
        }

        else if(ubiCode[ubiIndex]==')')
        {
            strcpy(buffer, tokenise());
            addIntoCCode(buffer);
            noOfBrackets--;
        }

        else if((ubiCode[ubiIndex]>='a' && ubiCode[ubiIndex]<='z') || (ubiCode[ubiIndex]>='A' && ubiCode[ubiIndex]<='Z') || ubiCode[ubiIndex]=='_')
        {
            strcpy(buffer, tokenise());
            isVariableNameDefined(buffer);
            addIntoCCode(buffer);
        }

        else if(ubiCode[ubiIndex]>='0' && ubiCode[ubiIndex]<='9')
        {
            strcpy(buffer, tokenise());
            addIntoCCode(buffer);
        }

        else
        {
            strcpy(buffer, tokenise());
            printf("\nLine %d : '%s' is not allowed here.\nCompilation terminated <----->\n",noOfLines, buffer);
            terminate();
        }
    }

    skipAndAddNewLine();
    strcpy(buffer, tokenise());
    if(!strcmp(buffer, "{"))
    {
        addBlankSpaces();
        addIntoCCode(buffer);
        addIntoCCode("\n");
        noOfCurlyBraces++;
    }       
    
    else
    {
        printf("\nLine %d : '{' xccjvdfv required.\nCompilation terminated <----->\n",noOfLines);
        terminate();
    }
}

//INDIRECT--> This function is called by 'checkifFun()'
void addIfExpressionIntoC()
{
    char buffer[200];
    if(ubiCode[ubiIndex]==')')
    {
        strcpy(buffer, tokenise());
        addIntoCCode(buffer);
        noOfBrackets--;
    }

    while(noOfBrackets!=0 && ubiCode[ubiIndex]!='{')
    {
        if(ubiCode[ubiIndex]=='<' || ubiCode[ubiIndex]=='>' || (ubiCode[ubiIndex]=='<' && ubiCode[ubiIndex+1]=='=') || (ubiCode[ubiIndex]=='>' && ubiCode[ubiIndex+1]=='=') || (ubiCode[ubiIndex]=='=' && ubiCode[ubiIndex+1]=='=') || ubiCode[ubiIndex]=='!' || (ubiCode[ubiIndex]=='|' && ubiCode[ubiIndex+1]=='|') || (ubiCode[ubiIndex]=='&' && ubiCode[ubiIndex+1]=='&') || ubiCode[ubiIndex]=='&' || ubiCode[ubiIndex]=='|' || ubiCode[ubiIndex]=='^')
        {
            strcpy(buffer, tokenise());
            addIntoCCode(buffer);
        }

        else if(ubiCode[ubiIndex]=='(')
        {
            strcpy(buffer, tokenise());
            addIntoCCode(buffer);
            noOfBrackets++;
        }

        else if(ubiCode[ubiIndex]==')')
        {
            strcpy(buffer, tokenise());
            addIntoCCode(buffer);
            noOfBrackets--;
        }

        else if((ubiCode[ubiIndex]>='a' && ubiCode[ubiIndex]<='z') || (ubiCode[ubiIndex]>='A' && ubiCode[ubiIndex]<='Z') || ubiCode[ubiIndex]=='_')
        {
            strcpy(buffer, tokenise());
            isVariableNameDefined(buffer);
            addIntoCCode(buffer);
        }

        else if(ubiCode[ubiIndex]>='0' && ubiCode[ubiIndex]<='9')
        {
            strcpy(buffer, tokenise());
            addIntoCCode(buffer);
        }

        else
        {
            strcpy(buffer, tokenise());
            printf("\nLine %d : '%s' is not allowed here.\nCompilation terminated <----->\n",noOfLines, buffer);
            terminate();
        }
    }

    skipAndAddNewLine();
    strcpy(buffer, tokenise());
    if(!strcmp(buffer, "{"))
    {
        addBlankSpaces();
        addIntoCCode(buffer);
        addIntoCCode("\n");
        noOfCurlyBraces++;
    }       
    
    else
    {
        printf("\nLine %d : '{' xccjvdfv required.\nCompilation terminated <----->\n",noOfLines);
        terminate();
    }
}

int checkIfKeyword(char* variableName)
{
    if(!strcmp(variableName, store) || !strcmp(variableName, checkif) || !strcmp(variableName, notif) || !strcmp(variableName, loop) || !strcmp(variableName, display))
    {
        printf("\nLine %d : '%s' is a keyword. It can't be a variable name.\nCompilation terminated <----->\n",noOfLines, variableName);
        terminate();
    }
    return 1;
}
    
void addBlankSpaces()
{
    for(int i=1; i<=noOfBlankSpaces*(noOfCurlyBraces+1); i++)
    {
        addIntoCCode(" ");
    }    
}

void storeFun()
{
    char buffer[200];
    addIntoCCode(Int);

    if((ubiCode[ubiIndex]>='a' && ubiCode[ubiIndex]<='z') || (ubiCode[ubiIndex]>='A' && ubiCode[ubiIndex]<='Z') || ubiCode[ubiIndex]=='_')
    {
        strcpy(buffer, tokenise());
        declareVariable(buffer);
        addIntoCCode(buffer);
        char tempBuffer[200];
        strcpy(tempBuffer, buffer);        
        strcpy(buffer, tokenise());

        if(!strcmp(buffer, ";"))
        {
            addIntoCCode(buffer);
            
            while (ubiCode[ubiIndex]!='\n' && ubiCode[ubiIndex]!='\0')
            {
                strcpy(buffer, tokenise());
            }
            addIntoCCode("\n");
        } 

        else if (!strcmp(buffer, "="))
        {
            addIntoCCode(buffer);
            
            if(ubiCode[ubiIndex]>='0' && ubiCode[ubiIndex]<='9')
            {
                strcpy(buffer, tokenise());
                defineVariable(tempBuffer);
                addIntoCCode(buffer);
                strcpy(buffer, tokenise());

                if(!strcmp(buffer, ";"))
                {
                    addIntoCCode(buffer);
                    skipAndAddNewLine();
                }

                else if(!strcmp(buffer, "\n"))
                {
                    printf("\nLine %d : Newline is not permitted after variable definition.\nCompilation terminated <---->\n", noOfLines);
                    terminate();
                }

                else
                {
                    printf("\nLine %d : ';' required.\nCompilation terminated <----->\n", noOfLines);
                    terminate();
                }
            }

            else if(ubiCode[ubiIndex]==';')
            {
                printf("\nLine %d : Variable '%s' needs to be defined.\nCompilation terminated <----->\n",noOfLines, tempBuffer);
                terminate();
            }

            else if(ubiCode[ubiIndex]=='\n')
            {
                printf("\nLine %d : Newline is not permitted after an assignment operator.\nCompilation terminated <----->\n", noOfLines);
                terminate();
            }

            else
            {
                printf("\nLine %d : Invalid variable definition for the variable '%s'.\nCompilation terminated <----->\n", noOfLines, tempBuffer);
                terminate();
            }
        }

        else
        {
            printf("\nLine %d : '%s' is not allowed here.\nCompilation terminated <----->\n",noOfLines,buffer);
            terminate();
        }        
    }

    else
    {
        printf("\nLine %d : Invalid variable.\nCompilation terminated <----->\n", noOfLines);
        terminate();
    }
}

void loopFun()
{
    char buffer[200];
    addIntoCCode(While);
    strcpy(buffer, tokenise());

    if(!strcmp(buffer, "("))
    {
        addIntoCCode(buffer);
        noOfBrackets++;
        addWhileExpressionIntoC();
    }

    else
    {
        printf("\nLine %d : '(' required.\nCompilation terminated <----->\n", noOfLines);
        terminate();
    }   
}

void checkifFun()
{
    char buffer[200];
    addIntoCCode(If);
    strcpy(buffer, tokenise());

    if(!strcmp(buffer, "("))
    {
        addIntoCCode(buffer);
        noOfBrackets++;
        addIfExpressionIntoC();
    }

    else
    {
        printf("\nLine %d : '(' required.\nCompilation terminated <----->\n", noOfLines);
        terminate();
    }
}

void notifFun()
{
    char buffer[200];
    addIntoCCode(Else);

    skipAndAddNewLine();
    strcpy(buffer, tokenise());
    
    if(!strcmp(buffer, "{"))
    {
        addBlankSpaces();
        addIntoCCode("{");
        noOfCurlyBraces++;
        skipAndAddNewLine();
    }

    else
    {
        printf("\nLine %d : '{' required.\nCompilation terminated <----->\n", noOfLines);
        terminate();
    }
}

void displayFun()
{
    char buffer[200];
    addIntoCCode(Printf);
    strcpy(buffer, tokenise());

    if(!strcmp(buffer, "("))
    {
        addIntoCCode(buffer);
        noOfBrackets++;
        addIntoDisplay();
    }

    else if(!strcmp(buffer, "\n"))
    {
        printf("\nLine %d : Newline is not allowed here.\nCompilation terminated <----->\n", noOfLines);
        terminate();
    }

    else
    {
        printf("\nLine %d : '(' required.\nCompilation terminated <----->\n", noOfLines);
        terminate();
    }
}

void spaceRemover()
{
    while((ubiCode[ubiIndex]==' ' || ubiCode[ubiIndex]=='\t') && noOfDoubleQuotes==0)
    {
        ubiIndex++;
    }
}

char* tokenise()
{
    int size=400;
    char *tokeniseBuffer=(char*)malloc(sizeof(char*)*size);
    int i=0;

    spaceRemover();

    //returns a variable name or a keyword
    if((ubiCode[ubiIndex]>='a' && ubiCode[ubiIndex]<='z') || (ubiCode[ubiIndex]>='A' && ubiCode[ubiIndex]<='Z') || ubiCode[ubiIndex]=='_')
    {
        while((ubiCode[ubiIndex]>='a' && ubiCode[ubiIndex]<='z') || (ubiCode[ubiIndex]>='A' && ubiCode[ubiIndex]<='Z') || (ubiCode[ubiIndex]>='0' && ubiCode[ubiIndex]<='9') || ubiCode[ubiIndex]=='_')
        {
            tokeniseBuffer[i]=ubiCode[ubiIndex];
            i++;
            ubiIndex++;
        }

        tokeniseBuffer[i]='\0';

        spaceRemover();
        return tokeniseBuffer;
    }

    //returns a number, either an integer or float
    if(ubiCode[ubiIndex]>='0' && ubiCode[ubiIndex]<='9')
    {
        while(ubiCode[ubiIndex]>='0' && ubiCode[ubiIndex]<='9')
        {
            tokeniseBuffer[i]=ubiCode[ubiIndex];
            i++;
            ubiIndex++;
        }

        if(ubiCode[ubiIndex]=='.' && (ubiCode[ubiIndex+1]>='0' && ubiCode[ubiIndex+1]<='9'))
        {
            tokeniseBuffer[i]=ubiCode[ubiIndex];
            ubiIndex++;
            i++;
            while(ubiCode[ubiIndex]>='0' && ubiCode[ubiIndex]<='9')
            {
                tokeniseBuffer[i]=ubiCode[ubiIndex];
                i++;
                ubiIndex++;
            }
        }
        tokeniseBuffer[i]='\0';

       spaceRemover();
       return tokeniseBuffer;
    }

    //returns operator '!=', '&&', '||', '==', '>=' or '<='  
    if((ubiCode[ubiIndex]=='&' && ubiCode[ubiIndex+1]=='&') || (ubiCode[ubiIndex]=='!' && ubiCode[ubiIndex+1]=='=') || (ubiCode[ubiIndex]=='|' && ubiCode[ubiIndex+1]=='|') || (ubiCode[ubiIndex]=='=' && ubiCode[ubiIndex+1]=='=') || (ubiCode[ubiIndex]=='>' && ubiCode[ubiIndex+1]=='=') || (ubiCode[ubiIndex]=='<' && ubiCode[ubiIndex+1]=='='))
    {
        tokeniseBuffer[i]=ubiCode[ubiIndex];
        i++;
        ubiIndex++;

        tokeniseBuffer[i]=ubiCode[ubiIndex];
        i++;
        ubiIndex++;

        tokeniseBuffer[i]='\0';

        spaceRemover();
        return tokeniseBuffer;
    }

    //returns single symbols
    if(((ubiCode[ubiIndex]==32 || ubiCode[ubiIndex]=='\t')  && noOfDoubleQuotes!=0) || ubiCode[ubiIndex]=='\n' || (ubiCode[ubiIndex]>=33 && ubiCode[ubiIndex]<=47) || (ubiCode[ubiIndex]>=58 && ubiCode[ubiIndex]<=64) || (ubiCode[ubiIndex]>=91 && ubiCode[ubiIndex]<=94) || ubiCode[ubiIndex]==96 || (ubiCode[ubiIndex]>=123 && ubiCode[ubiIndex]<=126))
    {
        tokeniseBuffer[i]=ubiCode[ubiIndex];
        ubiIndex++;
        i++;

        tokeniseBuffer[i]='\0';

        // spaceRemover function is called only if the parsed string is not a double quote
        if(strcmp(tokeniseBuffer, "\""))
        {
            spaceRemover();
        }

        if(!strcmp(tokeniseBuffer, "\n"))
        {
            noOfLines++;
        }
        return tokeniseBuffer;
    }
}

void compiler()
{
    char buffer[200];
    //append the two header files in the 'cCode' string
    addIntoCCode(Include);
    addIntoCCode(Conio);

    //appends the 'void main()' string
    addIntoCCode(Void);
    addIntoCCode(Main);
    addIntoCCode("\n{\n");

    while (ubiCode[ubiIndex]!='\0')
    {
        strcpy(buffer, tokenise());

        if(!strcmp(buffer, "}"))
        {
            if(noOfCurlyBraces==0)
            {
                printf("Line %d : '{' required.\nCompilation terminated <----->\n",noOfLines);
                terminate();
            }

            else if(noOfCurlyBraces>0)
            {
                noOfCurlyBraces--;
                addBlankSpaces();
                addIntoCCode("}");
                while (ubiCode[ubiIndex]=='\n')
                {
                    strcpy(buffer, tokenise());
                }
                addIntoCCode("\n");
            }
        }

        else if(strcmp(buffer, "\n"))
        {
            addBlankSpaces();
        }

        if(!strcmp(buffer, store))
        {
            storeFun();
        }

        else if(!strcmp(buffer, checkif))
        {
            checkifFun();
        }

        else if(!strcmp(buffer, notif))
        {
            notifFun();
        }

        else if(!strcmp(buffer, display))
        {
            displayFun();
        }

        else if(!strcmp(buffer, loop))
        {
            loopFun();
        }

        else if(checkIfKeyword(buffer) && (buffer[0]>='a' && buffer[0]<='z') || (buffer[0]>='A' && buffer[0]<='Z') || buffer[0]=='_')
        {
            variablesFun(buffer);
        }
    }
}

void checkFileName(char* FileName)
{
    int length=strlen(FileName);
    if(FileName[length-1]!='i' && FileName[length-2]!='b' && FileName[length-3]!='u')
    {
        printf("\nFile format not supported\n.Compilation terminated <----->\n");
        terminate();
    }
}

void main()
{
    printf("Enter the file name : "); //getting the file name from the user
    char fileName[100];
    gets(fileName);
    if(fileName[0]=='"')
    {
        strcpy(fileName, removeQuotes(fileName));        
    }
    checkFileName(fileName);
    FILE* ubiFp;
    ubiFp=fopen(fileName, "r");

    if(ubiFp==NULL) //terminates the program if there is no content in the file
    {
        printf("\nCan't open the file %s.\nCompilation terminated <----->\n", fileName);
        terminate();
    }

    int ubiI=0;
    char ubiChar;
    ubiChar=fgetc(ubiFp);

    while(ubiChar!=EOF) //copying the contents of the file in the global variable 'Code'
    {
        ubiCode[ubiI]=ubiChar;
        ubiI++;
        ubiChar=fgetc(ubiFp);
    }

    fclose(ubiFp); //close the file aftre reading its contents

    compiler();
    if(noOfCurlyBraces!=0)
    {
        printf("\nLine %d : '}' required.\nCompilation terminated <----->\n", noOfLines);
        terminate();
    } 
    addIntoCCode("\n}");

    //creating a 'c' file
    char cFileName[100];
    strcpy(cFileName, createCFile(fileName));
    FILE* cFp;
    cFp=fopen(cFileName, "w+"); //creating a C file
    fprintf(cFp, "%s", cCode); //appending the contents of the buffer into the C file
    fclose(cFp);
}