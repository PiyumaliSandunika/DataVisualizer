
	// Project Name 	: Meeting Data Visualizer
  	// Author 			: Piyumali Sandunika

	// This project is to analyze multiple data files (.csv) and visually represent the information as the user may require
	// usage:
	// ./executedFileName [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

//User defined data type- zoomData
typedef struct Node {
	
  char name[80];			//for name of the person
  int indexNum;				//this index for tracing the order of entries in a given csv file when reading
  int mNum;					//how many number of meetings have for a particular person
  int participants;			//sum of participants in those meetings for a particular person
  int total_time;			//total time of meetings for a particular person
  float x1nonscaled;		//this variable is first used for collect hours, then for collecting nonscaled values
  float x2scaled;			//this variable is first used for collect minutes, then for collecting scaled values
  struct Node*next;			//pointer to the next node 
  
  } zoomData;

zoomData *head = NULL;		//the pointer which points to the first node of the linked list

//funcction prototyping

int readFile(char*file);				//funtion to read data from csv file and remove the repeated items
int length();							//function to get the no of nodes in the linked list
void sortByChoice(int ,char);			//for sort the list in descending order as given choice m,p or t 
void sortByIndex(int,char );			//function to sort the list according to index number (ascending order)
void drawGraph(int,int,int,int,int);	//funtion to draw the graph
void maxLenName(int ,int,int,int);		//to find the length of the name which has the max length
void scaledData(int,int,int);			//function to calculate the scaled size
void nonscaledMeet(int,int,int,int);	//function to calculate the nonscaled size
void printingFormat1(int,char*);		//for printing purposes in the graph
void printingFormat2(int,char*,int,char*);

