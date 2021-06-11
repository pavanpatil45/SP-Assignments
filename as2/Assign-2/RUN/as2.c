
//Implimentation of Pass-2 of assembler

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int returnsymnameaddress(char operandindex[])
{
	FILE *fpsym;
	int symadd;
	char sr[10],symname[30];

		fpsym=fopen("sym.txt","r");
		fscanf(fpsym,"%s%s%d",sr,symname,&symadd);			
		while(!feof(fpsym))
		{
			if(strcmp(sr,operandindex)==0)
			{
				return symadd; //if symbol index is found then return address
			}
			fscanf(fpsym,"%s%s%d",sr,symname,&symadd);			
		}//check till the end of file
		fclose(fpsym);
return 1; //not found
}//it returns the symbol address from the symbol table


int returnliteraladdress(char operandindex[])
{
	FILE *fpltr;
	int ltradd;
	char sr[10],ltrname[30];

		fpltr=fopen("lit.txt","r");
		fscanf(fpltr,"%s%s%d",sr,ltrname,&ltradd);			
		while(!feof(fpltr))
		{
			if(strcmp(sr,operandindex)==0)
			{
				return ltradd; //if literal index is found then return address
			}
			fscanf(fpltr,"%s%s%d",sr,ltrname,&ltradd);
		}//check till the end of file
		fclose(fpltr);
return 1;  //not found
}//it returns the literal address from the literal table


int main()
{

	remove("targetcode.txt");	
	
	FILE *fptc,*fpic,*fpsym,*fpltr;
	char address[30],mnemonic[30],operand1[10],operand2[10];
	char str1_mne[10],str2_mne[10],str3_mne[10],str1_ope1[10],str2_ope1[10],str3_ope1[10],str1_ope2[10],str2_ope2[10],str3_ope2[10];
	int add1,add2;


	fptc=fopen("targetcode.txt","a");
	fpic=fopen("ic.txt","r");
	fpsym=fopen("sym.txt","r");
	fpltr=fopen("lit.txt","r");

	fscanf(fpic,"%s%s%s%s",address,mnemonic,operand1,operand2);	
	while(!feof(fpic))
	{
		if(strcmp(address,"*")!=0)
		{
			if(mnemonic[0]!='=')
			{	
				//split the string into different tokens
					//strtok parse the string into a sequence of tokens...if no more tokens are found then it returns NULL...and the string does not include delimitong character...
				
				//for mnemonic
				strcpy(str1_mne,strtok(mnemonic,",")); 
 				strcpy(str2_mne,strtok(NULL,",")); 
				strcpy(str3_mne,strtok(str2_mne,")")); 
				
				
				//for operand1
				strcpy(str1_ope1,strtok(operand1,","));
				strcpy(str2_ope1,strtok(NULL,","));
				strcpy(str3_ope1,strtok(str2_ope1,")"));
				
				
				//for operand2	
				strcpy(str1_ope2,strtok(operand2,","));
				strcpy(str2_ope2,strtok(NULL,","));
				strcpy(str3_ope2,strtok(str2_ope2,")"));
				

				
				//now find the address of the symbol or literal from the literal table
				//for operand1
				if(str1_ope1[1]=='S') //bcoz 1st character is 'S'
				{
					add1=returnsymnameaddress(str3_ope1);
					sprintf(str3_ope1,"%d",add1);
				}//finding address of the symbol from the symbol table		
				else if(str1_ope1[1]=='L') //bcoz ist character is 'L'
				{
					add1=returnliteraladdress(str3_ope1);
					sprintf(str3_ope1,"%d",add1);
				}//finding address of the literal from the literal table
			
				
				
				//for operand2
				if(str1_ope2[1]=='S') //bcoz 1st character is 'S'
				{
					add2=returnsymnameaddress(str3_ope2);
					sprintf(str3_ope2,"%d",add2);
				}//finding address of the symbol from the symbol table	
				else if(str1_ope2[1]=='L')  //bcoz ist character is 'L'
				{
					add2=returnliteraladdress(str3_ope2);
					sprintf(str3_ope2,"%d",add2);
				}//finding address of the literal from the literal table
				
				
				else if(str1_mne[1]=='D')
				{
					strcpy(str3_mne,"00"); //if it is a declarative statement then its opcode is "00"				
				}//if it is a declarative statement			
				
	
				fprintf(fptc,"%s\t%s\t%s\t%s\n",address,str3_mne,str3_ope1,str3_ope2);

			}//if it is not a literal	
			else
			{	 
				strcpy(str3_ope1,"-");
				strcpy(str3_ope2,"-");
				fprintf(fptc,"%s\t%s\t%s\t%s\n",address,mnemonic,str3_ope1,str3_ope2);
			}//else
		}//if it is not assembler directives then only include it in the target code


		fscanf(fpic,"%s%s%s%s",address,mnemonic,operand1,operand2);	
	
	}//open ic file and read till the end of file

	fclose(fptc);
	fclose(fpic);
	fclose(fpsym);
	fclose(fpltr);

printf("\n...Pass2 Completed...\n");
}//main



