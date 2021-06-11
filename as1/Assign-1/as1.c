
//Implimentation of Pass-1 of assembler

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

static int newadd=0; //default addrees start from '0'
static int symsr=1;
static int stentry=0;
static int ltrsr=1;
static int ptsr=1;
int address;

typedef struct srccode
{
	char label[30];
	char mnemonic[30];
	char operand1[30];
	char operand2[30];
}srccode; // for reading instr from src code


typedef struct motcontent
{
	int sr;
	char name[30];
	int opcode;
	char class[30];
}motcontent; // for reading instr from mot file


typedef struct opcodeclass
{
	char a1[10];
	char a2[10];
}opcodeclass;


typedef struct operand1reg
{
	char a1[10];
	char a2[10];
}operand1reg;


typedef struct operand2sym
{
	char a1[10];
	char a2[10];
}operand2sym;



void writedatatost(char label[],int add)
{
	FILE *fpsym;
		
	fpsym=fopen("symtable.txt","a");
	fprintf(fpsym,"%d\t%s\t%d\n",symsr,label,add);
    symsr++;
	fclose(fpsym);
	stentry=1;
}//writing a data to symbol table
 
 
int issymbolalreadyexist(char label[])
{
	FILE *fpsym;
	int sr,symadd;
	char symname[30];

		fpsym=fopen("symtable.txt","r");
		fscanf(fpsym,"%d%s%d",&sr,symname,&symadd);
		while(!feof(fpsym))
		{
			if(strcmp(symname,label)==0)
			return 1;	//already present
			fscanf(fpsym,"%d%s%d",&sr,symname,&symadd);
		}//while
		fclose(fpsym);
return 0;  //not present
}//if the symbol already exist in symbol table
 
 

void updateaddressinst(char label[],int add)
{
	FILE *fpsym,*fptemp;
	int sr,symadd;
	char symname[30];

		fpsym=fopen("symtable.txt","r");
		fptemp=fopen("temp.txt","w");
		fscanf(fpsym,"%d%s%d",&sr,symname,&symadd);
		while(!feof(fpsym))
		{
			if(strcmp(symname,label)==0)
				fprintf(fptemp,"%d\t%s\t%d\n",sr,label,add);
			else
				fprintf(fptemp,"%d\t%s\t%d\n",sr,symname,symadd);	
			fscanf(fpsym,"%d%s%d",&sr,symname,&symadd);
		}//while
		fclose(fpsym);
		fclose(fptemp);
		
		remove("symtable.txt");
		rename("temp.txt","symtable.txt");
		remove("temp.txt");
}//updating the address in the symbol table
  

int loc_of_sym(char operand[])
{
	FILE *fpsym;
	int sr,symadd;
	char symname[30];
	
		fpsym=fopen("symtable.txt","r");
		fscanf(fpsym,"%d%s%d",&sr,symname,&symadd);
		while(!feof(fpsym))
		{
			if(strcmp(symname,operand)==0)
				return sr;
			fscanf(fpsym,"%d%s%d",&sr,symname,&symadd);
		}//while
	return -1;	
}//return the sr no of the symbol from the symbol table


int add_of_sym(char operand[])
{
	FILE *fpsym;
	int sr,symadd;
	char symname[30];
	
		fpsym=fopen("symtable.txt","r");
		fscanf(fpsym,"%d%s%d",&sr,symname,&symadd);
		while(!feof(fpsym))
		{
			if(strcmp(symname,operand)==0)
				return symadd;
			fscanf(fpsym,"%d%s%d",&sr,symname,&symadd);
		}//while
	return -1;	
}//return the sr no of the symbol from the symbol table



void writedatatolt(char label[],int add)
{
	FILE *fpltr;
		
	fpltr=fopen("ltrtable.txt","a");
	fprintf(fpltr,"%d\t%s\t%d\n",ltrsr,label,add);
    ltrsr++;
	fclose(fpltr);
}//writing a data to symbol table


