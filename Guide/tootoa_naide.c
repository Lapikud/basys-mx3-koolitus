//programmikoodi näidis
//Kommenteerimine
#include <stdio.h>//printf
#include <stdbool.h>// true, false, bool
//prototüübid
int arvutaSumma(int arvudeNimekiri[], int punktideArv);
int vagaSuurPunktiSumma(int arvudeNimekiri[], int punktideArv);
void prindiPunktid(int punktiSumma);
//põhifunktsioon
int main(void)
{
    //muutujate deklareerimine
    char taht = 'A';
    int number = 0, nimekiri[6] = {3, 36, 1, 1, 2, 6};
    int punktideSumma;
	//muutujatele väärtuste omistamine
	number = 6;
	nimekiri[0] = 5; // jada esimene arv indeksiga 0
	nimekiri[3] = 2; //jada neljas arv indeksiga 3
	//nimekiri[6] = {5, 36, 1, 2, 2, 6}
	punktideSumma = arvutaSumma(nimekiri, number); //arvutatakse punktid kokku
	//switch case'i kasutamise näide
	switch (vagaSuurPunktiSumma(nimekiri, number))
	{
	    case 0:
	        printf("Punktisummad olid väiksed.\n");
	        break;
        case 1:
            printf("Leidus suur punktisumma.\n");
            break;
        default:
            printf("Miski läks valesti!\n"); //Kui see prinditakse, läks midagi halvasti!
	}
	prindiPunktid(punktideSumma);//kuvatakse punktid

	return 0;
}
/* ------------------------------------------------------------ */
/***	arvutaSumma
**	Parameetrid:
**          int arvudeNimekiri[] - Meie nimekiri arvudest
**          int laskudeArv - elementide arv nimekirjas
**	Tagastatav väärtus:
**          int arvudKokku - Summa, kus on kõik arvud kokku liidetud meie arvude nimekirjast
**	Kirjeldus:
**		Arvutab kokku, kõik arvud, mis on arvude nimekirjas.
**
*/
int arvutaSumma(int arvudeNimekiri[], int punktideArv)
{
    int i; //loendur
    int arvudKokku = 0; //summa
    for(i = 0; i < punktideArv; i++)
    {
        arvudKokku = arvudKokku + arvudeNimekiri[i];
    }

    return arvudKokku;
}

int vagaSuurPunktiSumma(int arvudeNimekiri[], int punktideArv)
{
    bool kontrollime = true;
    int i = 0, tulem = 0;// 0 - false 1 - true
    while(kontrollime)
    {
        if(arvudeNimekiri[i] > 30) //Kui tulemus on suurem kui 30
        {
            tulem = 1;
            break; // väljutakse tsüklist

        }else
        {
            i++; // i = i + 1;
        }

        if( i == punktideArv)
        {
            break;//väljutakse tsüklist
        }
    }
    return tulem; //Tagastab 1 kui mõni punktisumma oli üle 30, muidu tagastab 0
}
void prindiPunktid(int punktiSumma)
{
    printf("Punktide summa on %d\n", punktiSumma); //%d asendub antud muutuja väärtusega, ehk punktiSumma väärtusega
}
