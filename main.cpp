#include "slike.h"
#include <windows.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <bitset>

int binaryToDecimal(std::string n)
{
	using namespace std;
    string num = n;
    int dec_value = 0;
    int base = 1;
    int len = num.length();
    for (int i = len - 1; i >= 0; i--) {
        if (num[i] == '1')
            dec_value += base;
        base = base * 2;
    }
    return dec_value;
}

std::string setStringtoASCII(std::string str)
{
	using namespace std;
    int N = int(str.size());
    if (N % 8 != 0) {
        return "Nije moguce pretvoriti!";
    }
    string res = "";
    for (int i = 0; i < N; i += 8) {
        int decimal_value = binaryToDecimal((str.substr(i, 8)));
        res += char(decimal_value);
    }
    return res;
}

void skrivanje(){
	using namespace std;
	string plaintext;
  string naziv_datoteke;
  cout<<"Unesi naziv tekstualne datoteke u koju zelis sakriti poruku: ";
  cin>>naziv_datoteke;
  ifstream file(naziv_datoteke.c_str());
  
  string str;
  
  while (std::getline(file, str)) {
    plaintext += str;
  }

  int plaintext_duljina = plaintext.length();
 
  char tekst[plaintext_duljina + 1];
  strcpy(tekst, plaintext.c_str());

  cout << endl << endl; 
    uint64_t brojac_whitespace = 0;
  		for (uint64_t i = 0; i < sizeof(tekst); i++) {
    		if (tekst[i] == 32) {
      			brojac_whitespace++;
    		}
  }

  string poruka = "";
  cin.ignore();
  cout<<"Unesi poruku (max znakova->"<<brojac_whitespace / 8 <<" ): ";
  getline(cin, poruka);
  
  int n = poruka.length();
  char polje_poruke[n + 1];
  strcpy(polje_poruke, poruka.c_str());

  string proba;

  for (int i = 0; i < n; i++) {
    proba = proba + bitset < 8 > (polje_poruke[i]).to_string();
  }


  int duljina = proba.length();
  char skrivena_poruka[duljina + 1];
  strcpy(skrivena_poruka, proba.c_str());

  if ((poruka.size()*8) > brojac_whitespace) {
    cout << "Poruka je prevelika! " << endl;
  } else {
  	

    uint64_t pom_brojac = 0;
    for (uint64_t i = 0; i < plaintext.size(); i++) {
      if(plaintext[i]==32 && skrivena_poruka[pom_brojac]==48 && pom_brojac<sizeof(skrivena_poruka)){
			pom_brojac++;
		}
		else if(plaintext[i]==32 && skrivena_poruka[pom_brojac]==49 && pom_brojac<sizeof(skrivena_poruka)){
			plaintext.insert(i, 1, 32); i++;
			pom_brojac++;
		} else if(pom_brojac==sizeof(skrivena_poruka)-1 && plaintext[i]==32 ){
			
			plaintext.insert(i, 2, 32);
			break;
		}

    }

    cout<<"Poruka je uspjesno sakrivena! "<<endl;
    string izlaz;
    cout<<"Unesi naziv izlazne datoteke (+.txt): ";
    cin>>izlaz;
    ofstream out(izlaz.c_str());
    out << plaintext;

  }
}

void otkrivanje(){
	using namespace std;
	string kodirani_text;
  string naziv_datoteke_c;
  cout<<"Unesi naziv tekstualne datoteke iz koje zelis procitati poruku: ";
  cin>>naziv_datoteke_c;
  ifstream file(naziv_datoteke_c.c_str());
  string str;
  while (std::getline(file, str)) {
    kodirani_text+=str;
  }
  
  string skrivena_poruka="";
  string nula="0";
  string jedan="1";

	for(uint64_t i=0;i<kodirani_text.size();i++){
		if(kodirani_text[i]==32 && kodirani_text[i+1]==32 && kodirani_text[i+2]!=32 ){
			skrivena_poruka+=jedan;
		}else if(kodirani_text[i]==32 && kodirani_text[i+1]!=32 && kodirani_text[i-1]!=32 ){
			skrivena_poruka+=nula;
		} else if(kodirani_text[i]==32 && kodirani_text[i+1]==32 && kodirani_text[i+2]==32){
			break;
		}
		
	}
	cout<<endl;
cout << setStringtoASCII(skrivena_poruka);
}


void steganografija_poruke() {
	using namespace std;
	
  cout<<endl<<"        ********* PROGRAM ZA SKRIVANJE I OTKRIVANJE PORUKE U TEKSTUALNOJ DATOTECI *********"<<endl<<endl<<endl;
  
  char odabir;
  bool nastavi=1;
  while(nastavi==1){
  	
  cout<<"Za sakrivanje poruke unesite S, za otkrivanje unesite O: ";
  cin>>odabir;
  	
  	if(odabir=='S'){
  		skrivanje();
	  }else if(odabir=='O'){
	  	otkrivanje();
	  }else cout<<"Porgresan unos! "<<endl;
	  cout<<endl;
  	cout<<"Nastavi koristiti program? (0-NE  1-DA): ";
  	cout<<endl;
  	cin>>nastavi;
  }
}

void steganografija_slike(){
	char a = 'a';
	bool x = false;
	do{
		std::cout<<"Zelite encodeat ili decodeat poruku? [e/d]:";std::cin>>a;
		if(a == 'e' || a == 'd')x = true;
	}while(!x);
	
	if(a == 'e'){
		char filepath[64], poruka[256], naziv[69];
		printf("Unijeti naziv slike (mora ukljucivati (.png/.jpg/.bmp/.tga) na kraju):");
		std::cin.ignore();
		std::cin.getline(filepath, 64);
		
		printf("Unijeti poruku:");
		std::cin.getline(poruka, 256);
		
		printf("Unijeti naziv nove slike (mora ukljucivati (.png/.jpg/.bmp/.tga) na kraju):");
		std::cin.getline(naziv, 64);
		
		Image test(filepath);
		
		test.encodeMessage(poruka);
		test.write(naziv);
		
		printf("Poruka je uspjesno encodeana!\n");
	}
	else if(a == 'd'){
		char filepath[64];
		printf("Unijeti naziv slike (mora ukljucivati (.png/.jpg/.bmp/.tga) na kraju):");
		std::cin.ignore();
		std::cin.getline(filepath, 64);
		
		Image test(filepath);
		
		char buffer[256] = {0};
		size_t len = 0;
		test.decodeMessage(buffer, &len);
		
		printf("Message: %s\n", buffer);
	}
}

void izbornik(){
	printf("Steganografija - Goran Mardetko - Ivan Lucic\n");
	printf("-------------\n1. Skrivanje poruke unutar teksta\n2. Skrivanje poruke unutar slike\n3. Izlaz iz programa\nIzbor: ");
}

int main(int argc, char **argv)
{
	int a;
	
	do{
		izbornik();
		std::cin>>a;
		switch(a){
			case 1:
				steganografija_poruke();
				break;
			case 2:
				steganografija_slike();
				break;
			case 3:break;
		}
	}while(a != 3);
	
	return 2;
}