//variables to get the total sum which needs in nonscaled calculations
long long meetingCount=0,participantCount=0,timeCount=0;
int csvCount=0;
int main(int argc, char **argv){
	
	int csvavailable=0,mavailable=0,pavailable=0,tavailable=0,savailable=0,lavailable=0;   //to detect the arguments given
	int givenSize=10;																	   //default no of the bars printed in the graph
	int currentLength=0;																   //number of nodes in the linked list
	int scal=0;																				//if --scaled is given --> scal=1 otherwise 0
	
	//number of arguments in the command line is 1
	if(argc==1){
		printf("No input files were given\n");
		printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
		return 1;
	}
	
	//number of arguments in the command line is 2
	//it check whether there's .csv files 
	//if there any it reads files
	//there's a flag 'found' if it is 1 other functions are also executed
	
	else if(argc==2){
		
		int found = 0;
		int strOf;
		strOf = strlen(argv[1]);
			if(((argv[1][strOf-1])=='v')&&((argv[1][strOf-2])=='s')&&((argv[1][strOf-3])=='c')&&((argv[1][strOf-4])=='.'))
			{	
				found=1;
				csvCount++;
				if(argv[1][0]=='-'){
				printf("Invalid option [%s]\n",argv[1]);
				printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
				return 1;
				}
				else if(readFile(argv[1])==1){
					return 1;}
			}
	
		
		if (found!=0){
			
			currentLength=length();
			sortByChoice(currentLength,'m');
			sortByIndex(currentLength,'m');
			maxLenName(givenSize,currentLength,1,scal);
			
		}
		else{
			printf("Only .csv files should be given as inputs.\n");
			return 1;
		}
		
	}
	
	//number of arguments in the command line is 3
	 
	else if (argc>=3){
		
		int strOf,dd=0;
		
		for (int i=1;i<argc;i++){
			
			strOf = strlen(argv[i]);
			//check whether csv files has been given or not
			//and checking for what are the arguments are given 
			//get the given no of bar and stored into givenInfo
			if(strcmp(argv[i],"-l")==0){
				lavailable=1;
				if(i==argc-1)
					lavailable=3;
				else if((strcmp(argv[i+1],"0")==0)||(strcmp(argv[i+1],"-0")==0))
					return 1;
        
				else if(atoi(argv[i+1])>0){
					lavailable =2;
					
					givenSize  = atoi(argv[i+1]);
				}
				else if(atoi(argv[i+1])<0){
					printf("Invalid option(negative) for [-l]\n");
					printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
					return 1;
				}
				else{
					printf("Invalid options for [-l]\n");
					printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
					return 1;
				}
			}
			
			else if(strcmp(argv[i],"-m")==0)
				mavailable=1;
			else if(strcmp(argv[i],"-p")==0)
				pavailable=1;
			else if(strcmp(argv[i],"-t")==0)
				tavailable=1;
			else if(strcmp(argv[i],"--scaled")==0)
				savailable=1;
			
			else if(argv[i][0]=='-'){
				printf("Invalid option [%s]\n",argv[i]);
				printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
				return 1;
			}
			else {
				if(((argv[i][strOf-1])=='v')&&((argv[i][strOf-2])=='s')&&((argv[i][strOf-3])=='c')&&((argv[i][strOf-4])=='.')){
				csvavailable=1;
				csvCount++;}
				else{
					if(strcmp(argv[i-1],"-l")!=0)
						dd=i;
				}
			}
			
		}
		
		if((lavailable!=0)&&(dd!=0)&&(strcmp(argv[dd-1],"-l")!=0)){
			printf("Only .csv files should be given as inputs.\n");
			return 1;
		}
		if((lavailable==0)&&(dd!=0)){
			printf("Only .csv files should be given as inputs.\n");
			return 1;
		}
		if((lavailable==1)||(lavailable==3)){
			printf("Not enough options for [-l]\n");
			printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
			return 1;
		}
		//error handling 
		//if the given files are not csv files
		if(csvavailable==0){
			printf("Only .csv files should be given as inputs.\n");
			return 1;
		}
		
		//if there are -m, -p,-t are given together
		if(mavailable+pavailable+tavailable>1){
			printf("Cannot plot multiple parameters in same graph.\n");
			printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
			return 1;
		}
		
		//lavailable becomes 1 when it doesn't get any value 
		
		
		
		for (int i=1;i<argc;i++){
			
			strOf = strlen(argv[i]);
			if(((argv[i][strOf-1])=='v')&&((argv[i][strOf-2])=='s')&&((argv[i][strOf-3])=='c')&&((argv[i][strOf-4])=='.'))
			{	
			
				if(argv[i][0]=='-'){
					printf("Invalid option [%s]\n",argv[i]);
					printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
					return 1;}
				else if(readFile(argv[i])==1){
					return 1;}
				
			}
		
		}
		
		//to get the scaled graph this use as a flag
		if(savailable==1){
			scal=1;
		}
		
		//when '-m' is given 
		if(mavailable==1){
			
			currentLength = length();
			sortByChoice(currentLength,'m');
			sortByIndex(currentLength,'m');
			maxLenName(givenSize,currentLength,1,scal);
			
			
		}
		//when '-p' is given
		else if(pavailable==1){
			currentLength = length();
			sortByChoice(currentLength,'p');
			sortByIndex(currentLength,'p');
			maxLenName(givenSize,currentLength,2,scal);
			
			
		}
		//when '-t' is given
		else if(tavailable==1){
			currentLength = length();
			sortByChoice(currentLength,'t');
			sortByIndex(currentLength,'t');
			maxLenName(givenSize,currentLength,3,scal);
			
		}
		//neither '-m' '-p' '-t' given it acts in default mood
		else{
			currentLength = length();
			sortByChoice(currentLength,'m');
			sortByIndex(currentLength,'m');
			maxLenName(givenSize,currentLength,1,scal);
			
		}

	}
}
	

