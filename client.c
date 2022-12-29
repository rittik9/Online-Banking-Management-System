//Rittik Panda
//MT2022090


#include <sys/types.h>
#include "sys/stat.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "unistd.h"
#include "stdio.h"
#include "fcntl.h"
#include "string.h"
#include "stdlib.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include "math.h"
#include "./structures/account.h"
#include "./structures/transactions.h"

account sendtoserver(account); //declared sendtoserver function of account type here and used later outside main

int main(int argc, char const *argv[])
{
	account acc; 
	strcpy(acc.action,"LOGIN") ; //default login (whenever we run the client.c code)
	
	printf("Enter usertype 1)ADMIN 2)NORMAL USER 3)JOINT USER : \n");
	scanf("%d",&acc.usertype); 
	
	
	//////////////////////////////////////////////////////// Admin ///////////////////////////////////////////////
 	if(acc.usertype == 1)
	{	
		printf("Enter password : \n");
		scanf(" %[^\n]",acc.password);
		
		acc = sendtoserver(acc); 
		
		if(acc.result==1) 
			printf("Login Success\n");
		else 
		{ 
			printf("Login Failure\n"); 
			exit(0); 
		} 

		printf("Enter 1)Create 2)Search 3)Modify 4)Delete \n"); 
		int choice; 
		scanf("%d",&choice); 
		
		switch(choice)
		{
			case 1 : 
				{
					account newacc; 
					newacc.active = 1; 
				
					printf("Enter user type 1.Normal 2.Joint \n");
					scanf("%d",&newacc.usertype); 
				
					newacc.usertype++ ; // 2.-> Normal User, 3.->Joint
				
					if(newacc.usertype == 2)
					{ 
						printf("Enter Name : \n"); 
						scanf(" %20[^\n]",newacc.name1); 
					}
					 
					else 
					{ 
						printf("Enter Name 1 and Name 2: \n"); 
						scanf(" %20[^\n]",newacc.name1); 
						scanf(" %20[^\n]",newacc.name2 ); 
					}

					printf("Enter new 5 digit account number : \n");
					scanf("%ld",&newacc.acc_no);
					
					if(newacc.acc_no/100000 > 0) 
					{
						printf("Exceeded 5 digits\n"); 
						exit(0); 
					}
					 
					printf("Enter password : \n");
					scanf(" %[^\n]",newacc.password);
					
					newacc.balance = 0; 
					strcpy(newacc.action,"CREATE"); 
					
					newacc = sendtoserver(newacc); 
					
					if(newacc.result == 1) 
						printf("Success\n");
						
					else 
						printf("Failure. Duplicate account number\n");
						
			}//end of case 1 (CREATE)
			break;

			case 2 :

				 {
				 	account newacc;
				 	printf("Enter account to search :\n");
				 	scanf("%ld",&newacc.acc_no); 
				 	strcpy(newacc.action,"SEARCH");  
				 	newacc = sendtoserver(newacc); 
				 	if(newacc.result == 1)
				 	{ 
				 		printf("Account balance : %0.2lf\n", newacc.balance); 
				 		if(newacc.usertype == 2)
				 		{ 
				 			printf("User type : Normal\n");
				 			printf("User : %s\n", newacc.name1);
				 	    } 
				 		if(newacc.usertype == 3)
				 		{
				 	   	 	printf("User type : Joint\n");  
				 	   	 	printf("Users : %s, %s\n", newacc.name1, newacc.name2); 
				 	   	} 
				 	} 
				 	
				 	else
				    	printf("No account found\n");
				  }//end of case 2 (SEARCH)
				 
			break; 
			case 3 : 
				{
					account newacc; 
					printf("Enter account to modify :\n");
				 	scanf("%ld",&newacc.acc_no);
				 	
				 	if(newacc.acc_no != 11111)
				 	{
						strcpy(newacc.action,"SEARCH"); 
						newacc = sendtoserver(newacc); 
						
						if(newacc.result == 1)
						{
							int choice; 
							
							if(newacc.usertype == 2)
							{ 	
								printf("Change account to joint ? 1) YES 2)NO \n"); 
								scanf("%d",&choice);
								
								if(choice == 1) 
								{
									printf("Enter second name :\n"); 
									scanf(" %20[^\n]",newacc.name2);
									newacc.usertype = 3; 
								}
							} 						 
 						
 							printf("Enter new password : \n"); 
 							scanf(" %20[^\n]",newacc.password); 
 							strcpy(newacc.action,"MODIFY"); 
 							newacc = sendtoserver(newacc); 
 							printf("Change Success\n"); 
						}
						else printf("Account not found \n");  
					}

					else
					{
						printf("Enter admin password : \n"); 
						scanf(" %[^\n]",newacc.password); 
						strcpy(newacc.action,"MODIFY");
						newacc = sendtoserver(newacc); 
						printf("Change Success\n"); 
					} 

				}//end of case 3(MODIFY)
			break; 
			case 4 :
				{
					account newacc; 
					printf("Enter account to delete :\n");
				 	scanf("%ld",&newacc.acc_no);
					strcpy(newacc.action,"SEARCH"); 
					newacc = sendtoserver(newacc); 
					if(newacc.result == 1)
					{
						strcpy(newacc.action,"DELETE"); 
						newacc.active = 0; 
						newacc = sendtoserver(newacc); 
						if(newacc.result == 1) 
							printf("Delete Success\n");
						else 
							printf("Delete Failed\n");
					}
					else 
					printf("Account not found\n");
				}//end of case 4(DELETE)
				
		}//end of switch 

	}//end of usertype->1 (ADMIN)
//////////////////////////////////////////////// Normal Users /////////////////////////////////////////
	else if(acc.usertype == 2)	{	

		
		printf("Enter account number :\n");
		scanf(" %ld",&acc.acc_no);
		printf("Enter password : \n");
		scanf(" %[^\n]",acc.password);
		acc = sendtoserver(acc);

		if(acc.result==1) printf("Login Success\n");
		else if(acc.result==0) { printf("Login Failure\n"); exit(0);} 
		else if(acc.result==-1) { printf("Account not found\n"); exit(0); } 

		strcpy(acc.action,"LOCK");
		acc = sendtoserver(acc); 
		if(acc.result==0) {printf("Simultaneous access. Try again later.\n"); exit(0); } 

		printf("Enter 1)Deposit 2)Withdraw 3)Balance  4)View Details 5)Change Password 6)Exit \n"); 
		int choice; 
		scanf("%d",&choice); 
		switch(choice)
		{
			case 1: 
			{
			      
		
				strcpy(acc.action,"DEPOSIT");
				printf("Enter deposit amount : \n");
				scanf("%lf",&acc.balance);
				acc = sendtoserver(acc); 
				printf("New Balance : %0.2lf\n",acc.balance);  

			}
			break; 
			case 2: 
			{
				strcpy(acc.action,"WITHDRAW"); 
				printf("Enter amount to withdraw : \n");
				scanf("%lf",&acc.balance); 
				acc = sendtoserver(acc); 
				if(acc.result == 1) printf("New balance : %0.2lf\n",acc.balance );
				else printf("Insufficient funds\n"); 
			}
			break; 
			case 3: 
			{
				strcpy(acc.action,"BALANCE"); 
				acc = sendtoserver(acc); 
				printf("Balance : %0.2lf\n", acc.balance);
			}
			break;

			case 4: 
			{
				strcpy(acc.action, "DETAILS"); 
				sendtoserver(acc); 
			}
			break; 
			case 5: 
			{
				strcpy(acc.action,"MODIFY"); 
				printf("Enter new password : \n");
				scanf(" %[^\n]",acc.password); 
				printf("Confirm password : \n");
				char buf[20]; 
				scanf(" %[^\n]",buf); 
				if(strcmp(acc.password,buf)!=0) { printf("Passwords do not match\n"); exit(0); }
				else acc = sendtoserver(acc);
				if(acc.result==1)printf("Password Change Success\n");
				else printf("Password change failed\n"); 
			}
			break; 
			default: break;

		}
		strcpy(acc.action,"UNLOCK");
		sendtoserver(acc); 

	}
	else if(acc.usertype == 3)	
	{	
	
	printf("Enter account number :\n");
		scanf(" %ld",&acc.acc_no);
		printf("Enter password : \n");
		scanf(" %[^\n]",acc.password);
		acc = sendtoserver(acc);

		if(acc.result==1) printf("Login Success\n");
		else if(acc.result==0) { printf("Login Failure\n"); exit(0);} 
		else if(acc.result==-1) { printf("Account not found\n"); exit(0); } 


		printf("Enter 1)Deposit 2)Withdraw 3)Balance  4)View Details 5)Change Password 6)Exit \n"); 
		int choice; 
		scanf("%d",&choice); 
		switch(choice)
		{
			case 1: 
			{
				strcpy(acc.action,"WLOCK");
		               acc = sendtoserver(acc); 
		               if(acc.result==0) 
		               {
		               printf("Simultaneous access. Try again later.\n"); 
		               exit(0); 
		               } 
				
				strcpy(acc.action,"DEPOSIT");
				printf("Enter deposit amount : \n");
				scanf("%lf",&acc.balance);
				acc = sendtoserver(acc); 
				printf("New Balance : %0.2lf\n",acc.balance);  
				
				strcpy(acc.action,"UNLOCK");
		               sendtoserver(acc); 

				
				

			}
			break; 
			case 2: 
			{
			       strcpy(acc.action,"WLOCK");
		               acc = sendtoserver(acc); 
				if(acc.result==0) 
		               {
		               printf("Simultaneous access. Try again later.\n"); 
		               exit(0); 
		               } 
			
				strcpy(acc.action,"WITHDRAW"); 
				printf("Enter amount to withdraw : \n");
				scanf("%lf",&acc.balance); 
				acc = sendtoserver(acc); 
				if(acc.result == 1) printf("New balance : %0.2lf\n",acc.balance );
				else printf("Insufficient funds\n"); 
				
				strcpy(acc.action,"UNLOCK");
		               sendtoserver(acc); 

				

			}
			break; 
			case 3: 
			{
			       strcpy(acc.action,"RLOCK");
		               acc = sendtoserver(acc); 
				if(acc.result==0) 
		               {
		               printf("Simultaneous access. Try again later.\n"); 
		               exit(0); 
		               } 
			
			
				strcpy(acc.action,"BALANCE"); 
				acc = sendtoserver(acc); 
				printf("Balance : %0.2lf\n", acc.balance);
				
				
				strcpy(acc.action,"UNLOCK");
		               sendtoserver(acc); 

			}
			break;

			case 4: 
			{
			       strcpy(acc.action,"RLOCK");
		               acc = sendtoserver(acc); 
			       if(acc.result==0) 
		               {
		               printf("Simultaneous access. Try again later.\n"); 
		               exit(0); 
		               } 
		               
		               
				strcpy(acc.action, "DETAILS"); 
				sendtoserver(acc); 
				
				
			       strcpy(acc.action,"UNLOCK");
		               sendtoserver(acc); 
			}
			break; 
			case 5: 
			{
			       strcpy(acc.action,"WLOCK");
		               acc = sendtoserver(acc); 
		               if(acc.result==0) 
		               {
		               printf("Simultaneous access. Try again later.\n"); 
		               exit(0); 
		               } 
			
				strcpy(acc.action,"MODIFY"); 
				printf("Enter new password : \n");
				scanf(" %[^\n]",acc.password); 
				printf("Confirm password : \n");
				char buf[20]; 
				scanf(" %[^\n]",buf); 
				if(strcmp(acc.password,buf)!=0) { printf("Passwords do not match\n"); exit(0); }
				else acc = sendtoserver(acc);
				if(acc.result==1)printf("Password Change Success\n");
				else printf("Password change failed\n"); 
				
				
			       strcpy(acc.action,"UNLOCK");
		               sendtoserver(acc); 
				
			}
			break; 
			default: break;

		}	
}
exit(0); 
}

account sendtoserver(account a)
{	
	struct sockaddr_in server; 
	int sd; ; 
	sd = socket(AF_UNIX, SOCK_STREAM, 0); 

	server.sin_family = AF_UNIX; 
	server.sin_addr.s_addr = INADDR_ANY; 
	server.sin_port = htons(7017); // <---------------------------------------- change port  

	connect(sd, (void *)(&server), sizeof(server));


	write(sd, &a, sizeof(a)); //this write will be read by server for the first checkup for action (login,create, deposit.....)
		
	read(sd, &a, sizeof(a)); //reads the value from server side and later returns to calling function

	if(strcmp(a.action,"DETAILS")==0) 
	{
		printf("Account number : %ld\n",a.acc_no );
		printf("Operation.................Amount.................Balance\n");
		printf("--------------------------------------------------------\n");
		trans t; 
		int size = read(sd, &t, sizeof(trans));	
		
		while(size!=0) 
		{
			//printf("%d\n",size );
			printf("%s.................%0.2lf.................%0.2lf \n",t.action,t.amount,t.balance);
			size = read(sd, &t, sizeof(trans));	
		}
	}
 
	
	return a;  
	
}
