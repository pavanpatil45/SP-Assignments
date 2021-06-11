#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

int ustsr=1;
int idsr=1;
int ltrsr=1;

typedef struct ust
{
int srno;
char name[50];
char token[50];
int index;
}ust;

typedef struct idtable
{
int srno;
char name[50];
}idtable;

typedef struct ltrtable
{
int srno;
char name[50];
char base[50];
char scale[50];
int precision;
}ltrtable;

typedef struct tertable
{
int srno;
char terminal[50];
char indicator[50];
}tertable;



void writedatatoust(ust t1)
{
	FILE *fpust;
	fpust=fopen("ust.txt","a");
	fprintf(fpust,"%d\t\t%-10s\t\t%10s\t\t%d\n",t1.srno,t1.name,t1.token,t1.index);
	fclose(fpust);
}//writing data into the uniform symbol table



int isidalreadypresent(char key[30])
{
	FILE *fpid;
	int sr;
	char name[30];
	int i=1;
		fpid=fopen("idtable.txt","r");
		while(!feof(fpid))
		{			
			fscanf(fpid,"%d %s ",&sr,name);
			if(strcmp(key,name)==0)
			{
				return i;
			}
			i++;
		}
		fclose(fpid);
	return 0 ;
}//searchin for the sr no of the present identifier



void writedatatoltrtable(ltrtable t1)
{
	FILE *fpltr;
	fpltr=fopen("literaltable.txt","a");
	fprintf(fpltr,"%d\t\t%-10s\t\t%10s\t\t%10s\t\t%d\n",t1.srno,t1.name,t1.base,t1.scale,t1.precision);
	fclose(fpltr);
}//writing data into the literal table



tertable checkfortokenintertable(char key[30])
{
	tertable t1;
	FILE *fpter;
	int sr;
	char terminal[30];
	char indicator[30];
		
		t1.srno=-1;
		fpter=fopen("terminaltable.txt","r");
		while(!feof(fpter))
		{
			fscanf(fpter,"%d %s %s",&sr,terminal,indicator);
			if(strcmp(key,terminal)==0)
			{
				t1.srno=sr;
				strcpy(t1.terminal,terminal);
				strcpy(t1.indicator,indicator);
				return t1;
			}		//fscanf(f,"%d %s %s",&sr,terminal,indicator);
		}
	fclose(fpter);
return t1 ;	
}//check for token is present in the terminal table or not



void writedatatoidtable(idtable t1)
{
	FILE *fpid;
	fpid=fopen("idtable.txt","a");
	fprintf(fpid,"%d\t\t%-10s	\n",t1.srno,t1.name);
fclose(fpid);
}//writing data into the identifier table