//funtion to read the data from csv file and make nodes to store them and to remove the repeated data
int readFile(char*file){
	
	int len_original=0;
	
	char line[100];
	FILE *data;											
	data = fopen(file, "r");			//open file for reading
	
	if(!data)
	{
		printf("Cannot open one or more given files\n");		
		return 1;
	}
	fseek(data,0,SEEK_END);					//moving file pointer to end of the file
	if((ftell(data)==0)&&(csvCount==1)){
		printf("No data to process\n");
		return 1;}
	fseek(data,0,SEEK_SET);		//moving back file pointer to the begining of the file
	
	zoomData *entry,*ptr,*ptr2;
	entry = (zoomData *)malloc(sizeof(zoomData));  //allocating memory for pointers
	ptr=(zoomData*)malloc(sizeof(zoomData));
	ptr2=(zoomData*)malloc(sizeof(zoomData));
	
	
	if(data){
		
		while(fgets(line,100,data)!=NULL){   
			
			zoomData *entryNew = (zoomData *)malloc(sizeof(zoomData));
			
			/*getting data separated by comma ,unless strtok returns NULL pointer and add them to relevant elements in the structure
			by string copying and convert them to float values using atof because x1 nonscaled x2 scaled are float for future calculations*/
			
			char*val0= strtok(line,",");	
      if(val0!=NULL)
				{strcpy(entryNew->name,val0);}
		  
			char*val1= strtok(NULL,",");
			if(val1!=NULL)
				{entryNew->participants = atoi(val1);}
		   
			char*val2 = strtok(NULL,":");
			if(val2!=NULL)
				{ entryNew->x1nonscaled = atof(val2);}
		
			char*val3 = strtok(NULL,":");
			if(val3!=NULL)
				{entryNew->x2scaled = atof(val3);}
		  
			
			entryNew-> total_time = (int)entryNew-> x2scaled + (int)entryNew-> x1nonscaled*60 ;
			entryNew -> mNum =1 ;
			entryNew ->indexNum = 1 + len_original++ ;
			meetingCount+=entryNew->mNum;
			participantCount+=entryNew->participants;
			timeCount+=entryNew->total_time;
			
			if(head==NULL){
			  head = entryNew;
			}
			else{
			  entry= head;
			  head = entryNew;
			  entryNew->next = entry;
			}
			if(head->next!=NULL){
			
			ptr=head;
			ptr2=ptr->next;
			
			while(ptr2!=NULL){
				
				/*here if the name of the new entry is already exists it add current values of participants,time and increase the
				no of meeting.Then the new entry will be deleted*/
				
				if(strcmp(ptr->name,ptr2->name)==0){
					ptr2->participants+=ptr->participants;
					ptr2->total_time+=ptr->total_time;
					ptr2->mNum++;
					
					head = ptr ->next;
					ptr->next=NULL;
					free(ptr);
					break;
				}
				
				ptr2 = ptr2->next;
			}
			
			}

		}
	}
	
	
    fclose(data);
	return 0;
}

//here as user input,(-p,-m or -t) items of the linked lists will be sorted in descending order usinfg bubble sort*/

void sortByChoice(int length, char choice){  
  
	zoomData**startFrom,*ptr1,*ptr2,*ptr3;
	int i,j,swapFlag;
	for(i=0;i<=length;i++){
		startFrom = &head;
		swapFlag=0;
		
		if(choice=='m'){
			for(j=0;j<length-i-1;j++){
				ptr1= *startFrom;
				ptr2=ptr1->next;
				if(ptr1->mNum < ptr2->mNum){	//condition for swapping nodes
					ptr3=ptr2->next;
					
					ptr2->next = ptr1;		
					ptr1->next= ptr3;
					*startFrom = ptr2;
					swapFlag=1;   //this is to indicate that there's a swap happened in atleast one stage.
				}
				startFrom = &(*startFrom)->next;
			}
		}
		else if(choice=='p'){
			for(j=0;j<length-i-1;j++){
				ptr1= *startFrom;
				ptr2=ptr1->next;
				if(ptr1->participants < ptr2->participants){
					ptr3=ptr2->next;
					
					ptr2->next = ptr1;
					ptr1->next= ptr3;
					*startFrom = ptr2;
					swapFlag=1;
				}
				startFrom = &(*startFrom)->next;
			}
		}
		else if(choice=='t'){
			for(j=0;j<length-i-1;j++){
				ptr1= *startFrom;
				ptr2=ptr1->next;
				if(ptr1->total_time < ptr2->total_time){
					ptr3=ptr2->next;
					
					ptr2->next = ptr1;
					ptr1->next= ptr3;
					*startFrom = ptr2;
					swapFlag=1;
				}
				startFrom = &(*startFrom)->next;
			}
		}
		
		if(swapFlag==0)  //if there's no swap has done in any stage no need to check for order anymore.
			break;
	}
  
}

//to get the count of nodes in the linked list
int length(){   

  int count =0;
  zoomData *temp =head;

  while(temp!=NULL){
    count++;
    temp = temp -> next;
  }
  
  return count;
  }

//this is to sort the list such that first entries to print first