int findtotalPT()
{

	int cnt=0;
	FILE *fpltr;
	int sr,ltradd;
	char ltrname[30];
		fpltr=fopen("ltrtable.txt","r");
		fscanf(fpltr,"%d%s%d",&sr,ltrname,&ltradd);
		while(!feof(fpltr))
		{
			cnt++;
			fscanf(fpltr,"%d%s%d",&sr,ltrname,&ltradd);
		}//while
		fclose(fpltr);

return cnt;


}


int isliteralalreadyexist(char label[30])
{
	FILE *fpltr;
	int sr,ltradd;
	char ltrname[30];
		fpltr=fopen("ltrtable.txt","r");
		fscanf(fpltr,"%d%s%d",&sr,ltrname,&ltradd);
		//printf("\nhii\n");
		while(!feof(fpltr))
		{
			//printf("%d %s %d",&sr,ltrname,&ltradd);
			//printf("Comaprison:%d",strcmp(ltrname,label));		
			if(strcmp(ltrname,label)==0 && ltradd==-1)
			return 1;	//already present
			fscanf(fpltr,"%d%s%d",&sr,ltrname,&ltradd);
		}//while
		fclose(fpltr);

return 0;  //not present
}//if the symbol already exist in symbol table



int loc_of_ltr(char operand[])
{
	FILE *fpltr;
	int sr,ltradd;
	char ltrname[30];
	
		fpltr=fopen("ltrtable.txt","r");
		fscanf(fpltr,"%d%s%d",&sr,ltrname,&ltradd);
		while(!feof(fpltr))
		{
			if(strcmp(ltrname,operand)==0)
				return sr;
			fscanf(fpltr,"%d%s%d",&sr,ltrname,&ltradd);
		}//while
	return -1;	
}//return the sr no of the symbol from the symbol table



int writedataintoic(opcodeclass a,operand1reg b,operand2sym c)
{
	FILE *fpic;
		
		fpic=fopen("ictable.txt","a");
		if(strcmp(a.a1,"AD")==0)
			fprintf(fpic,"\t*\t(%s,%s)\t(%s,%s)\t(%s,%s)\n",a.a1,a.a2,b.a1,b.a2,c.a1,c.a2);
		else
		{
			fprintf(fpic,"\t%d\t(%s,%s)\t(%s,%s)\t(%s,%s)\n",newadd,a.a1,a.a2,b.a1,b.a2,c.a1,c.a2);
			//printf("\nNew address Found %d",newadd);
			newadd++;
		}
		fclose(fpic);
}// write data in IC



int returnsymboladdress(char symbol[])
{
	FILE *fpsym;
	int sr,symadd;
	char symname[30];
		fpsym=fopen("symtable.txt","r");
		fscanf(fpsym,"%d%s%d",&sr,symname,&symadd);			
		while(!feof(fpsym))
		{
			if(strcmp(symname,symbol)==0)
			{
			return symadd;
			}
			fscanf(fpsym,"%d%s%d",&sr,symname,&symadd);
		}
		fclose(fpsym);
	
return 1;
}//return symbol address




void updateicbyltrtable()
{
	FILE *fpltr,*fpic,*fppt,*fptemp;
	opcodeclass a;
	int sr,ltradd;
	char ltrname[30];
	
	fpltr=fopen("ltrtable.txt","r");
	fpic=fopen("ictable.txt","a");
	fppt=fopen("pooltable.txt","a");
	fptemp=fopen("temp.txt","w");

	fscanf(fpltr,"%d%s%d",&sr,ltrname,&ltradd);
	while(!feof(fpltr))
	{
		if(ltradd==-1)
		{
			fprintf(fpic,"\t%d\t%s\t(-,-)\t(-,-)\n",newadd,ltrname);
			fprintf(fptemp,"%d\t%s\t%d\n",sr,ltrname,newadd);
			newadd++;
		}
		else
		{
			fprintf(fptemp,"%d\t%s\t%d\n",sr,ltrname,ltradd);
		}
	     fscanf(fpltr,"%d%s%d",&sr,ltrname,&ltradd);
	}//while
	
	fclose(fpltr);
	fclose(fpic);
	fclose(fppt);
	fclose(fptemp);

	remove("ltrtable.txt");
	rename("temp.txt","ltrtable.txt");
	remove("temp.txt");
}//writing data to ic using literal table



