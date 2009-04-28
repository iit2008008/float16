/*
 * FILE: prog.cpp
 * DESCRIPTION: "Demonstration of the float16 datatype"
 *
 * DFS Assignment - A new Float Datatype
 *
 * Specifications:-
 *
 *	Name: float16
 * 	Size: 16 bytes
 * 	Allocation: 
 * 		Byte 1: 	Sign of Mantissa
 * 		Byte 2-32: 	Mantissa
 * 		Byte 33: 	Sign of Exponent
 * 		Byte 34-128: 	Exponent
 *
 * 	e.g.- The number 123.45e30 is stored as - 
 * 		Mantissa = 12345
 * 		Exponent = 32
 *
 * Team Members: 
 *
 * iit2008001 - Malay Singh
 * iit2008002 - Anupam Srivastava
 * iit2008003 - Yagyadutt Mishra
 * iit2008004 - Shubham Gupta
 * iit2008005 - Anvay Srivastava
 * iit2008006 - Shailendra Upadhyaya
 * iit2008007 - Shubham Aggarwal
 * iit2008008 - Pranjal Kumar Singh
 * iit2008009 - Santak Dalai
 * iit2008010 - Sunny Shulka
 *
 *----------------------------------------------------------------------------------------------------------------------
 * Work Distribution :- 
 *
 * Storing Integration 		- Pranjal Kumar Singh (IIT2008008)
 * String Manipulation		- Anvay Srivastava (IIT2008005) and Pranjal Kumar Singh (IIT2008008)
 * Addition and Substraction 	- Anupam Srivastava (IIT2008002) and Shailendra Upadhyaya (IIT2008006) 
 * Multiplication 		- Sunny Shukla (IIT2008010), Santak Dalai(IIT2008009) and Yagyadutt Mishra(IIT2008003)
 * Division 			- Shubham Gupta (IIT2008004) and Shubham Aggarwal (IIT2008007)
 *----------------------------------------------------------------------------------------------------------------------
 */

#include <iostream>
#include "float16.h"

using namespace std;

int main()
{
	float16 ff1, ff2, ff3;
	string input;
	int ch=0;
	do {
		cout<<"----------------------------"<<endl;
		cout<<"1. Addition"<<endl;
		cout<<"2. Subtraction: "<<endl;
		cout<<"3. Multiplication: "<<endl;
		cout<<"4. Division: "<<endl;
		cout<<"5. Exit"<<endl;
		cout<<"----------------------------"<<endl;
		cout<<"Enter choice: ";
		cin>>ch;
		if(ch == 5) break;
		cout<<"Enter first number: ";
		cin>>ff1;
		cout<<"Enter second number: ";
		cin>>ff2;

		switch(ch) {
			case 1:
				ff3 = ff1 + ff2;
				break;
			case 2:
				ff3 = ff1 - ff2;
				break;
			case 3:
				ff3 = ff1 * ff2;
				break;
			case 4:
				ff3 = ff1 / ff2;
				break;
		}
		cout<<"Result: "<<ff3<<endl;
	}while(ch != 5);

	return EXIT_SUCCESS;
}
