#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node{
	char phoneNumber[15];
	long long int asciiPhone;
	int numberVisit;
	char name[27];
	char email[22];
	struct node *parent;
	struct node *left;
	struct node *right;
};

struct medicine{
	char medName[20];
	long int price;
};

struct medNode{
	int medCode;
	int totalBought;
	struct medNode *next;
};

const struct medicine medList[5] = {
	{"Paracetamol", 5000},
	{"Diapet", 7000},
	{"Oralit", 3500},
	{"Paratusin", 5500},
	{"OB Herbal", 6000}
};

long long int countAscii(char *phoneNumber)
{
	int i=0, isFound = 0;
	long long int asciiTotal = 0;
	while(phoneNumber[i] != '\0')
	{
		asciiTotal += (long long int)phoneNumber[i] * i * 2;
		i++;
	}
	
	return asciiTotal;
}

struct node *searchPhoneNumber(char *phoneNumber, struct node **root, long int asciiPhone)
{
	struct node *ptr = *root;
	
	if(ptr != NULL)
	{
		while(1)
		{
			if(ptr == NULL)
			{
				return NULL;
			}
			else if(asciiPhone == ptr->asciiPhone)
			{
				if(strcmp(phoneNumber, ptr->phoneNumber)==0)
				{
					return ptr;
				}
				else if(strcmp(phoneNumber, ptr->phoneNumber)<0)
				{
					ptr = ptr->left;
				}
				else if(strcmp(phoneNumber, ptr->phoneNumber)>0)
				{
					ptr = ptr->right;
				}
			}
			else if(asciiPhone < ptr->asciiPhone)
			{
				ptr = ptr->left;
			}
			else if(asciiPhone > ptr->asciiPhone)
			{
				ptr = ptr->right;
			}
		}
		return ptr;
	}
	return NULL;
}

void addMember(struct node *newMember, struct node **root)
{
	if(*root == NULL)
	{
		*root = newMember;
		newMember->parent = NULL;
	}
	else
	{
		struct node *ptr = *root;
		
		if( ptr->asciiPhone > newMember->asciiPhone && ptr->left == NULL)
		{
			ptr->left = newMember;
			ptr->left->parent = ptr;
		}
		else if( ptr->asciiPhone > newMember->asciiPhone && ptr->left != NULL)
		{
			addMember(newMember, &ptr->left);
		}
		else if( ptr->asciiPhone < newMember->asciiPhone && ptr->right == NULL)
		{
			ptr->right = newMember;
			ptr->right->parent = ptr;
		}
		else if( ptr->asciiPhone < newMember->asciiPhone && ptr->right != NULL)
		{
			addMember(newMember, &ptr->right);
		}
	}
}

int validatePhoneNumber(char *phoneNumber)
{
	if(strlen(phoneNumber) < 10 || strlen(phoneNumber) > 13)
	{
		return 0;
	}
	
	int i=0;
	while(phoneNumber[i] != '\0')
	{
		if(phoneNumber[i] < 48 || phoneNumber[i] > 57)
		{
			return 0;
		}
		i++;
	}
	
	return 1;
}

int validateName(char *name)
{
	if(strlen(name)<5 || strlen(name)>25)
	{
		return 0;
	}
	
	int i,j;
	char beginWithMr[6];
	for(i=0,j=0;i<4;i++,j++)
	{
		beginWithMr[j] = name[i];
	}
	beginWithMr[j] = '\0';
	
	char beginWithMrs[7];
	for(i=0,j=0;i<5;i++,j++)
	{
		beginWithMrs[j] = name[i];
	}
	beginWithMrs[j] = '\0';
	
	if(strcmp(beginWithMr, "Mr. ") != 0 && strcmp(beginWithMrs, "Mrs. ") != 0)
	{
		return 0;
	}
	
	i=0;
	while(name[i] != '\0')
	{
		if( !(name[i]>=65 && name[i]<=90) && !(name[i]>=97 && name[i]<=122) && name[i]!='.' && name[i]!=' ' )
		{
			return 0;
		}
		i++;
	}
	
	return 1;
}

