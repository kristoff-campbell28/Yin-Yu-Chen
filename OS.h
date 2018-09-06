/* Name: <Yin Yu Chen>
   Course: 34000 section 3
   Assignment: <Operation system>

*/
#ifndef CSCI340_OS
#define CSCI340_OS

#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <vector>
using namespace std;
struct D_info{
	int id;
	string file;
	int level;
	int time_quantum;
}info, CPU, process;


class OS{
 public:
 	// set up disk and MEM
	void Set(int mem, int page_size, int hard_disk){
 		time = 1;
 		count = 0;
		RAM = mem; 
		Frame = page_size;
		disk_size = hard_disk;
		PG_size = mem / page_size;
		queue<D_info> que;
		for(int i=0; i<disk_size; i++){
			Disk.push_back(que);
		}
	}
	
	void Preemptive(){
		if(CPU.level == 1){
			level1.push(CPU);
			while(level1.front().id != CPU.id){
				level1.push(level1.front());
				level1.pop();
			}
		}
		if(CPU.level == 2){
			level2.push(CPU);
			while(level2.front().id != CPU.id){
				level2.push(level2.front());
				level2.pop();
			}		
		}
	}
	
	void Creat_process(){
		process.id++;
		level0.push(process);
		if(CPU.id == 0){
			CPU = level0.front();
			level0.pop();
		}
		if(CPU.level > 0){  //check preemptive
			Preemptive();
			Scheduling();
		}
		
		if(count < PG_size){ // check MEM size
			Page_Table(process.id, 0);
			count ++;
		}
		else{
			LRU(0, process.id); //if MEM size full LRU
		}  
	}

	void Page_Table(int cpu, int page_num){ // MEM
		vector<int> vec;
		vec.push_back(page_num);
		vec.push_back(cpu);
		vec.push_back(time);
		time++;
		PG.push_back(vec);
		}

	void Memory(int address){ 
		int page_num = address / Frame;
		bool same_PG = false;
		for(unsigned int i = 0; i< PG.size(); i++){
			if(PG[i][0] == page_num && PG[i][1] == CPU.id){  // same page num updat time
				PG[i][2] = time;
				time++; 
				same_PG = true;
			}
		}
		if(!same_PG){  
			if(count < PG_size){ // not same page num and the MEM is no full
				Page_Table(CPU.id, page_num);
				count++;
			}
			else{	// MEM is full go LRU
				LRU(page_num, CPU.id);
			}
		}
	}

	void RR(){
		if(CPU.level == 0){ 
			level1.push(CPU);
			Scheduling();
		}
		else if(CPU.level == 1 && CPU.time_quantum == 0){
			CPU.time_quantum++;
		}
		else if(CPU.level == 1 && CPU.time_quantum == 1){
			level2.push(CPU);
			Scheduling();
		}
	}

	void Scheduling(){
		if(!level0.empty()){
			CPU = level0.front();
			level0.pop();
			CPU.level = 0;
		}
		else if(!level1.empty()){
			CPU = level1.front();
			level1.pop();
			CPU.level = 1;
		}
		else if(!level2.empty()){
			CPU = level2.front();
			level2.pop();
			CPU.level = 2;
		}
		else{
			CPU.id = 0;
			CPU.level = 2;
		}
	}

	
	void LRU(int page_num, int process){
		int LRU_ = 0;
		// check LRU
		for(unsigned int i = 0; i<PG.size()-1; i++){
			if(PG[LRU_][2] > PG[i+1][2]){
				LRU_ = i+1;
			}
			
		}
		// store data to LRU
		PG[LRU_][0] = page_num;
		PG[LRU_][1] = process;
		PG[LRU_][2] = time;
		time++;
	}

	void Terminate(){
		for(unsigned int i = 0; i<PG.size(); i++){
			if(PG[i][1] == CPU.id){
				PG[i][0] = -1;
				PG[i][1] = -1;
				PG[i][2] = -1;
			} 
		}
		Scheduling();
	}

	void DiskIO(int num, string file_name){
		info.id = CPU.id;
		info.file = file_name;
		info.level = CPU.level;
		info.time_quantum = 0;
		Disk[num].push(info);
		Scheduling();
	}

	void Disk_Finish(int num){
		if(Disk[num].front().level == 0 && !Disk[num].empty()){
			level0.push(Disk[num].front());
			if(CPU.level > 0){
				Preemptive();
				Scheduling();
			}
		}
		
		if(Disk[num].front().level == 1 && !Disk[num].empty()){
			level1.push(Disk[num].front());
			if(CPU.level == 2){
				Preemptive();
				Scheduling();
			}
		}
		
		if(Disk[num].front().level == 2 && !Disk[num].empty()){
			level2.push(Disk[num].front());
			if(CPU.id == 0){
				CPU = level2.front();
				level2.pop();
			}
		}

		if(!Disk[num].empty()){
			Disk[num].pop();
		}
		
	}
		
	void Print_Memory(){
		for(unsigned int i = 0; i<PG.size(); i++){
			cout<<"Frame: "<<i<<" Page: "<<PG[i][0]<<" PID: "<<PG[i][1]<<" time: "<<PG[i][2]<<endl;
		}
	}

	void Print_Ready_Q(){
		if(CPU.id == 0){
			cout<<"No process is currently using the CPU"<<endl;
		}
		else{
			cout<<"CPU: PID "<<CPU.id<<" Level: "<<CPU.level<<endl;
			queue <D_info> Copy;
			Copy = level0;
			if(!Copy.empty()){
				cout<<"Ready-Queue Level 0: PID "<<Copy.front().id;
				Copy.pop();
			}
			else{
				cout<<"Ready-Queue Level 0: empty";
			}
			while(!Copy.empty()){
				cout<<" PID "<<Copy.front().id;
				Copy.pop();
			}
			cout<<endl;
			Copy = level1;
			if(!Copy.empty()){
				cout<<"Ready-Queue Level 1: PID "<<Copy.front().id;
				Copy.pop();
			}
			else{
				cout<<"Ready-Queue Level 1: empty";
			}
			while(!Copy.empty()){
				cout<<" PID "<<Copy.front().id;
				Copy.pop();
			}
			cout<<endl;
			Copy = level2;
			if(!Copy.empty()){
				cout<<"Ready-Queue Level 2: PID "<<Copy.front().id;
				Copy.pop();
			}
			else{
				cout<<"Ready-Queue Level 2: empty";
			}
			while(!Copy.empty()){
				cout<<" PID "<<Copy.front().id;
				Copy.pop();
			}
			cout<<endl;
		}
	}
	
	void Print_Disk_IO(){
		vector<queue<D_info> > Copy;
		Copy = Disk;
		for(unsigned int i = 0; i<Copy.size(); i++){
			if(!Copy[i].empty()){
				cout<<"Disk "<<i<<":"<<Copy[i].front().id<<" "<<Copy[i].front().file<<endl;
				Copy[i].pop();
				if(!Copy[i].empty()){
					cout<<"Disk "<<i<<" I/O Queue: "<<endl;
				}
				while(!Copy[i].empty()){
					cout<<Copy[i].front().id<<" read/write file "<<Copy[i].front().file<<endl;
					Copy[i].pop();
				}
			} 
			else{
				cout<<"Disk "<<i<<": is empty"<<endl;
			}
		}
	}
								
 private:
 	int disk_size;
 	int RAM;
 	int Frame;
 	int PG_size;
	queue <D_info> level0;
	queue <D_info> level1;
	queue <D_info> level2;
	int time;
	int count;
	vector<vector<int> > PG;
	vector<queue<D_info> > Disk;
};
#endif // CSCI340_OS

