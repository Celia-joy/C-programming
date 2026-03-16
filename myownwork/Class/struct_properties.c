#include <stdio.h>
struct Car {
	int nbrOfTyres, nbrOfDoors, nbrOfSits;
	char Color[20], PlateNbr[10];
	float price;
};

struct Car Toyota, Benz;
struct Car *ptrCar= NULL;

void readInputs(){
	printf("INPUT PROPERTIES VALUES\n");
	printf("1. \n__TOYOTA_____________\n");
	ptrCar = &Toyota;
	printf("\n Nbr of Tyres: ");
	scanf("%d" , &ptrCar->nbrOfTyres);
	printf("\n Nbr of Doors: ");
	scanf("%d" , &ptrCar->nbrOfDoors);
	printf("\n Nbr of Sits: ");
	scanf("%d" , &ptrCar->nbrOfSits);
	printf("\n Price: ");
	scanf("%f" , &ptrCar->price);
	printf("\n Color: ");
	scanf("%s" , ptrCar->Color); 
	printf("\n Plate Nbr: ");
	scanf("%s" , ptrCar->PlateNbr);
	
	
	printf("2. BENZ\n_______________\n");
	ptrCar = &Benz;
	printf("\n Nbr of Tyres: ");
	scanf("%d" , &ptrCar->nbrOfTyres);
	printf("\n Nbr of Doors: ");
	scanf("%d" , &ptrCar->nbrOfDoors);
	printf("\n Nbr of Sits: ");
	scanf("%d" , &ptrCar->nbrOfSits);
	printf("\n Price: ");
	scanf("%f" , &ptrCar->price);
	printf("\n Color: ");
	scanf("%s" , ptrCar->Color); 
	printf("\n Plate Nbr: ");
	scanf("%s" , ptrCar->PlateNbr);
}
void readOutputs(){
	printf("OUTPUT PROPERTIES VALUES\n");
	printf("1. TOYOTA\n_______________\n");
	ptrCar = &Toyota;
	printf("\n Nbr of Tyres: %d" , ptrCar->nbrOfTyres);
	printf("\n Nbr of Doors: %d" , ptrCar->nbrOfDoors);
	printf("\n Nbr of Sits: %d" , ptrCar->nbrOfSits);
	printf("\n Price:%f" , ptrCar->price);
	printf("\n Color:%s" , ptrCar->Color); 
	printf("\n Plate Nbr: %s\n" , ptrCar->PlateNbr);
	
	printf("2. BENZ\n_______________\n");
	ptrCar = &Benz;
	printf("\n Nbr of Tyres: %d" , ptrCar->nbrOfTyres);
	printf("\n Nbr of Doors: %d" , ptrCar->nbrOfDoors);
	printf("\n Nbr of Sits: %d" , ptrCar->nbrOfSits);
	printf("\n Price:%f" , ptrCar->price);
	printf("\n Color:%s" , ptrCar->Color); 
	printf("\n Plate Nbr: %s\n" , ptrCar->PlateNbr);
	
}

int main () {
	readInputs();
	readOutputs();
    printf("\n SIZE OF ANY CAR: %d\n", sizeof(Toyota));   
    printf("\n SIZE OF ANY CAR: %d", sizeof(Benz));
	return 0;
}