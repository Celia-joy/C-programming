 #include <stdio.h>
 struct Car{
 	int nbrOfTyres, nbrDoors, nbrOfWindows, sits;
 	char color[20], plateNbr[10];
 };
  struct Car Benz, Toyota, Fuso, Ferrari;
  
  void inputBenz(){
   	  	printf("Number of tyres: ");
  	    scanf("%d", &Benz.nbrOfTyres);
  	    printf("Number of tyres for Benz = %d",Benz.nbrOfTyres);
   }
   
   void inputToyota(){
   	printf("The number of doors:");
   	scanf("%d", &Toyota.nbrDoors);
   	printf("Number of doors for Toyota = %d", Toyota.nbrDoors);
   	return;
   }
   
   void inputFuso () {
   	    printf("Number of windows: ");
  	    scanf("%d", &Fuso.nbrOfWindows);
  	    printf("Number of windows for Fuso = %d",Fuso.nbrOfWindows);   	
   }
   
   void inputFerrari (){
   	printf("Color:");
   	scanf("%s", &Ferrari.color);
   	printf("The color of the ferrari = %s", Ferrari.color);  
	return; 	
   }
   
  int main(){ 
  	printf("Input data for objects\n _________________\n 1. Benz\n____________________\n");
  	inputBenz();
  	printf("\n");
  	
  	printf("2. Toyota \n____________________\n");
  	inputToyota();
  	printf("\n");
  	
  	printf("3. Fuso\n____________________\n");
  	inputFuso();
  	printf("\n");
  	
  	printf("4. Ferrari\n____________________\n");
  	inputFerrari();
  	printf("\n");
  	
  	return 0;
  }
