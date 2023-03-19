# APTS_Mati
A solution to the following programming problem (text is in Latvian):


## Programmējamais uzdevums Mati
**Atmiņa**: 4 MB

**Laiks**: 0.2 second

**Ievadfails**: hair.in

**Izvadfails**: hair.out

## Apraksts

Lielā pilsētā (vairāki miljoni iedzīvotāju, bet ne vairāk par miljardu) atrodas viena vienīga frizētava, kurā strādā neliels daudzums frizieru $[1..9]$. Katram frizierim ir savs numurs $[1..9]$, lai vieglāk varētu organizēt frizētavas darbu. Frizētavā laiku skaita nosacītās laika vienībās $[1..2000000000]$, un skaitīšana ir sākta kopš frizētavas atvēršanas brīža.

Kaut arī pilsētā klientu ir ļoti daudz un frizieri ir ļoti pieprasīti, katram frizierim obligāti ir jādodas atpūsties. Obligātais atpūtas laiks frizierim ir tad, kad laika momenta simtnieku pozīcijas cipars sakrīt ar friziera numuru, t.i. frizierim ar numuru 5 jāatpūšas laikos $[500..599]$, $[1500..1599]$, $[2500..2599]$, utt. Pārtraukuma laikā frizierim ir kategoriski aizliegts apkalpot klientu. Bez tam klienta apkalpošanu nedrīkst sadalīt vairākos etapos, t.i. apkalpot drīkst tikai viens frizieris un bez jebkādiem pārtraukumiem. Tātad frizieris nedrīkst sākt apkalpot klientu, ja to nevar paspēt līdz pārtraukuma brīdim.

Klients ir jāapkalpo nekavējoties, ja eksistē brīvs frizieris un tam nav nekādu ierobežojumu veikt šo darbu. Frizierim, beidzot darbu pie kārtējā klienta, ar nākošo laika momentu jāmēģina apkalpot kādu klientu. Precīzāka specifikācija: klients $K_1$ ierodas laikā $T_1$ un apkalpošanai nepieciešams laiks $A_1$. Ir Brīvs frizieris $F_1$. Līdz ar to apkalpošana notiek laika intervālā $[T_1..T_1+A_1-1]$. Apkalpošanas beigas ir laika momentā $T_1+ A_1-1$. Ja klients $K_2$ jau ir ieradies pirms vai tieši laika momentā $T_1+ A_1$, tad frizieris $F_1$ var sākt apkalpošanu klientam $K_2$ laika momentā $T_1+ A_1$.

Klienti gaida stingrā (godīgā) rindā. Vienā laika momentā var ierasties tikai viens klients. Reģistrators uzreiz nosaka klientam ierašanās kārtas numuru $[1..200 000]$ un pakalpojuma laiku $[1..900]$.

Ja eksistē neapkalpots klients un uz to vienlaicīgi pretendē vairāki brīvi frizieri, tad:
1. priekšroka ir frizierim, kurš visilgāk ir bijis bez klientiem (mēra no klienta apkalpošanas beigām);
2. ja frizieriem ir vienāds gaidīšanas laiks, tad priekšroka ir frizierim ar mazāko numuru.
 
Zinot klientu ierašanās laikus, klientu kārtas numurus un pakalpojumu ilgumus, izdrukāt klientu apkalpošanas beigu momentus, apkalpojošā friziera numuru un klienta kārtas numuru. Drukāšana jāveic laika pieaugšanas secībā. Ja vienā laika momentā beidz apkalpot vairākus klientus, tad rezultāti jādrukā frizieru numuru secībā.

## Ieeja:
Pirmajā rindiņā ir vesels skaitlis, kas nosaka fricieru skaitu. Tālāk seko informācija par klientiem to ierašanās secībā.
```
Frizieri
Laiks Klients Ilgums
...
0
```
* Frizieri nosaka frizieru skaitu $[1..9]$
* Laiks nosaka laika momentu, kad klients ieradies un ir jau gatavs apkalpošanai $[1..2 000 000 000]$
* Klients nosaka klienta kārtas numuru $[1..200 000]$
* Ilgums nosaka nepieciešamā pakalpojuma ilgumu $[1..900]$
* 0 nosaka, ka ieejas dati ir beigušies. Šajā gadījumā Klients un Ilgums netiek norādīti

Ieejas dati ir stingrā hronoloģiskā secībā.

Ieejas dati ir korekti saskaņā ar ieejas formātu un dotajiem ierobežojumiem.

## Izeja:
Atbilstoši ieejas failam katra klienta apkalpošanas beigu ieraksts ir formātā:
```
Laiks Frizieris Klients
```
* Laiks nosaka laika momentu, kad ir pēdējais klienta apkalpošanas moments $[1..2 000 000 000]$
* Frizieris nosaka frizieri, kas apkalpoja klientu, t.i. tas ir friziera numurs $[1..9]$
* Klients nosaka klienta, kuru apkalpoja, kārtas numuru $[1..200 000]$

Rezultātos neviens apkalpošanas beigu laiks nebūs lielāks par $2 000 000 000$.

## Piemērs:
Ieejas faila hair.in saturs:
```
2
11 1 10
21 2 50
31 3 20
0
```

Izejas faila hair.out saturs:
```
20 1 1
50 1 3
70 2 2
```
