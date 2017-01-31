#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * codeRunner: run c code as an interpreted language such as python...
 *          it can save your trouble to write/compile the c program on your own.
 *          good for education/testing use.
 *
 * yshen 2017
 *
 * tour:
 *              1) enter c code and codeRunner will compile and run your code
 *
 *                       codeRunner:#1> int a = 100;
 *                       codeRunner:#2> printf("%d\n",a);
 *                       100
 *
 *                 after that you can enter another line of c code
 *
 *                       codeRunner:#3> printf("a=%d\n",a);
 *                       a=100
 *
 *              2) if your code failed to compile, such as
 *
 *                       codeRunner:#1> int a = 100;
 *                       codeRunner:#2> printf("%d\n",a) // ; is missing
 *                       error info output by the compiler
 *
 *                 you can enter again your code
 *
 *                       codeRunner:#2> printf("%d\n",a);
 *                       100
 *
 */

//#define DEBUG
#define bool int
#define true 1
#define false 0
#define BUFLEN 1024

/*
 * global variables
 */

/* options */
bool noPrompt = false;
bool debug = false;
char *template=NULL;
/* buffers */
char *newCodeBuffer = NULL;
char *oldCodeBuffer = NULL;
char *newOutputBuffer = NULL;
char *oldOutputBuffer = NULL;
char *actualOutputBuffer = NULL;
char *tempLineBuffer = NULL;
char *codeBody = NULL;
char *fileName="yourCode.c";
char *progName="yourProg";
int lineNo = 1;

typedef enum{
	CMD_BAD,
	CODE,
	CODE_CONT,
	CODE_BLOCK_START,
	CODE_BLOCK_END,
	CMD_LIST,
	CMD_QUIT,
	CMD_CLEAR,
	CMD_PRINT,
	CMD_OTHER,
	CMD_HELP,
} CmdType;
/*
 * function prototypes
 */
void InitBuffers();
void FreeBuffers();
CmdType GetOneLine();
void GenerateCode();
bool DoCompile();
void DoExecute();
void DoOutput();
void DoCleaning();
void help();
void usage();
void ListCodeBuffer();

/*
 * helper function
 */
void die(char *errMsg)
{
	fprintf(stderr, errMsg);
	abort();
}

void help()
{
	printf("\"list\" \"l\" list the c code buffer\n");
	printf("\"clear\" \"c\" clear the c code buffer\n");
	printf("\"print\" \"p\" print the old output buffer\n");
	printf("\"quit\" \"q\" quit the codeRunner\n");
	printf("\"exit\" \"e\" quit the codeRunner\n");
	printf("\"help\" \"h\" show the help\n");
}

void usage()
{
	printf("codeRunner: run c code as an interpreted language\n\n"
		   "usage: codeRunner [options]\n"
		   "options:\n"
		   "       --no-prompt              do not show prompt for the user\n"
		   "       --template=templatefile  use templatefile as the code template\n"
		   "       --help                   show this help\n"
		   "contact shenyufly@163.com for bugs or support.\n"
		   "Shen Yu 2017\n"
		  );
}
/*
 * PrintPrompt(): print the prompt on the screen to hint the user
 * of current program status.
 */
void PrintPrompt()
{
	if (!noPrompt)
		printf("codeRunner:#%d> ",lineNo);
}

/*
 * init all the buffers: alloc mem
 */
void InitBuffers()
{
	newCodeBuffer = calloc(BUFLEN,sizeof(char));
	oldCodeBuffer = calloc(BUFLEN,sizeof(char));
	newOutputBuffer = calloc(BUFLEN,sizeof(char));
	oldOutputBuffer = calloc(BUFLEN,sizeof(char));
	codeBody = calloc(BUFLEN,sizeof(char));
	actualOutputBuffer = calloc(BUFLEN,sizeof(char));
	lineNo = 1;
}

void FreeBuffers()
{
	free(newCodeBuffer);
	free(oldCodeBuffer);
	free(newOutputBuffer);
	free(oldOutputBuffer);
	free(codeBody);
	free(actualOutputBuffer);
}

/*
 * GetOneLine: get one line from stdin and store it in tempLineBuffer
 * see if is a command or c code
 * if all letters without any space or special chars then it is a command
 * else possibly be c code
 */