void sortByIndex(int length , char choice){   

  zoomData**startFrom,*ptr1,*ptr2,*ptr3;
	int i,j,swapFlag;
	for(i=0;i<=length;i++){
		startFrom = &head;
		swapFlag=0;
		
		if(choice=='m'){
			for(j=0;j<length-i-1;j++){
				ptr1= *startFrom;
				ptr2=ptr1->next;
				//when we found entrieswith same count of meeting number we need to check for their index and sort using bubble sort
				if((ptr1->mNum == ptr2->mNum)&&(ptr1->indexNum >ptr2->indexNum)){
					ptr3=ptr2->next;
					
					ptr2->next = ptr1;
					ptr1->next= ptr3;
					*startFrom = ptr2;
					swapFlag=1;
				}
				startFrom = &(*startFrom)->next;
			}
		}
		else if(choice=='p'){
			for(j=0;j<length-i-1;j++){
				ptr1= *startFrom;
				ptr2=ptr1->next;
				if((ptr1->participants == ptr2->participants)&&(ptr1->indexNum >ptr2->indexNum)){
					ptr3=ptr2->next;
					
					ptr2->next = ptr1;
					ptr1->next= ptr3;
					*startFrom = ptr2;
					swapFlag=1;
				}
				startFrom = &(*startFrom)->next;
			}
		}
		else if((choice=='t')){
			for(j=0;j<length-i-1;j++){
				ptr1= *startFrom;
				ptr2=ptr1->next;
				if((ptr1->total_time == ptr2->total_time)&&(ptr1->indexNum >ptr2->indexNum)){
					ptr3=ptr2->next;
					
					ptr2->next = ptr1;
					ptr1->next= ptr3;
					*startFrom = ptr2;
					swapFlag=1;
				}
				startFrom = &(*startFrom)->next;
			}
      
		}
		if(swapFlag==0)
			break;
	}
  
}

//this is a function to get the name with maximum length in first n entries; n is bars to print
void maxLenName(int sample_size,int full_size,int flag,int scal){

  zoomData*ptr = head;
  int maxLen = 0;
  if (sample_size>full_size)
	  sample_size=full_size;
  for(int j=0;j<sample_size;j++){
    if(strlen(ptr->name)>maxLen){
      maxLen = strlen(ptr->name);
    }
    ptr = ptr->next;
  }
  //here it call s the function to draw the graph
  drawGraph(sample_size,full_size,maxLen,flag,scal);
}

//function to print the graph with given number of bars 
//full size : total nodes in the list , sample size:number of entries we want to print
//flag and scal to pass whether it need to print m,p or t or whether scaled data

void drawGraph(int sample_size,int full_size,int maxLen , int flag ,int scal){
  zoomData *ptr1= head,*ptr2=head;
  int division,predivision;
  int selected_size = sample_size*4;
  int var1=0,var2=0;

	//default mode id nonscale data. calling the  function to calculate the non scaled bar size
	if(scal==0)
	  nonscaledMeet(sample_size,full_size,maxLen,flag);
	//if used parsed '--scaled' it calling the function to calculate the scaled bar size
	else if(scal==1)
		scaledData(full_size,maxLen,flag);
  printf("\n");
	 for(int i=1;i<=(selected_size+1);i++){
		division = i/4;
		predivision = ((i-1)/4);
	//to recognize from where to change to print the  next entry
    if(division!=predivision){
      ptr1= ptr1->next;
    }
    for(int j=1;j<=80;j++){
      if((i==2+var1)&&(j==1)&&(i<(selected_size+1))){
        printingFormat2(1,ptr2->name,(maxLen-strlen(ptr2->name)+1),"\u2502");

        var1 = var1+4;
        ptr2 = ptr2->next;
      }
		//u2502 to construct the y axis
      if((j==1)&&(i<selected_size)&&(i%4==0)) {
        printingFormat1((2+maxLen),"\u2502");
      }
      if((i%4==0)&&(j>2)&&(i!=selected_size)) {
        printf(" ");
      }
	  //u2591 to shade the block and make bar visible
      if((scal==0)&&(j>2)&&(i%4!=0)&&(i<selected_size)&&(j<=(int)ptr1->x1nonscaled+2)){
        printf("\u2591");
      }
	  else if((scal==1)&&(j>2)&&(i%4!=0)&&(i<selected_size)&&(j<=(int)ptr1->x2scaled+2)){
        printf("\u2591");
      }
	  //to print the number of meetings at the end of the bars
      if((scal==0)&&(flag==1)&&(i==2+var2)&&(j==(int)ptr1->x1nonscaled+3)){
        printf("%d",ptr1->mNum);
        var2 = var2+4;
      }
	  else if((scal==1)&&(flag==1)&&(i==2+var2)&&(j==(int)ptr1->x2scaled+3)){
        printf("%d",ptr1->mNum);
        var2 = var2+4;
      }
	  //to print the number of participants at the end of the bars
      if((scal==0)&&(flag==2)&&(i==2+var2)&&(j==(int)ptr1->x1nonscaled+3)){
        printf("%d",ptr1->participants);
        var2 = var2+4;
      }
	  else if((scal==1)&&(flag==2)&&(i==2+var2)&&(j==(int)ptr1->x2scaled+3)){
        printf("%d",ptr1->participants);
        var2 = var2+4;
      }
	  //to print the time inseconds at the end of the bars
      if((scal==0)&&(flag==3)&&(i==2+var2)&&(j==(int)ptr1->x1nonscaled+3)){
        printf("%d",ptr1->total_time);
        var2 = var2+4;
      }
	  else if((scal==1)&&(flag==3)&&(i==2+var2)&&(j==(int)ptr1->x2scaled+3)){
        printf("%d",ptr1->total_time);
        var2 = var2+4;
      }
      if((i%2!=0)&&(j==1)&&(i<=selected_size)){
        printingFormat1((2+maxLen),"\u2502");
      }
      if((i==selected_size)&&(j==1)){
        printingFormat1((2+maxLen),"\u2502");
      }
      if((i==selected_size+1)&&(j==1)){
        printingFormat1((2+maxLen),"\u2514");
      }
	  //u2500 to construct the x axis
      if((i==selected_size+1)&&(j>1)&&(j<(80-maxLen-1))){
        printf("\u2500");
      }
    }
    printf("\n");
  }
  
}

