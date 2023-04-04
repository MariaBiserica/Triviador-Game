# Triviador

Jocurile de inteligență și strategie sunt populare în rândul copiilor, dar și al adulților care se mandresc cu o vastă cultură generală. Un astfel de tip de joc este Triviador, iar scopul acestui proiect este de a implementa propria noastra versiune a acestui joc. Pentru implementarea aplicației trebuie să respectăm regulile jocului:

1. Acțiunea jocului se desfășoară în cadrul unei hărți și este asemenea unei bătălii pentru teritoriu din cadrul unui război, obiectivul fiind cel de cucerire a unei suprafețe cat mai mari prin luptă. 

2.“Lupta” între 2 jucători se realizeaza prin intermediul întrebărilor de cultura generala, iar întrebările pot fi 2 tipuri:

a. întrebări de tip grila cu un singur răspuns corect: acest tip de întrebări decide castigatorul doar dacă unul dintre jucătorii răspunde corect

b. întrebări cu răspuns numeric: aceste întrebări decid castigatorul, acesta fiind cel care a oferit cel mai apropiat răspuns de cel corect(în cazul în care toți jucătorii au oferit același răspuns castigator va fi cel care a răspuns primul)

Obs: Aplicația trebuie sa gestioneze cel puțin 100 de întrebări!

3. Fiecare jucător activ al jocului deține:

a. o regiune numită “bază” - cucerirea bazei unei jucator reprezinta pierderea tuturor regiunilor detinute de acesta

b. mai multe(sau niciuna) regiuni teritoriale

4. Jucătorul devine inactiv în momentul în care pierde regiunea “bază”

5. Fiecărei regiuni îi este atribuit un anumit scor, reprezentand importanța acesteia
Jocul este alcatuit din 4 etape:

a. Etapa alegerii bazei: prin intermediul unei intrebari cu raspuns numeric va fi stabilită ordinea în care jucătorii își vor alege locul în care va fi construită bază următorului joc. Scorul atribuit acestei regiuni este 300

b. Etapa împărțirii regiunilor: prin intermediul unei intrebari cu raspuns numeric va fi stabilită ordinea și numărul de regiunii pe care le va putea selecta fiecare jucător(jucătorul de poziția k în clasament va putea selecta n-k regiuni care îi vor intra în posesie, n-fiind numărul de jucători). Această etapă se încheie în momentul în care toate regiunile din hartă sunt atribuite unui jucător, scorul acestora fiind 100 de unități.

c. Duelul: se va desfășura pe mai mai multe runde(numărul acestora diferă în funcție  de numărul de  jucători). În cadrul rundelor fiecare jucător(în ordine aleatoare) va încerca să cucerească o regiune vecină(2 regiuni sunt vecine doar dacă au cel puțin o granița comuna). Cei doi jucători se vor lupta iar dacă:

i. posesorul regiunii castiga lupta, atunci scorul regiunii va fi incrementat cu 100 de unități

ii. cel care a pornit duelul castiga, atunci regiunea intra în posesia acestuia cu scorul 100 doar dacă scopul acesteia era 100 inainte de duel, astfel regiunea va ramane posesia adversarului, dar scorul va fi decrementat cu 100 de unități

d. Stabilirea câștigătorilor: însumând scorurile tuturor regiunilor deținute de jucători după încheierea rundelor de joc 

6. Avantaje: în cadrul luptelor, jucătorii pot aplica cel mult o data pe durata unui joc fiecare dintre următoarele avantaje:

a. 50-50: avantaj care va elimina 2 răspunsuri dintre cele 4 ale întrebărilor de tip grila

b. alegere răspuns: avantaj care oferă utilizatorului 4 răspunsuri din care poate alege în cazul întrebărilor cu răspuns numeric 
 
c. sugerare răspuns: avantaj care sugerează utilizatorului răspunsul corect(sau o valoare apropiata cu acesta) în cazul întrebărilor cu răspuns numeric 

Costul fiecărui avantaj este egal cu 100 de unități. Acest cost va fi plătit din scorul unei regiuni a cărei valoare este de minimum 200 de unități, regiunea care va plăti prețul va fi aleasă de jucător după încheierea luptei în care a folosit avantajul.


Cerințe de bază

➡ Retelistica: implementarea aplicației respectand arhitectura client-server(aplicația trebuie sa asigure posibilitatea creării a minim 2 instanțe de client + 1 aplicație server care vor comunica prin rețea). 

➡ Pagină de Login/Register: la pornire, unui utilizator i se oferă posibilitatea de a se loga în contul său sau își poate crea un cont. Logarea/înregistrarea presupune introducerea numelui de utilizator. Atenție: numele de utilizator trebuie să fie unic!

➡ Pagină/Fereastra jocului: este fereastra în care va fi desenată harta(în varianta basic va fi afișată în consolă orice reprezentare a acesteia) și desfășurarea jocului.

Dimensiunea hartii este dependentă de numărul de jucători:
2 jucatori → harta  de 3x3, 5 runde
3 jucatori → harta  de 5x3, 4 runde 
4 jucatori → harta  de 6x4, 5 runde 

Dimensiunea tablei de joc și numărul de runde pot fi modificate! A se urmări o implementare cat mai generică. Deasemenea, dimensiunea tablei de joc poate avea și altă formă(diferită de cea dreptunghiulară)

➡ Pagina de profil a utilizatorului: Aici un utilizator poate vizualiza un istoric al meciurilor jucate 


Componente avansate ale proiectului

➡ GUI - Să se implementeze o interfață grafică pentru cerințele de bază folosind framework-ul Qt.

➡ Bază de date: Pentru a vă organiza datele, puteți să le stocați într-o bază de date. Se va folosi biblioteca de SQLite SQLite ORM (NU ALTA). Se poate instala într-un proiect de Visual Studio folosind Microsoft vcpkg.