int validateEmail(char *email)
{
	if(strlen(email) < 10 || strlen(email) > 20)
	{
		return 0;
	}
	
	int i = 0, numberOfAt = 0, indexAt = 0;
	while(email[i] != '\0')
	{
		if(email[i] == '@' && numberOfAt > 0)
		{
			return 0;
		}
		else if(email[i] == '@' && email[i+1] != '.')
		{
			indexAt = i;
			numberOfAt++;
		}
		else if(email[i] == '@' && email[i+1] == '.')
		{
			return 0;
		}
		i++;
	}
	
	i=0;
	while(email[i] != '\0')
	{
		if(email[i] == ' ' || (email[i]>=65 && email[i]<=90) )
		{
			return 0;
		}
		i++;
	}
	
	char endWithCom[6];
	int j;
	for(i=strlen(email)-4,j=0;i<strlen(email);i++,j++)
	{
		endWithCom[j] = email[i];
	}
	endWithCom[j] = '\0';
	
	char endWithCoId[8];
	for(i=strlen(email)-6,j=0;i<strlen(email);i++,j++)
	{
		endWithCoId[j] = email[i];
	}
	endWithCoId[j] = '\0';
	
	if(strcmp(endWithCom, ".com") != 0 && strcmp(endWithCoId, ".co.id") != 0)
	{
		return 0;
	}
	
	i=0;
	int amountOfDot = 0;
	while(email[i] != '\0')
	{
		if(email[i] == '.') amountOfDot++;
		i++;
	}
	
	if(amountOfDot > 1 && strcmp(endWithCoId, ".co.id")!=0 )
	{
		return 0;
	}
	
	return 1;
}

