/*

G��wne okno programu wygl�da tak:

	Text.ClrScr();
	Text.GoTo(0,0);
	Text.SetSpaces(1);
	Text.Write("\x88 \x81   A         B  \x80 ");
	
	Text.GoTo(0,2);
	Text.Write("1");
	
	Text.GoTo(0,5);
	Text.Write("2");

	Text.GoTo(0,7);
	Text.Write("\x89");
	
	Text.SetSpaces(1);
	Text.GoToAbs(10, 2);
	Text.Write("-2.31E31");
	
	// wykropkowanie poziome
	for( uint8_t k=0; k<8; k+=3	){
		for( uint8_t i=6; i<126; i+=6 ){
			Lcd_KS0108.SetPixel( i, 7+(8*k));
		};
	};
	
	// wykropkowanie pionowe
	
	for( uint8_t k=0; k<2; k++ ){
		for( uint8_t i=0; i<60; i+=4 ){
			Lcd_KS0108.SetPixel(6+(60*k), 3+i);
		};
	};
	
	for( uint8_t i=0; i<60; i+=4 ){
		Lcd_KS0108.SetPixel(126, 3+i);
	};

Dost�pne s� kom�rki od A do H (8). Prezenrtowane s� zawsze dwa wiersze i dwie kolumny. Strza�kami
przewuwamy zawarto�� ekranu na po��dane miejsce. 

Program steruj�cy przechowuje adres kom�rki znajduj�cej si� w lewym g�rnym rogu, pocz�tkowo jest 
to @A1. 

*************************
Program przechowuje adres lewej g�rnej kom�rki. Dotkni�cie strza�ki powoduje jego zmian� zgodnie z
wybranym wariantem oraz aktualizacje widoku na ekranie, czyli ustawienie w�a�ciwych liter kolumn oraz
w�a�ciwych numer�w wierszy, nast�pnie na ich podstawie pobierane s� dane z pami�ci ram o wynikach
oblicze� zawartych w tych kom�rkach. 
*************************

Dotkni�cie pola kom�rki wywo�uje okno textbox. 




KOM�RKA

pole robocze to 8 x 72 = 576 kom�rek
ka�da kom�rka przechowuje wy��cznie tekst, maksymalnie 40 znak�w
znak '=' wprowadzony przed innymi znakami powoduje obliczenie wyra�enia o ile to mozliwe
dotkni�cie kom�rki powoduje wy�wietlenie textboxa. 
je�li kom�rka jest pusta, textbox jest pusty
je�li kom�rka cokolwiek zawiera, prezentowana jest zawarto�� tekstowa
odwo�anie do kom�rki realizowane jest poprzez wpisanie do nowej kom�rki @ oraz adresu 
z�o�onego z litery kolumny A-H oraz numeru wiersza 1-72,
elementem docelowym kom�rki, do kt�rej adres podali�my jest wynik obliczony w tej kom�rce,
nie ca�a zawarto�� tekstowa. 
Dla warto�ci liczbowej prezentowane s� 3 cyfry znacz�ce i maksymalnie 6 znak�w. Wyk�adnik E oznacza 10^E,
wyk�adnik e oznacza 10^(-e). Przyk�ady:
123456	- 123E6
123		- 123
-123	- -123
-123123 - -123E6
0.13	- 0.13
0.000123- 123e6
-0.000123 - -123e6
Dotykaj�c kom�rki z warto�ci� liczbow� pojawi si� textbox z wprowadzonym tekstem, wybranie opcji E
wy�wietli dok�adn� warto�� obliczonego wyra�enia. Dok�adna warto�� oznacza 7 cyfr znacz�cych. Pozosta�e 
opcje prezentowania wynik�w pozostaj� takie jak wy�ej. 
W widoku g��wnym prezentowane s� maksymalnie 4 pierwsze znaki z wprowadzonego tekstu. 




TEXTBOX
pole textowe prezentuje tekst wprowadzony do kom�rki. 
textbox zawiera dwie strza�ki kierunku: w lewo i w prawo. S�u�� one do przesuwania kursora edycji tekstu. 
w textboxie widocznych jest maksymalnie 19 znak�w, w��cznie ze znakiem '='.
Gdy kursor dochodzi do ko�ca widocznego tekstu, tekst jest przesuwany w odpowiednim kierunku, 
zgodnie z ilo�ci� naci�ni�� odpowiedniej strza�ki lewo-prawo. 
Dotkni�cie pola tekstowego w textboxie przeniesie kursor we wskazane miejsce. Jednocze�nie je�li tekst
jest d�u�szy ni� 15 znak�w wyra�enie zostanie tak przesuni�te w lewo, by kursor by� na pozycji 8. 
Nad textboxem prezentowane s� znaki opcji edycji. Reprezentuj� one dost�pne funkcje wspomagaj�ce 
edycje tekstu. Ich ilo�� mo�e by� zmienna oraz mo�e by� zmieniana. 
Pod oknem edycji tekstu znajduje si� standardowa klawiatura. 
Widok textboxu zas�ania ca�y obszar widoku g��wnego.
Wybranie opcji "adres" chwilowo przywraca g��wny widok programu. Wybranie adresu realizowane jest przez 
dotkni�cie kom�rki, kt�rej adres chcemy pobra�. Po wybraniu adresu znika widok g��wny, powraca widok 
edycji textboxa a w tekscie znajduje si� adres wybranej przez dotkni�cie kom�rki. 
Dotkni�cie opcji OK zatwierdza wporowadzone zmiany.
Text przeznaczony do obliczenia ('=') obliczany jest natychmiast. Warunkiem wyswietlenia wyniku jest
logiczna poprawno�� wprowadzonego wyra�enia. B��d w procedurze obliczania sygnalizowny jest stosownym
komunikatem. 
Odwo�anie do pustej kom�rki traktowane jest jako b��d. 
Kursor jest automatycznie ustawiany za ostatnim znakiem znajduj�cym si� w polu tekstowym. 
Wybranie opcji "O" pozwoli wprowadzi� kr�tki opis tekstu (20 znak�w). Podpowied� nie jest normalnie 
widoczna. 
Textbox domyslnie zawiera pierwszy znak "=". Aby wprowadza� tekst nale�y si� go pozby� dotykaj�c "<-"

Automat - baza wzor�w
Baza zawiera gotowe fomu�y pozwalaj�ce na szybkie obliczenie bardziej z�o�onych wyra�e� 
cz�sto spotykanych w codziennej praktyce, na przyk�ad na obliczenie entalpii reakcji lub
czego� innego co mi do g�owy nie przychodzi. We�my na przyk�ad r�wnanie stanu gazu idealnego
"pv = nRT".
Aby wybra� taki wz�r nale�y dotkn�� kom�rki po�o�nej ni�ej ni� najni�sza aktualnie zaj�ta ( o maksymalnym
numerze adresu ), wpisa� "=+++" oraz dotkn�� "OK". 
Program wy�wietli menu z wyborem dziedziny ("MATH", "CHEM", "PHIS", "STAT");
Dotkni�cie pola spowoduje wy�wietlenie nazw dost�pnych r�wna� lub dokona dalszego zaw�ania obszaru 
poszukiwania formu�y. Ostateczie wybranie nazwy formu�y spowoduje pokazanie okna ze wszystkimi parametrami
formu�y - nale�y wybra� paramter, kt�ry chcemy obliczy�. 
Nast�pnie pojawi si� okno do wprowadzenia warto�ci pozosta�ych parametr�w. 
W pierwszej linijce znajduje si� symbol parametru oraz jego nazwa w j�zyku angielskim. 
Poni�ej znajduje si� pasek do wprowadzenia liczby. 
Poni�ej paska znajduje si� cz�� standardowej klawiatury ale zawieraj�ca jedynie znaki numeryczne 0-9 oraz
kropk� (nie u�ywamy przecink�w). 
Po wprowadzeniu warto�ci wybieramy "OK", pojawia si� nast�pny parametr. 
Gdy sko�czymy wprowadza� warto�ci parametr�w pojawi si� normalny textbox w wynikiem operacji. Po 
wybraniu "OK" pojawi si� g��wne okno programu. 
W oknie wybranym do wstawienia wzoru z bazy pojawi si� wynik w formie normalnie przyj�tej (3 cyfry 
znacz�ce, maksymalnie 6 znak�w)
Poni�ej znajd� sie symbole, kt�re wyst�powa�y w r�wnaniu a poni�ej nich ich warto�ci. Zmiana warto�ci
tych parametr�w spowoduje ponowne obliczenie wyniku i jego aktualizacj�. 


Troch� o dzia�aniu i pami�ci. 
Pami�� mikroprocesora nie zawiera �adnych danych na temat tego, co jest na ekranie. Wszystko znajduje si� 
w zewn�trzenj pami�ci ram. Pami�� ram uC przechowuje tymczasowe kopie obiekt�w za�adowane z pami�ci ram 
oraz dostarcza zasob�w niezb�dnych ale ograniczonych do potrzeb przetwarzania danych. 

Pami�� kom�rki musi zmie�ci� nast�puj�ce dane:
pami�� wyra�enia:
	odwo�ania s� traktowane jako zwyk�y tekst
	znak r�wno�ci, jesli wyra�enia podlega obliczaniu
	przyk�adowe wyra�enie:
		=sin(30)-log(123)
		=-atan(0.1)
		=2+2*2/123.123
 (#@A3)*=@A1/@A2
		(znak @ jest opcj� zawart� normalnie w oknie textbox)
wynik wyra�enia, kt�ry jest warto�ci� typu double (zajmuje 4 bajty). Do tej kom�rki pami�ci odnosi� b�dzie
si� adres wprowadzony z innej kom�rki.
* - kolejno�� obliczania - je�li zmieni si� A1 trzeba obliczy� A3.
Dlatego: do wyra�enia, kt�re jest nadrz�dne dla innych dodawana jest informacja, kt�re wyra�enia nale�y 
obliczy�. Dodanie takiej informacji polega na dopisaniu do g��wnego wyra�enia znaku "#" oraz adres�w
kom�rek, kt�re nale�y obliczy�. W przypadku, kiedy kom�rek jest wi�cej ni� jedna, oblicza si� kolejno
kom�rki. Powstanie do�� skomplikowna struktura. Trzeba to narysowa�.
		
	b1 = 1
	b2 = 2
	a1 = b1/b2
	a2 = a1/b1
	a3 = a1/3
		 
W momencie wci�ni�cia ok doda� do b1, �e ma zosta� obliczone a1 po obliczeniu b1.
Informacja jest dodawana r�wnie� do b2, bo nie musi si� zmienia� ale przecie� te� mo�e.
Zabronione s� odwo�ania do w�asnego adresu (wywo�ania cykliczne).

W obec tego kom�rka ma nast�puj�c� struktur�:
40 bajt�w dla danych znakowych (na ca�y zapami�tany tekst)
4 bajty wyniku
8 bajt�w podpowiedzi
Jedna kom�rka w pami�ci ram zajmuje dok�adnie 52 bajty. Rozmiar jest sta�y. 

jeszcze:

a1 = 1;	b1 = 3;	"a1" =1#a3b3a5	"b1" =3#b3
a2 = 2;	b2 = 4; "a2" =2#a3		"b2" =4

a3 = a1 + a2;	"a3" =a1+a2#a4
b3 = b1 + a1;	"b3" =b1+a1#a4

a4 = a3 + b3;	"a4" =a3+b3#a5
a5 = a4 + a1;	"a5" =a4+a1


a1=1;	a2=a1+1;	"a1" =1#a2a3	"a2" = a1+1#a3		"a3" =a1+a2
a3 = a1 + a2;
a3 = a1 + a1+1;

Standardowa klawiatura:

		Text.SetSpaces(4);
		
		Text.GoToAbs(5,4);
		Text.Write("1234567890()=");
		Text.GoToAbs(1,5);
		Text.Write("qwertyuiop+-/*");
		Text.GoToAbs(5,6);
		Text.Write("asdfghjkl.:'\"");
		Text.GoToAbs(1,7);
		Text.Write("\x88zxcvbnm<>?[]|");
		
		Text.GoToAbs(5,4);
		Text.Write("1234567890()=");
		Text.GoToAbs(1,5);
		Text.Write("QWERTYUIOP+-/*");
		Text.GoToAbs(5,6);
		Text.Write("ASDFGHJKL.:'\"");
		Text.GoToAbs(1,7);
		Text.Write("\x89ZXCVBNM<>?[]|");
Ramka TextBox

		Text.SetSpaces(0);
		Text.GoTo(0,1);
		Text.Write("\x82=======================\x84");
		
		Text.SetSpaces(1);
		Text.GoTo(0,2);
		Text.Write("\x86=                  \x87");
		
		Text.SetSpaces(0);
		Text.GoTo(0,3);
		Text.Write("\x83=======================\x85");
	
	dane wprowadzamy do TextBoxBuffer, kopiujemy do TextBoxWindow 19 znak�w wraz ze znakiem null
	i wy�wietlamy st�d na ekranie. 	
	
		uint8_t TextBoxBuffer[30];
		uint8_t TextBoxWindow[19];
		
		const char* tt = "qwertyuiopasdfghjkasd"; // wprowadzany tekst l�duje w TextBoxBuffer, 
		st�d trafia do pami�ci ram jako parametr kom�rki o wybranym adresie. 
		
		uint8_t len = strlen(tt);
		uint8_t start = 0;
		if( len > 18 ){
			start = len - 18;
		};
		
		strncpy( (char*)TextBoxWindow, tt+start, 19);
		
		
*************************
zmiana wielko�ci liter na ekranie
uwaga, do warto�ci znak�w specjalnych z tablicy lut doda� 32 
		if( read_key == 231 ){
			if( TextBoxViev.charsize == 2 ){
				TextBoxViev.SmalChars();
				}else{
				TextBoxViev.BigChars();
			};
			delay(0xFFFF);
		};
				
*/