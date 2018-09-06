/* Name: <Yin Yu Chen>
   Course: 34000 section 3
   Assignment: <Operation system>

*/
#include <OS.h>
#include <iostream>
#include <string>
using namespace std;
int main()
{	 
	long long int RAM;
	int page_size, hard_disk; 
	string input;
	cout<<"How much RAM memory is there on the simulated computer? Your program receives the number in bytes (no kilobytes or words). I can enter any number up to 4000000000 (4 billions) ";
	cin>>RAM;  
	cout<<"What is the size of a page/frame";   
	cin>>page_size; 
	cout<<"How many hard disks does the simulated computer have";
	cin>>hard_disk; 
	OS test;  
	test.Set(RAM, page_size, hard_disk);
	cout<<"Please enter inputs. Enter 'F' for exiting the program";
	getline(cin, input);
	while(input != "F"){ 
		stringstream convert(input);
		char instruction;
		convert >> instruction; 
		if(instruction == 'A'){  
			test.Creat_process();  
		} 
		if(instruction == 'Q'){ 
			test.RR();
		} 
		if(instruction == 't'){  
			test.Terminate(); 
		} 
		if(instruction == 'd'){ 
			int disk_num;
			string file;
			convert >> disk_num;
			convert >> file;
			if(disk_num < hard_disk){
				test.DiskIO(disk_num, file); 
			}
			else{ 
				cout<<"there is no disk "<<disk_num<<endl;
			}
		}  
		if(instruction == 'D'){  
			int disk_num; 
			convert >> disk_num; 
			test.Disk_Finish(disk_num);   
		} 
		if(instruction == 'm'){  
			int address; 
			convert >> address;
			test.Memory(address);
		}
		if(instruction == 'S'){
			char part2;
			convert >> part2;
			if(part2 == 'r'){
				test.Print_Ready_Q();
			}
			if(part2 == 'i'){
				test.Print_Disk_IO();
			}
			if(part2 == 'm'){
				test.Print_Memory();
			}
		}

		cout<<"Please enter new inputs. Enter 'F' for exiting the program"<<endl;
		getline(cin, input);
		
	}
}