void processOrder(struct node **root)
{
	char inputNumber[15];
	int validPhoneNumber = 0;
	
	do{
		printf("Input phone number[10-13][numeric]: ");
		fflush(stdin);
		scanf("%s", inputNumber);
		validPhoneNumber = validatePhoneNumber(inputNumber);
	}while(validPhoneNumber != 1);
	
	long long int asciiPhone = countAscii(inputNumber);
	struct node *isFound = searchPhoneNumber(inputNumber, root, asciiPhone);
	
	if(isFound == NULL)
	{
		char isJoinMember;
		do{
			printf("Do you want to join membership?[y/n]: ");
			fflush(stdin);
			scanf("%c", &isJoinMember);
		}while(isJoinMember != 'y' && isJoinMember != 'n');
		
		if(isJoinMember == 'y')
		{
			struct node *newMember = (struct node*)malloc(sizeof(struct node));
			newMember->left = NULL;
			newMember->right = NULL;
			newMember->asciiPhone = asciiPhone;
			
			int validName = 0, validEmail = 0;
			strcpy(newMember->phoneNumber, inputNumber);
			
			do{
				printf("Input name[5-25][Mr. | Mrs. ]: ");
				fflush(stdin);
				scanf("%[^\n]", newMember->name);
				validName = validateName(newMember->name);
			}while(validName != 1);
			
			do{
				printf("Input email[10-20][email format]: ");
				fflush(stdin);
				scanf("%[^\n]", newMember->email);
				validEmail = validateEmail(newMember->email);
			}while(validEmail != 1);
			
			newMember->numberVisit = 1;
			addMember(newMember, root);
			
			printf("\nInsert Success !\n\n");
			printf("Number of visits: %d\n", newMember->numberVisit);
		}
	}
	else
	{
		isFound->numberVisit++;
	}
	
	int i;
	
	for(i=0;i<34;i++) printf("-");
	printf("\n");
	
	printf("| %-3s | %-15s | %-6s |\n", "No", "Nama Obat", "Harga");
	
	for(i=0;i<34;i++) printf("-");
	printf("\n");
	
	for(i=0;i<5;i++)
	{
		printf("| %-3d | %-15s | %-6ld |\n", i+1, medList[i].medName, medList[i].price);
	}
	
	for(i=0;i<34;i++) printf("-");
	printf("\n");
	
	struct medNode *headMed = NULL;
	char stillWantToBuy = ' ';
	long int totalBill = 0;
	do{
		struct medNode *newMed = calloc(sizeof(struct medicine), 1);
		newMed->next = NULL;
		newMed->medCode = 0;
		newMed->totalBought = 0;
		
		do{
			printf("Input medicine[1-5]: ");
			fflush(stdin);
			scanf("%d", &newMed->medCode);
		}while(newMed->medCode < 1 || newMed->medCode > 5);
		
		newMed->medCode--;
		
		do{
			printf("Input quantity[>=1]: ");
			fflush(stdin);
			scanf("%d", &newMed->totalBought);
		}while(newMed->totalBought < 1);
		
		totalBill += medList[newMed->medCode].price * newMed->totalBought;
		
		if(headMed == NULL)
		{
			headMed = newMed;
		}
		else
		{
			struct medNode *ptr = headMed;
			
			while(ptr->next != NULL)
			{
				ptr = ptr->next;
			}
			
			ptr->next = newMed;
		}
		
		do{
			printf("Do you want to buy other medicine?[y/n]: ");
			fflush(stdin);
			scanf("%c", &stillWantToBuy);
		}while(stillWantToBuy != 'y' && stillWantToBuy != 'n');
	}while(stillWantToBuy == 'y');
	
	printf("Order summary:\n");
	
	struct medNode *ptr = headMed;
	
	do{
		long int price = ptr->totalBought * medList[ptr->medCode].price;
		printf("%-15s - %dx = %ld\n", medList[ptr->medCode].medName, ptr->totalBought, price);
		ptr = ptr->next;
	}while(ptr != NULL);
	
	if(isFound != NULL)
	{
		if(isFound->numberVisit % 5 == 0)
		{
			double discount = (double)0.9 * (double)totalBill;
			totalBill = (long int)discount;
		}
	}
	
	printf("Total: %ld\n", totalBill);
}

void showMenu()
{
	printf("SUNIB's Pharmacy\n");
	printf("================\n");
	printf("1. Process Order\n");
	printf("2. View All Customer\n");
	printf("3. Remove Customer\n");
	printf("4. Exit\n");
	printf(">> ");
}

void printInOrder(struct node *ptr, int found)
{
	if(ptr == NULL && found == 0) printf("There is no data !\n");
	else if(found == 0)
	{
		int i;
		for(i=0;i<80;i++) printf("-");
		printf("\n");
		
		printf("| %-26s | %-14s | %-21s | %-7s |\n", "Name", "Phone Number", "Email", "Visits");
		
		for(i=0;i<80;i++) printf("-");
		printf("\n");
		found == 1;
	}
	
	if(ptr != NULL)
	{
		if(ptr->left != NULL)
		{
			printInOrder(ptr->left, 1);
		}
		
		printf("| %-26s | %-14s | %-21s | %-7d |\n", ptr->name, ptr->phoneNumber, ptr->email, ptr->numberVisit);
		int i;
		for(i=0;i<80;i++) printf("-");
		printf("\n");
		
		if(ptr->right != NULL)
		{
			printInOrder(ptr->right, 1);
		}
	}
}

void viewAllCustomer(struct node **root)
{
	struct node *ptr = *root;
	printInOrder(ptr, 0);
}