void writeaddofltr()
{
	int sr,ltradd;
	char ltrname[30];
	FILE *fpltr,*fpic,*fptemp;
	fpltr=fopen("ltrtable.txt","r");
	fpic=fopen("ictable.txt","a");	
	fptemp=fopen("temp.txt","w");

	fscanf(fpltr,"%d%s%d",&sr,ltrname,&ltradd);
	while(!feof(fpltr))
	{
		if(ltradd==-1)
		{
			fprintf(fpic,"%d\t%s\t(-,-)\t(-,-)\n",newadd,ltrname);
			fprintf(fptemp,"\t%d\t%s\t%d\n",sr,ltrname,newadd);
			newadd++;
		}
		else
		{
			fprintf(fptemp,"\t%d\t%s\t%d\n",sr,ltrname,ltradd);
		}
	    	fscanf(fpltr,"%d%s%d",&sr,ltrname,&ltradd);
	}//while


fclose(fpltr);
fclose(fpic);
fclose(fptemp);
remove("ltrtable.txt");
rename("temp.txt","ltrtable.txt");
remove("temp.txt");
}//after end statement is 



int issymbolisvalid()
{
	FILE *fpsym;
	int sr,symadd;
	char symname[30];
	int flag=1;
	
		fpsym=fopen("symtable.txt","r");
		fscanf(fpsym,"%d%s%d",&sr,symname,&symadd);			
		while(!feof(fpsym))
		{
			if(symadd==-1)
			{
				//printf("\nSymbol '%s' has no address",symname);
				flag=0;
			}
			fscanf(fpsym,"%d%s%d",&sr,symname,&symadd);		}
		fclose(fpsym);
	if(flag==0)
	return 0;	
return 1;
}//check validity of a symbol





