#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <vector>
#include <sstream>
#include <string.h>

using namespace std;
/**
**Author: Mengling Ding
**Date:11/8/2020
**to have hands©\on experience of building a paging-based memory management system
** 
*/ 

const int framesize0 = 1;
const int allocSize = 1;
const int N = 100; 
const int M = 10;
vector <int> freeFrameList; //freeFrameList keeps track of free frames: simply a list of the frame numbers of free frames
int *memory = NULL;
string commands[3];
string input_command;
int alloSize;
int initSize;
//string com1, com2, com3;


struct process{
	int pid;//process id
	int size;//size of process
	int *pagetable;//pointer to pagetable
	
};
struct activeprocess{
	int pid;//active process id
	bool active;
	int size;//the size of active process
};
//processList keeps track of active processes (that are loaded in memory) 
//and their sizes in terms of the number of allocated pages
struct process processList[M]; 
struct activeprocess activeProcesslist[M];

void memoryManager(int memSize, int frameSize);
int allocate(int allocSize, int pid);
int deallocate (int pid);
int write (int pid, int logical_address);
int read (int pid, int logical_address);
void printMemory();
void inputcommand();

int main()
{
	inputcommand();
	//initialize the process and active process
	for(int i = 0; i < M; i++){
		processList[i].pid = i;
		processList[i].pagetable = (int *)malloc(sizeof(int)*M);
		activeProcesslist[i].size = 0;
		activeProcesslist[i].active = 0;
	}
	
	while(input_command != "exit")
	{
	if(commands[0]=="M")
	initSize = stoi(commands[1]);
	if(commands[0]=="A")
	alloSize = stoi(commands[1]);
	
	if(commands[0]=="M")
	memoryManager(stoi(commands[1]),framesize0);
	else if(commands[0]=="A")
	allocate(stoi(commands[1]), stoi(commands[2]));
	else if(commands[0]=="W")
	write(stoi(commands[1]), stoi(commands[2]));
	else if(commands[0]=="R")
	read(stoi(commands[1]), stoi(commands[2]));
	else if(commands[0]=="D")
	deallocate(stoi(commands[1]));
	else if(commands[0]=="P")
	printMemory();
	else
	    cout<<"Wrong enter! Please enter again. "<<endl;
	    inputcommand();
	}
	//freeFrameList[commands[1]];
    return 0;
}

//input the command and divide it by space
void inputcommand(){
	cout<<"Please enter the command, enter exit out:"<<endl;
	getline(cin, input_command);
	//use stringstream to divide the input command into three part by space
	istringstream is(input_command);
	is>>commands[0]>>commands[1]>>commands[2];
}

//Simulates creation and initialization of a physical memory space consisting of frames
void memoryManager(int memSize, int frameSize){ 
	//initializ memory space
	memory = (int *)malloc(sizeof(int)*memSize);
	//when initializ memory space successfully, can do the following things
	for(int i=0;i<(memSize/frameSize);i++){	
	    memory[i] = 0;
	}
	//////free(memSize);
	//memSize=NULL;
	//push element number to frame list
	for(int i=0;i<(memSize/frameSize);i++){
		freeFrameList.push_back(i);
	}
	cout<<"Create memory space consisting of "<< memSize << " free fames, Each frame will be initialized with 0."<<endl;
}

int allocate(int allocSize, int pid){
	srand (time(NULL));
	if(freeFrameList.size() >= allocSize){
	for(int i=0; i < allocSize; i++){
	    //randow to get the index from the free fram list,start from 0
	    int randomIndex = rand() % freeFrameList.size();
	    int randomelement = freeFrameList[randomIndex]; 
	    
	    //create page table;
		processList[pid].pagetable[i] =  randomelement;
		
		//update freefram list by iterator
		vector<int>::iterator iter;
	    iter = freeFrameList.begin();
		while( iter != freeFrameList.end() ){
			//freeFrameList.clear();
			//freeFrameList.push_back(*iter);
			iter++;
		}
	}
	//update active process list
	activeProcesslist[pid].size = allocSize;
	activeProcesslist[pid].active = 1;
	cout<<"Allocate " << allocSize << " pages to a process with process ID "<< pid<<endl;
	return 1;
	}
	else 
	//cout<<"Do not have enough space, allocate failed."<<endl;
	return -1; 
}


int deallocate (int pid){
	for(int i = 0; i < M; i++){
		if(processList[i].pid == pid){
			//after deallocate, free frame list get space back
			for(int j = 0; j < alloSize; j++ ){
			freeFrameList.push_back(processList[i].pagetable[j]);
			processList[i].pagetable[j] = 0;
			}
			//update process list, delete pagetable pointer
			delete processList[i].pagetable;
			///////???????
			 for(int l = 0; l < initSize; l++)
            {
               processList[i].pagetable[l] = 0;
            }
			//update active process list
			activeProcesslist[i].size = 0;
			activeProcesslist[i].active = 0;
		cout<<"Deallocates a memory space from a process with process ID "<<pid<<endl;
		return 1;
		}
	}
    cout<<"Haven't allocate space for this process, can't deallocate."<<endl;
	return -1;
}

int write (int pid, int logical_address){
	for(int i = 0; i < M; i++){
		if(processList[i].pid == pid){
			processList[i].pagetable[logical_address] = 1;
			cout<<"Write a value of 1 at the memory location specified by the page number " << logical_address<< " of a process with ID " <<pid<<endl;
			return 1;
		}
	}
	return -1;
}

int read (int pid, int logical_address){
	for(int i = 0; i < M; i++){
		if(processList[i].pid == pid){
			cout << "Read the data of process ID "<< pid <<endl;
			cout << processList[i].pagetable[logical_address]<<endl;
			return 1;
		}
	}
	cout<<"Nothing has been written into this process." <<endl;
	return -1;
}

void printMemory(){
	cout<<"The Free Frame List is: "<< endl;
	vector<int>::iterator ite;
	ite = freeFrameList.begin();
	while( ite != freeFrameList.end() ){
        cout << *ite << " ";
		ite++;
	}
	cout<<"\nThe Process List is: " <<endl;
	for(int i = 0; i < M; i++){
		if(activeProcesslist[i].active = 1){
			cout <<"The No. "<< i << " process' size is "<<activeProcesslist[i].size<<endl;
		}
	}
	//cout<<processList[];
}