CmdType GetOneLine()
{
	int  n = 0;
	int  numRead = 0;
	int  i = 0;
	bool hasSpecials = false;
	bool isSemicolonEnded = false;
	bool hasLeftBranket = false;
	bool hasRightBranket = false;

	CmdType type = CMD_BAD;

	numRead = getline(&tempLineBuffer,&n,stdin);

	/* parse the user input line */
	for (i = numRead - 2; i >= 0; i--)
	{
		if (tempLineBuffer[i]=='{')
			hasLeftBranket = true;
		if (tempLineBuffer[i]=='}')
			hasRightBranket = true;
		if (!isalnum(tempLineBuffer[i]))
		{
			hasSpecials = true;
			break;
		}
	}

	isSemicolonEnded = tempLineBuffer[numRead-2] == ';' ? true : false;
 
	if (!hasSpecials && strncmp(tempLineBuffer,"l",1) == 0)
		type = CMD_LIST;
	else if (!hasSpecials && strncmp(tempLineBuffer,"q",1) == 0)
		type = CMD_QUIT;
	else if (!hasSpecials && strncmp(tempLineBuffer,"e",1) == 0)
		type = CMD_QUIT;
	else if (!hasSpecials && strncmp(tempLineBuffer,"c",1) == 0)
		type = CMD_CLEAR;
	else if (!hasSpecials && strncmp(tempLineBuffer,"p",1) == 0)
		type = CMD_PRINT;
	else if (!hasSpecials && strncmp(tempLineBuffer,"h",1) == 0)
		type = CMD_HELP;
	else if (hasLeftBranket)
		type = CODE_BLOCK_START;
	else if (hasRightBranket)
		type = CODE_BLOCK_END;
	else if (!isSemicolonEnded)
		type = CODE_CONT;
	else if (hasSpecials)
		type = CODE;
	else
		type = CMD_BAD;

	if (debug)
		printf("GetOneLine:%stype=%d\n",tempLineBuffer,type);

	return type;
}


/*
 * GenerateCode(): put newCodeBuffer in main()
 */
void GenerateCode()
{
	FILE *fp = NULL;
	int nWritten = 0;

	memset(codeBody, 0, sizeof(codeBody));

	/* if the user didn't provide a template, use our default */
	if (template == NULL)
	{
		strcat(codeBody, "#include <stdio.h>\n");
		strcat(codeBody, "#include <stdlib.h>\n");
		strcat(codeBody, "#include <ctype.h>\n");
		strcat(codeBody, "#include <math.h>\n");
		strcat(codeBody, "#include <string.h>\n");
		strcat(codeBody, "int main()\n");
		strcat(codeBody, "{\n");
		strcat(codeBody, newCodeBuffer);
		strcat(codeBody, "return 0;\n");
		strcat(codeBody, "}\n");
	}
	else
	{
		/* use the user provided template */
		FILE *templateFp = NULL;
		char *line = NULL;
		int n;
		int nRead = 0;

		templateFp = fopen(template, "r");

		while(0 < (nRead = getline(&line, &n, templateFp)))
		{
			if (strstr(line, "$$yourCode$$"))
			{
				strcat(codeBody, newCodeBuffer);
			}
			else
			{
				strcat(codeBody, line);
			}

			free(line);
		}

		fclose(templateFp);
	}
	
	/* now write to file */
	fp = fopen(fileName, "w");
	if (fp == NULL)
		die("cannot open file for reading\n");

	nWritten = fwrite(codeBody, sizeof(char), strlen(codeBody), fp);
	if (nWritten != strlen(codeBody))
		die("cannot write to file\n");

	fclose(fp);


	if (debug)
		printf("GenerateCode:\n%s",codeBody);

}


bool DoCompile()
{
	/* check if file exists */

	/* compile the file using gcc */
	char cmd[BUFLEN]={0};

	sprintf(cmd, "gcc %s -o %s", fileName, progName);
	int ret = system(cmd);

	if (ret != 0)
	{
		if (debug)
			printf("complie failed!\n");

		return false;
	}
	if (debug)
		printf("complie ok!\n");

	return true;
}
/*
 * DoExecute: 
 * 		open a pipe and execute the program
 * 		save the ret in a buffer
 */
