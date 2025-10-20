task 1
verifica daca blocul se afla in interiorul chunk-ului iar apoi adauga tipul de block dorit la coordonatele (x,y,z)

task 2
cauta coordonatele minime si cele maxime ale paralelipipedului dorit iar apoi se parcurge paralelipipedul si se pun in interior tipul de block dorit

task 3
cu ajutorul formulei (x-x0)^2+(y-y0)^2+(z-z0)^2 < raza^2 modific doar block-urile care se afla in interiorul sferei

task 4
pe copia unui chunck si cu ajutorul a trei vectori de pozitii verifica existenta vecinilor blocurilor de tipul cautat, iar apoi adauga pe pozitiile acestora target_block-ul. am facut copia pentru a nu iesi din dimensiunile chun-ului initial

task 5
modifica blocul de la coordonatele (x,y,z), iar apoi cu ajutorul a doi vectori de pozitii si a unui algoritm de fill care are la baza o coada(algoritmul lui lee) modifica vecinii

task 6
modifica blocul de la coordonatele (x,y,z), iar apoi cu ajutorul a trei vectori de pozitii si a unui algoritm de fill care are la baza o coada(algoritmul de tip bfs) modifica vecinii

task 7 
am facut o copie a chunk-ului cu noile dimensiuni in care am adaugat valorile blocurile corespunzatoare

task 8
se face o copie a chunk-ului si o matrice pt a tine evidenta blocurilor prin care am trecut. cu ajutorul unui algoritm bfs se gasesc corpurile. dupa se verifica daca poate cobori si se muta blocul(cu o unitate pe axa y). se modifica inaltimea cu cel mai inatlt block si se returneaza noul chunk

task 9
am facut doua subprograme: unul care realizeaza forma ceruta atunci cand numarul este mai mic decat 32 si altul care realizeaza o alta forma atunci cand numarul este in intervalul [32,4095]. pentru a realiza forma dorita a vectorului trec toate valorile din matricea tridimensionala intr-un vector pe care il parcurg pentru a forma secvente de maxim 4095 de elemente. fiecare astfel de secventa este transformata intr-un while care apeleaza cele doua functii implementate anterior.

task 10
parcurge vectorul dat si transforma valorile in blocuri pe care le pune in chunk-ul nou format, dupa parcurgerea vectorului se da return la chunk