int main()
{
		
	FILE *f;

	tertable t1;
	ust ust1;
	idtable id;
	ltrtable ltr;
	
	f=fopen("ust.txt","w");
	fclose(f);
	f=fopen("literaltable.txt","w");
	fclose(f);	
	f=fopen("idtable.txt","w");
	fclose(f);
	
	int flag=0,ltr1,storeltr,i=0,fentry=0,prev_char=0;
	char oldchar='\0',newchar='\0',ch;
	char temp[2],token[50],token1[2];

	FILE *fpsrc,*fpltr,*fpid,*fpust;

	fpsrc=fopen("cprg.txt","r");
	if(fpsrc==NULL)
	{	
		printf("Error in Opening Source File");
		return 0;
	}

	while((ch=fgetc(fpsrc))!=EOF) //read complete file
	{
		if(ch=='"')	// If String Found
		{
			temp[0]='"';
			temp[1]='\0';
			ust1.srno=ustsr++;
			strcpy(ust1.name,temp);
			strcpy(ust1.token,"TRM");
			t1=checkfortokenintertable(temp);
			ust1.index=t1.srno;
			writedatatoust(ust1);
						
			while(1) 
			{
				ch=fgetc(fpsrc);
				if(ch=='"')
				{
					ch=fgetc(fpsrc);
					token[i]='\0';
					break;
				}
				else
					token[i++]=ch;
						
			}		//read Complete String
			i=0;
					
					
			if(strstr(token,"%d")==NULL )
					{
					ltr.srno=ltrsr++;
					strcpy(ltr.name,token);
					strcpy(ltr.base,"String");
					strcpy(ltr.scale,"FIXED");
					ltr.precision=strlen(token);
					writedatatoltrtable(ltr);;
					
						ust1.srno=ustsr++;
						strcpy(ust1.name,token);
						strcpy(ust1.token,"LIT");
						t1=checkfortokenintertable(temp);
						ust1.index=t1.srno;
							
						writedatatoust(ust1);				
					}//If %d Occured in String Then Avoid The String
					
							strcpy(ust1.name,temp);
							strcpy(ust1.token,"TRM");
							ust1.index=isidalreadypresent(temp);
							writedatatoust(ust1);
					
				} 	// For Strings
				else if(isdigit(ch))
				{
					while(isdigit(ch)) 
					{
						if(ch==' ')
						{
							token[i]='\0';
							break;
						}
						else
						token[i++]=ch;
						ch=fgetc(fpsrc);
						
					}
						i=0;
					
					ltr.srno=ltrsr++;
					strcpy(ltr.name,token);
					strcpy(ltr.base,"Decimal");
					strcpy(ltr.scale,"FIXED");
					ltr.precision=strlen(token);
					writedatatoltrtable(ltr);;
					
						ust1.srno=ustsr++;
						strcpy(ust1.name,token);
						strcpy(ust1.token,"LIT");
						ust1.index=ltr.srno;
						writedatatoust(ust1);				
				} 	// For Constants
				
	
			if(isalnum(ch))
			{
				if(flag==1)
				{	
					token[i++]=ch;
					token[i]='\0'; //terminate token
					i=0;  	    //again read token
					temp[0]=ch;
					temp[1]='\0';
					flag==0;
					ust1.srno=ustsr++;
					strcpy(ust1.name,token);
					strcpy(ust1.token,"IDT");
					ust1.index=isidalreadypresent(temp);
					writedatatoust(ust1);				
				}
				else
					token[i++]=ch;	
			}
			else if(ch==' ' || ch=='\n' || ch==',' || ch=='(' || ch ==')' || ch==';' )
			{	
							
				token[i]='\0'; //terminate token
				i=0;  	    //again read token
				//printf("\nToken is:	%-10s",token);
	
				t1=checkfortokenintertable(token);
				if(t1.srno!=-1)
				{
					if(strcmp(t1.indicator,"N")==0)
					{
						ust1.srno=ustsr++;
						strcpy(ust1.name,token);
						strcpy(ust1.token,"KWT");
						ust1.index=t1.srno;
						writedatatoust(ust1);				
					}
					if(strcmp(t1.indicator,"Y")==0)
					{
						ust1.srno=ustsr++;
						strcpy(ust1.name,token);
						strcpy(ust1.token,"TRM");
						ust1.index=t1.srno;
						writedatatoust(ust1);
					}
				}	// If Found In Terminal Table Then Update UST
				else
				{
					if(strcmp(token," ")==1 && (fentry==1)) //check whether first entry in identifier happened or not
					{
						//printf("Identifier Search Result %d",isidalreadypresent(token));
						
						if(isidalreadypresent(token)==0)	//if identifier not found in idtable
						{	
							id.srno=idsr++;
							strcpy(id.name,token);
							writedatatoidtable(id);
						
							ust1.srno=ustsr++;
							strcpy(ust1.name,token);
							strcpy(ust1.token,"IDN");
							ust1.index=id.srno;
							writedatatoust(ust1);
						}
						else
						{
							ust1.srno=ustsr++;
							strcpy(ust1.name,token);
							strcpy(ust1.token,"IDN");
							ust1.index=isidalreadypresent(token);
							writedatatoust(ust1);
						}	
							
					}
					else if(strcmp(token," ")==1)
					{
						id.srno=idsr++;
						strcpy(id.name,token);
						//printf("First Entry");
						writedatatoidtable(id);
						fentry=1;
						
						ust1.srno=id.srno;
						strcpy(ust1.name,token);
						strcpy(ust1.token,"IDN");
						ust1.index=id.srno;
						writedatatoust(ust1);
							
						//printf("\nWritten %s",token);
					}
					
				} //Else IT is Identifier Update Indentifier Table 
				
				token[i++]=ch;	
				token[i]='\0'; //terminate token
				//printf("\nToken:%-10s",token);
				t1=checkfortokenintertable(token);
				if(t1.srno!=-1)
				{
					if(strcmp(t1.indicator,"Y")==0)
					{
						//printf("   ->Terminal");								
						ust1.srno=ustsr++;
						strcpy(ust1.name,token);
						strcpy(ust1.token,"TRM");
						ust1.index=t1.srno;
						writedatatoust(ust1);
					}			
				}
				i=0;		// This Is For Condtion operators inclusions in token else those were lost  	 
				
			}
			else		// Special Case Characters
			{
				if(ch=='&')
				{
					token[i++]=ch;	
					flag=1;
				}
				else if(ch=='+' || ch=='-')
				{	
					flag=0;
					token[i++]=ch;	
				}
				else
				{
					flag=0;
					token[i++]=ch;	
					token[i]='\0'; 
					i=0;
					t1=checkfortokenintertable(token);
					if(t1.srno!=-1)
					{
						if(strcmp(t1.indicator,"Y")==0)
						{
							ust1.srno=ustsr++;
							strcpy(ust1.name,token);
							strcpy(ust1.token,"TRM");
							ust1.index=t1.srno;
							writedatatoust(ust1);
						}	
					}
					
				}
					
								
			}
		
	}//whiile
return 0;
}//main