void DoExecute()
{
	FILE *fp = NULL;
	int nRead = 0;
	int pos = 0;
	char cmd[BUFLEN]={0};

	sprintf(cmd, "./%s", progName);

	fp = popen(cmd,"r");
	if (fp == NULL)
		die("cannot execute the program\n");

	memset(newOutputBuffer, 0, sizeof(char)*BUFLEN);
	while(pos < BUFLEN && (nRead = fread(newOutputBuffer + (pos++), sizeof(char), 1, fp)));


	if (pos == BUFLEN - 1)
	{
		printf("Warning: output msg overflow internal buffer!\n");
	}

	if (debug)
		printf("DoExecute:\n%s", newOutputBuffer);


	pclose(fp);

}
void DoOutput()
{

	int i=0;
	int pos=0;
	int ch=0;
	/* we have some thing to print on the screen */
	if (memcmp(newOutputBuffer, oldOutputBuffer, sizeof(char)*BUFLEN) != 0)
	{
		/* just print the difference */
		while(newOutputBuffer[i] == oldOutputBuffer[i])
				i++;
	
		memset(actualOutputBuffer, 0, sizeof(char)*BUFLEN);
	
		while('\0'!=(ch=newOutputBuffer[i++]))
			actualOutputBuffer[pos++]=ch;
	
		printf("%s",actualOutputBuffer);
	
		memcpy(oldOutputBuffer, newOutputBuffer, sizeof(char)*BUFLEN);
	}
}
/*
 * internals:
 *              1) GetOneLine():get c code line input by user save it in [temp line buffer]
 *                  1.1) if a codeRunner command, do the command
 *                  1.2) if a c code
 *                       init [new code buffer] = [old code buffer]
 *                       append [temp line buffer] in [new code buffer]
 *                       and do the following stuffs.
 *              2) GenerateCode(): wrap up [new code buffer] in a main() body
 *                  2.1) we may have to add all the c header files in the
 *                       first place. this is the easiest way to handle
 *                       compile error.
 *              3) DoCompile(): compile the code using gcc 
 *              	3.1) if there is a compile error, this line is abandoned
 *              	     goto step (1)
 *              	3.2) if compile success, [old code buffer] = [new code buffer]
 *              4) DoExecute(): execute the program and save the output in [new output buffer]
 *              5) [actual output buffer] = [new output buffer] - [old output buffer]
 *              6) DoOutput(): do the actual output and save the output in [old output buffer]
 */
int main(int argc, char *argv[])
{
	int BlockLevel=0;

	/* process arguments */
	while(argc--)
	{
		if (strcmp(argv[argc],"--no-prompt") == 0)
		{
			noPrompt = true;
		}
		if (strstr(argv[argc],"--template="))
		{
			template = strdup(&argv[argc][11]);
		}
		if (strcmp(argv[argc],"--debug") == 0)
		{
			debug = true;
		}
		if (strcmp(argv[argc],"--help") == 0)
		{
			usage();
			exit(0);
		}
	}
	InitBuffers();
	while(1)
	{
		CmdType type;

		PrintPrompt();

		type = GetOneLine();

		switch(type)
		{
			case CODE_BLOCK_END:
				BlockLevel--;
			case CODE:

				strcat(newCodeBuffer, tempLineBuffer);
				free(tempLineBuffer);
	
				if (BlockLevel == 0)
				{
					GenerateCode();
		
					if (DoCompile())
					{
						DoExecute();
						DoOutput();
						DoCleaning();
						memcpy(oldCodeBuffer, newCodeBuffer, sizeof(char)*BUFLEN);
						lineNo++;
					}
					else
					{
						/* compile failed, restore the good old buffer */
						memcpy(newCodeBuffer, oldCodeBuffer, sizeof(char)*BUFLEN);
					}
				}
				break;
			case CODE_BLOCK_START:
				BlockLevel++;
			case CODE_CONT:
				strcat(newCodeBuffer, tempLineBuffer);
				free(tempLineBuffer);
				break;
			case CMD_LIST:
				printf("%s",oldCodeBuffer);
				break;
			case CMD_PRINT:
				printf("%s",oldOutputBuffer);
				break;
			case CMD_CLEAR:
				FreeBuffers();
				InitBuffers();	
				break;
			case CMD_HELP:
				help();
				break;
			case CMD_QUIT:
				printf("bye!\n");
				exit(0);
				break;

		}

	}
	return 0;
}
void DoCleaning()
{
	unlink(fileName);
	unlink(progName);
}