void removeCustomer(struct node **root)
{
	if(root == NULL)
	{
		printf("There is no data !");
	}
	else
	{
		char phoneNumber[15];
		int validPhoneNumber = 0;
		do{
			printf("Input phone number to delete[10-13][numeric]: ");
			fflush(stdin);
			scanf("%s", phoneNumber);
			validPhoneNumber = validatePhoneNumber(phoneNumber);
		}while(validPhoneNumber != 1);
		
		long long int asciiPhone = countAscii(phoneNumber);
		struct node *phoneNumberFound = searchPhoneNumber(phoneNumber, root, asciiPhone);
		
		if(phoneNumberFound == NULL)
		{
			printf("Data Invalid !\n");
		}
		else if(phoneNumberFound != NULL)
		{
			if(phoneNumberFound->left == NULL && phoneNumberFound->right == NULL)
			{
				if(phoneNumberFound->parent == NULL)
				{
					*root = NULL;
				}
				else if(phoneNumberFound->parent->left == phoneNumberFound)
				{
					phoneNumberFound->parent->left = NULL;
				}
				else if(phoneNumberFound->parent->right == phoneNumberFound)
				{
					phoneNumberFound->parent->right = NULL;
				}
				
				free(phoneNumberFound);
			}
			else if(phoneNumberFound->left != NULL)
			{
				struct node *ptr = phoneNumberFound->left;
				
				while(ptr->right != NULL)
				{
					ptr = ptr->right;
				}
				
				phoneNumberFound->asciiPhone = ptr->asciiPhone;
				phoneNumberFound->numberVisit = ptr->numberVisit;
				strcpy(phoneNumberFound->name, ptr->name);
				strcpy(phoneNumberFound->email, ptr->email);
				strcpy(phoneNumberFound->phoneNumber, ptr->phoneNumber);
				
				if(ptr->left != NULL)
				{
					ptr->parent->left = ptr->left;
					ptr->left->parent = ptr->parent;
				}
				
				if(ptr->parent->left == ptr)
				{
					ptr->parent->left = NULL;
				}
				else if(ptr->parent->right == ptr)
				{
					ptr->parent->right = NULL;
				}
				
				free(ptr);
			}
			else if(phoneNumberFound->right != NULL)
			{
				struct node *ptr = phoneNumberFound->right;
				
				while(ptr->left != NULL)
				{
					ptr = ptr->left;
				}
				
				phoneNumberFound->asciiPhone = ptr->asciiPhone;
				phoneNumberFound->numberVisit = ptr->numberVisit;
				strcpy(phoneNumberFound->name, ptr->name);
				strcpy(phoneNumberFound->email, ptr->email);
				strcpy(phoneNumberFound->phoneNumber, ptr->phoneNumber);
				
				if(ptr->right != NULL)
				{
					ptr->parent->right = ptr->right;
					ptr->right->parent = ptr->parent;
				}
				
				if(ptr->parent->left == ptr)
				{
					ptr->parent->left = NULL;
				}
				else if(ptr->parent->right == ptr)
				{
					ptr->parent->right = NULL;
				}
				
				free(ptr);
			}
			
			printf("Delete Success !\n");
		}
	}
}

void deleteAll(struct node **root)
{
	if(*root == NULL)
	{
		return;
	}
	else
	{
		if( (*root)->left != NULL )
		{
			deleteAll(&(*root)->left);
		}
		
		free(*root);
		
		if( (*root)->right != NULL )
		{
			deleteAll(&(*root)->right);
		}
	}
}

int main()
{
	struct node *root = NULL;
	int inputOperation = 0;
	
	do{
		system("cls");
		do{
			fflush(stdin);
			showMenu();
			scanf("%d", &inputOperation);
		}while(inputOperation < 1 || inputOperation > 5);
			
		switch(inputOperation)
		{
			case 1:
				processOrder(&root);
				break;
			case 2:
				viewAllCustomer(&root);
				break;
			case 3:
				removeCustomer(&root);
				printf("Thank you !!\n");
				break;
			case 4:
				deleteAll(&root);
		}
		printf("Press any key to continue . . .\n");
		getch();
	}while(inputOperation != 4);
	
	return 0;
}