//function to calculate the size of the bars in scaled version
//full horizontal space is 80 , 3 reduced for non printing areas and others
//after reducing the maximum length,and space for the number other space took by the maximum value bar
//other values are percentage of that
//calculated scaled values are added to x2scaled in each node which is previously used for collect the time values

void scaledData(int full_size,int length,int flag){
  zoomData *ptr = head;
  int digCount=0, value;
  
  if(flag==1){
    value = head->mNum;
    while(value!=0){
      value = value/10;
      digCount++;  }
      
    while(ptr!=NULL){
      ptr ->x2scaled = (float) ptr->mNum *(77.0-length-digCount)/head->mNum;
      ptr = ptr->next;
    }
  }
  else if(flag==2){
      value = head->participants;
      while(value!=0){
      value = value/10;
      digCount++;  }
    while(ptr!=NULL){
      ptr ->x2scaled = (float)ptr->participants *(77.0-length-digCount)/head->participants;
      ptr = ptr->next;
    }
  }
  else if(flag==3){
      value = head->total_time;
      while(value!=0){
      value = value/10;
      digCount++;  }
    while(ptr!=NULL){
      ptr ->x2scaled =(float) ptr->total_time *(77.0-length-digCount)/head->total_time;
      ptr = ptr->next;
    }
  }
}

//printing formats for the x axis of the graph
void printingFormat1(int indent,char*string){
  printf("%*s%s",indent,"",string);
}
//printing formats for the x axis and names of the graph
void printingFormat2(int indent,char*string,int indent2,char*string2){
  printf("%*s%s%*s%s",indent,"",string,indent2,"",string2);
}

//function to calculate the nonscaled values of each entry
//here values of each entry parameter is printed as a percentage of the total of that parameter
//(if total get the full length of the bar how much length will get by each entry in the graph)

void nonscaledMeet(int sample_size2,int full_size2,int len,int flag_passed){
	int digCount=0,value;
	zoomData*pointer2=head;

	if(flag_passed==1){
     value = head->mNum;
      while(value!=0){
      value = value/10;
      digCount++;  }
	for(int k=0;k<sample_size2;k++){  
		pointer2->x1nonscaled = ((float)(pointer2->mNum)*(77.0-len-digCount)) / meetingCount ;   
		pointer2 = pointer2->next;
		
	}
	}
	
	else if(flag_passed==2){
       value = head->participants;
      while(value!=0){
      value = value/10;
      digCount++;  }
	for(int k=0;k<sample_size2;k++){  
		pointer2->x1nonscaled = ((float)(pointer2->participants)*(77.0-len-digCount)) /participantCount ;  
		pointer2 = pointer2->next;
		
	}
	}
	else if(flag_passed==3){
     value = head->total_time;
      while(value!=0){
      value = value/10;
      digCount++;  }
	for(int k=0;k<sample_size2;k++){  
		pointer2->x1nonscaled = ((float)(pointer2->total_time)*(77.0-len-digCount)) / timeCount ;  
		pointer2 = pointer2->next;
		
	}
	}
	
}