int main()
{
	remove("symtable.txt");
	remove("ictable.txt");
	remove("pooltable.txt");
	remove("ltrtable.txt");

	char srcfile[]="code.txt"; //source file
	char motfile[]="mot.txt"; //mot file
	FILE *fpcode,*fpmot,*fpltr,*fppt;
	
	srccode sc;      //creating object of structure
	motcontent mot;  //creating object of structure
		
	opcodeclass a;
	operand1reg b;
	operand2sym c;	
		
	int motflag=0,updateaddvalue=0,ltorgflag=0,startflag=0;
	char tempstr[30],*token;	
		
	fpltr=fopen("ltrtable.txt","w");
	fclose(fpltr);
	
	fppt=fopen("pooltable.txt","w");
	fclose(fppt);
	
	fppt=fopen("pooltable.txt","a");
	fprintf(fppt,"%d\t#1\n",ptsr,findtotalPT());
	ptsr++;
		
	fpcode=fopen(srcfile,"r");
	fscanf(fpcode,"%s%s%s%s",sc.label,sc.mnemonic,sc.operand1,sc.operand2);
    if(strcmp(sc.operand1,"*")!=0)
	{
		newadd=atoi(sc.operand1);		
		address=newadd;
	}//if starting address is present		
    while(!(feof(fpcode)))
    {
    
    
       //for label field
    	if(strcmp(sc.label,"*")!=0)
    	{
    		if(stentry==0)
    		{
    			writedatatost(sc.label,newadd);
    		}//if no other symbol is present in symbol table
    		else
    		{
    			if(issymbolalreadyexist(sc.label)==0)
					writedatatost(sc.label,newadd);
				else
					updateaddressinst(sc.label,newadd);	 		   				
			}//if one of the symbol is already present in symbol table		
    	}//for label other than * symbol
    	
    
    
    	//for mnemonic field
    	if(strcmp(sc.mnemonic,"*")!=0)
    	{
    		motflag==0;
    		fpmot=fopen(motfile,"r");

    			fscanf(fpmot,"%d%s%d%s",&mot.sr,mot.name,&mot.opcode,mot.class);
    			while(!(feof(fpmot)))
    			{
    				//printf("%s\n",mot.name);
    				if(strcmp(sc.mnemonic,mot.name)==0)
    				{
    					//printf("hii...");
    					//printf("%s\n",mot.name);    					
    					strcpy(a.a1,mot.class);
    					sprintf(a.a2,"%d",mot.opcode);
					motflag=1;
    					break;
    					//printf("%s\t%s\n",a.a1,a.a2);
    				}//mnemonic found in mot
    				
    				fscanf(fpmot,"%d%s%d%s",&mot.sr,mot.name,&mot.opcode,mot.class);
    			}//while
    			
    			if(motflag==0)
    			{
    				printf("\nInvalid Mnemonic %s\n",sc.mnemonic);
    				exit(0);
    			}//mnomonic not fond in mot
    		    fclose(fpmot);		
    	}//for mnemonic checking mot entries
    
    	
    	
    	//for operand1 field
    	if(strcmp(sc.operand1,"*")!=0)
    	{
    		if(strcmp(sc.operand1,"AREG")==0 || strcmp(sc.operand1,"A")==0)
    		{
    			if(strcmp(sc.operand2,"*")!=0)
    			{
    				strcpy(b.a1,"R");
    				sprintf(b.a2,"%d",1);
    				//printf("%s\t%s\n",b.a1,b.a2);
    			}//means both the operands are present
    			else
    			{
    				if(stentry==0)
    				{
    					writedatatost(sc.operand1,-1);
    				}//if no other symbol is present in symbol table
    				else
    				{
    					if(issymbolalreadyexist(sc.label)==0)
							writedatatost(sc.operand1,-1);
				}//if one of the symbol is already present in symbol table		

					strcpy(b.a1,"S");
					sprintf(b.a2,"%d",loc_of_sym(sc.operand1));
					//printf("%s\t%s\n",b.a1,b.a2);	
    			}//means only one operand is present
    		}//operand1 is A register

			
			else if(strcmp(sc.operand1,"BREG")==0 || strcmp(sc.operand1,"B")==0)
    		{
    			if(strcmp(sc.operand2,"*")!=0)
    			{
    				strcpy(b.a1,"R");
    				sprintf(b.a2,"%d",2);
    				//printf("%s\t%s\n",b.a1,b.a2);
    			}//means both the operands are present
    			else
    			{
    				if(stentry==0)
    				{
    					writedatatost(sc.operand1,-1);
    				}//if no other symbol is present in symbol table
    				else
    				{
    					if(issymbolalreadyexist(sc.label)==0)
							writedatatost(sc.operand1,-1);
					}//if one of the symbol is already present in symbol table		

					strcpy(b.a1,"S");
					sprintf(b.a2,"%d",loc_of_sym(sc.operand1));
					//printf("%s\t%s\n",b.a1,b.a2);	
    			}//means only one operand is present
    		}//operand1 is B register
    		
    		
    		else if(strcmp(sc.operand1,"CREG")==0 || strcmp(sc.operand1,"C")==0)
    		{
    			if(strcmp(sc.operand2,"*")!=0)
    			{
    				strcpy(b.a1,"R");
    				sprintf(b.a2,"%d",3);
    				//printf("%s\t%s\n",b.a1,b.a2);
    			}//means both the operands are present
    			else
    			{
    				if(stentry==0)
    				{
    					writedatatost(sc.operand1,-1);
    				}//if no other symbol is present in symbol table
    				else
    				{
    					if(issymbolalreadyexist(sc.label)==0)
							writedatatost(sc.operand1,-1);
					}//if one of the symbol is already present in symbol table		

					strcpy(b.a1,"S");
					sprintf(b.a2,"%d",loc_of_sym(sc.operand1));
					//printf("%s\t%s\n",b.a1,b.a2);	
    			}//means only one operand is present
    		}//operand1 is C register
			
			
			else if(strcmp(a.a1,"DL")==0)
			{
				strcpy(b.a1,"C");
				strcpy(b.a2,sc.operand1);
				//printf("%s\t%s\n",b.a1,b.a2);
			}//if it is a DS or DC statements
			
			
			else if(strcmp(sc.operand1,"EQ")==0 || strcmp(sc.operand1,"NEQ")==0 || strcmp(sc.operand1,"GT")==0 || strcmp(sc.operand1,"GE")==0 || strcmp(sc.operand1,"LT")==0 || strcmp(sc.operand1,"LE")==0)
			{
				if(strcmp(sc.operand2,"*")!=0)
    				{
					FILE *fpco;
					int sr,code;
					char operator[20];
				
						fpco=fopen("cond_operator.txt","r");
						fscanf(fpco,"%d%s%d",&sr,operator,&code);
						while(!(feof(fpco)))
						{
							if(strcmp(operator,sc.operand1)==0)
							{
								strcpy(b.a1,"C");
								sprintf(b.a2,"%d",code);
								//printf("%s\t%s\n",b.a1,b.a2);
							}//if it is a conditional operator
							fscanf(fpco,"%d%s%d",&sr,operator,&code);
						}//read till the end of file
				}//if 2nd operand is present
				else
				{
					if(stentry==0)
    					{
    						writedatatost(sc.operand1,-1);
    					}//if no other symbol is present in symbol table
    					else
    					{
    						if(issymbolalreadyexist(sc.label)==0)
							writedatatost(sc.operand1,-1);
						}//if one of the symbol is already present in symbol table		

					strcpy(b.a1,"S");
					sprintf(b.a2,"%d",loc_of_sym(sc.operand1));
					//printf("%s\t%s\n",b.a1,b.a2);
				}//consider it as a symbol		
			}//if it is a conditional operator

			
				else 
				{
					strcpy(tempstr,sc.operand1);
					token=strtok(tempstr,"+");
				if(startflag!=0)
				{		
					if(stentry==0)
    				{
    					writedatatost(token,-1);
    				}//if no other symbol is present in symbol table
    				else
    				{
    						if(issymbolalreadyexist(token)==0)
								writedatatost(token,-1);
					
					}//if one of the symbol is already present in symbol table		

					strcpy(b.a1,"S");
					sprintf(b.a2,"%d",loc_of_sym(token));
					//printf("%s\t%s\n",b.a1,b.a2);

				}//if
				else
				{
					startflag=1;
					strcpy(b.a1,"C");
					sprintf(b.a2,"%d",newadd);
				}		
			}//else	
			
			/*else
			{
				strcpy(b.a1,"-");
				strcpy(b.a2,"-");
			}//if there is no operand
			*/
    	}//for operand1
    	else
    	{
    		strcpy(b.a1,"-");
			strcpy(b.a2,"-");
    	}//if operand1 is '*'
    	
    	
		//for operand2 field
		if(strcmp(sc.operand2,"*")!=0)
    	{
    		//printf("hii\n");
    		if(strcmp(sc.operand2,"AREG")==0 || strcmp(sc.operand2,"A")==0 || strcmp(sc.operand2,"BREG")==0 || strcmp(sc.operand2,"B")==0 || strcmp(sc.operand2,"CREG")==0 || strcmp(sc.operand2,"C")==0)
    		{
    			//printf("hii\n");
    			if(stentry==0)
    			{
    				writedatatost(sc.operand2,-1);
    			}//if no other symbol is present in symbol table
    			else
   				{
    				if(issymbolalreadyexist(sc.operand2)==0)
						writedatatost(sc.operand2,-1);
				}//if one of the symbol is already present in symbol table		

				strcpy(c.a1,"S");
				sprintf(c.a2,"%d",loc_of_sym(sc.operand2));
				//printf("%s\t%s\n",c.a1,c.a2);	
    		}//operand2 is A,B,C register
    		
    		
    		
    		else if(sc.operand2[0]=='=')
    		{
    			//printf("hii\n");
				//printf("Returned Value:%d",isliteralalreadyexist(sc.operand2));
				if(isliteralalreadyexist(sc.operand2)==0) 
				{
					//printf("hiih");
					writedatatolt(sc.operand2,-1);
					strcpy(c.a1,"l");
					sprintf(c.a2,"%d",ltrsr-1);
				}
				else
				{
					strcpy(c.a1,"l");
					sprintf(c.a2,"%d",loc_of_ltr(sc.operand2)); //to convert integer to string
				}//if literal is already present in the literal table
				//printf("%s\t%s\n",c.a1,c.a2);		   			
    		
    		}//if operand2 is a literal
    		
    		
    		else
    		{
    			//printf("hii\n");
    			if(stentry==0)
    			{
    				writedatatost(sc.operand2,-1);
    			}//if no other symbol is present in symbol table
    			else
   				{
    				if(issymbolalreadyexist(sc.operand2)==0)
						writedatatost(sc.operand2,-1);
					//else
						//updateaddressinst(sc.operand2,address);		
				}//if one of the symbol is already present in symbol table		

				strcpy(c.a1,"S");
				sprintf(c.a2,"%d",loc_of_sym(sc.operand2));
				//printf("%s\t%s\n",c.a1,c.a2);
    		}//if operand2 is a symbol
   		
    	}//for operand2
    	else
    	{
    		strcpy(c.a1,"-");
    		strcpy(c.a2,"-");
			//printf("%s\t%s\n",c.a1,c.a2);
    	}//for operand 2
    	
    	
    	
    	// for DS i.e. it's value is given
		if(strcmp(a.a1,"DL")==0 && strcmp(a.a2,"1")==0)
		{
			updateaddvalue=0;
			updateaddvalue=atoi(sc.operand1)-1;
		}//for DS
			
    	
    	//if mnemonic is a "LTORG" assembler directive
    	if(strcmp(a.a1,"AD")==0 && strcmp(a.a2,"3")==0)// for LTORG
		{
			//printf("\nMenmonic is LTORG");
			ltorgflag=1;
		}
    	if(ltorgflag==1)
		{
			updateicbyltrtable();
			fprintf(fppt,"%d\t#%d\n",ptsr,findtotalPT()+1);
			//printf("new value in pt:%d\t#%d\n",ptsr,findtotalPT()+1);
			
			ptsr++;	
		
			ltorgflag=0;
		}//if "LTORG" is encountered	
    	
    	
    	//if mnemonic is a "EQU" assembler directive
    	if(strcmp(a.a1,"AD")==0 && strcmp(a.a2,"4")==0)// for EQU
		{
			updateaddressinst(sc.label,returnsymboladdress(sc.operand1));
		}//for EQU
			
    	
    	//if mnemonic is a "ORIGIN" assembler directive
    	if(strcmp(a.a1,"AD")==0 && strcmp(a.a2,"5")==0)
		{
			//printf("\nMenmonic is Orgin");
			if(strcmp(sc.operand1,"*")==0)
				newadd=address;
			else
			{
 				strcpy(tempstr,sc.operand1);
				token=strtok(tempstr,"+");
				newadd=returnsymboladdress(token);
				//printf("\nreturned value of add %d",newadd);
					if(strlen(sc.operand1)>strlen(token))
					{
						token=strtok(NULL,"+");
						newadd=newadd+atoi(token);
						
					}//if
				
				if(newadd==-1)
				{
					printf("\nError...Forward Reference Symbol '%s'\n\n",sc.operand1);
					exit(0);
				}
					
			}//else	
				
		}//for "ORIGIN"
			
    	
    	
  	writedataintoic(a,b,c);
	newadd=newadd+updateaddvalue;
	updateaddvalue=0;
	
	//printf("%s %s %s %s\n",sc.label,sc.mnemonic,sc.operand1,sc.operand2);   
    fscanf(fpcode,"%s%s%s%s",sc.label,sc.mnemonic,sc.operand1,sc.operand2);
    }//outer while
    
	
	writeaddofltr();
	
	fclose(fpcode);
	//fclose(fpltr);
	 
	if(issymbolisvalid()==0)
	{
		printf("\nError...Invalid symbol\n");	
		exit(0);
	}    
   
fclose(fppt);
		
}//main


