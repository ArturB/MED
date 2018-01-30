######################################
####      Projekt 1 MED 2017Z     ####
####        Artur M. Brodzki      ####
######################################

# Celem projektu jest analiza porównawcza czynników będącym predyktorami rozwodu w zależności od etniczności badanych. W dostępnym zbiorze danych demograficznych, występują głównie badani etniczności albo białej, albo czarnej, z tego też powodu zostaną wykonane dwie osobne analizy: predyktorów rozwodu dla etniczności białej i predyktorów rozwodu dla etniczności czarnej. Dostępny zbiór danych nie jest reprezentatywny: na  21.727 rekordów, 18.378 dotyczy etniczności białej, a jedynie 3349 etniczności czarnej. Dodatkowo, w dostępnym zbiorze jest jedynie 3860 osób rozwiedzionych. Oznacza to, że rozwiedzionych białych jest 3068, a rozwiedzionych czarnych jest 555 (237 rozwiedzionych jest innych etniczności). Taka postać zbioru danych nakłada na nas następujące warunki na parametry algorytmu:

# - dla każdej etniczności chcemy odnaleźć kilkanaście - kilkadziesiąt reguł asocjacyjnych, których następnikiem jest fakt bycia osobą rozwiedzioną
# - wsparcie: zasadniczo im większe tym lepsze, ale dla zachowania statystycznej wiarygodności, średnio na każdą regułę powinno przypadać wsparcie co najmniej 25 osób. Przyjmujemy minimalne, nieprzekraczalne wsparcie równe 4% dla etniczności czarnej i 1% dla etniczności białej. W przypadku odnalezienia wielu reguł (np. kilkaset) będziemy mieć możliwość zwiększania współczynnika wsparcia, aż liczba reguł osiągnie zakładany poziom. 
# - zaufanie: ponieważ zaufanie nie jest najlepszą miarą jakości reguły asocjacyjnej, nie nakładamy spacjalnych warunków na ten współczynnik, jednak podczas analizy porównawczej znalezionych reguł będziemy preferować reguły o wyższym współczynniku pewności. Zaczynając od 0.1, będziemy stopniowo zwiększać ten współczynik gdyby liczba znalezionych reguł była duża (np. 200). 
# - współczynnik podniesienia: współczynnik ten będzie dla nas główną miarą jakości odnalezionych reguł asocjacyjnych. Będziemy dążyć do ustalenia jak największego progu minimalnego współczynnika podniesienia tak, aby liczba odnalezionych reguł mieściła się w ustalonym przedziale. Jako wstępny próg podniesienia uznawany za satysfakcjonujący przyjmujemy 1.5. Reguły o wsp. podniesienia <1.1 uznajemy za na pewno nieinteresujące. 

# Po odnalezieniu interesujących reguł dla obu etniczności, wykonamy ich analizę porównawczą oraz próbę wyjaśnienia odnalezionych zależnosci na gruncie psychologicznym, socjologicznym i dialektycznym.

########################################
####     Ustawienia początkowe      ####
########################################

#ustawienie katalogu roboczego
setwd("~/MEDR")

#Pakiet ARules
library(arules)
library(arulesViz)

#########################################
####       Przydatne funkcje         ####
#########################################

#Usunięcie atrybutów z tylko jedną wartością (nieróżnicujących)
delOneValued <- function(inputData11)
{
  res <- c(which(sapply(inputData11, function(x) {length(unique(x))}) == 1));
  if(length(res) > 0)         
  {
    data11 <- inputData11[,-res];
  }   
  else
  {
    data11 <-inputData11;
  }
  data11 
}

#Usunięcie atrybutów z każdą wartością unikalną (nie dających się agregować)
delUniqueValued<- function(inputData11)
{
  res <- c(which(sapply(inputData11, function(x) {length(unique(x))}) == nrow(inputData11)));
  if(length(res) >0)         
  {
    data11 <- inputData11[,-res];
  }   
  else
  {
    data11 <-inputData11;
  }
  
  data11 
}


#Funkcja wyszukująca na zadanym zbiorze danych reguły asocjacyjne z zadanym następnikiem. Zwraca zbiór odnalezionych reguł asocjacyjnych. 
rulesExperiment <- function(
  dataset,             # zbiór danych do analizy
  minSupport,          # minimalne wsparcie szukanych reguł
  minConfidence,       # minimalna pewność szukanych reguł
  subsetF = NULL,      # filtr na analizowane osoby. 
  antecendent = NULL,  # wymagane poprzedniki reguł.
  consequent = NULL,   # kształ następnika. 
  minLength = 2,       # minimalna długość reguł (wraz z następnikiem)
  maxLength = 10,      # maksymalna długosć reguł (wraz z następnikiem)
  minLift = 1.0)       # minimalny wsp. podniesienia reguł
{
  
  
  #Konwersja danych do formy transakcyjnej
  dataTR <- as(dataset, "transactions")
  #Usuwanie duplikatów
  dataTR <- unique(dataTR)
  
  #Filtr na analizowane osoby
  if(!is.null(subsetF)) {
    dataTR = subset(dataTR, subset = items %in% subsetF)
  }
  
  #Opcjonalny filtr na poprzednik i następnik reguł
  if(!is.null(antecendent) && !is.null(consequent)) {
    appearance = list(lhs = antecendent, rhs = consequent, default="lhs")
  }
  else if(is.null(antecendent) && !is.null(consequent)) {
    appearance = list(rhs = consequent, default = "lhs")
  }
  else if(!is.null(antecendent) && is.null(consequent)) {
    appearance = list(lhs = antecendent, default = "rhs")
  }
  else {
    appearance = NULL
  }
  
  #Obliczenie wsparcia poszczególnych atrybutów
  dataTRSupport  = itemFrequency(dataTR, type = "relative")
  #parametry algorytmu APriori
  aParam  = new("APparameter", 
                "confidence" = minConfidence, 
                "support" = minSupport, 
                "minlen"= minLength,
                "maxlen" = maxLength,
                "target" = "rules")
  #Wyszukiwanie reguł
  rules <- apriori(dataTR, aParam, appearance)
  #Filtrowanie według wsp. poniesienia
  print('filtering rules by lift...')
  rulesLift <- subset(rules, subset = lift >= minLift)
  print(paste0('writing... [', length(rulesLift), ' rule(s)] done [0.00s]'))
  rulesLift
}

#########################################
####     Przygotowanie danych        ####
#########################################

#Ładowanie zbioru danych
data("AdultUCI")

#Przygotowanie danych "AdultUCI"
## Usunięcie parametrów mających tylko jedną wartość, więc nieróżnicujących
AdultUCI <- delOneValued(AdultUCI)
## Usunięcie parametrów mających tylko unikalne wartości zatem nie dające się zagregować
AdultUCI <- delUniqueValued(AdultUCI)
## Usunięcie atrybutów fnlwgt i education-num, jako nieistotnych
AdultUCI <- AdultUCI[, -3]
AdultUCI <- AdultUCI[, -4]
## Usunięcie atrybutu relationship jako niemal identycznego z marital-status:
AdultUCI <- AdultUCI[, -6]
## Dyskretyzacja parametrów ciągłych:
### Wieku:
AdultUCI$age <- ordered(cut(AdultUCI[["age"]], c(0,18,30,40,50,65,Inf)), labels = c("underage", "twenties", "thirties", "fourties", "after 50", "pensioner"))
### Zyski kapitałowe (dzielimy na zerowe i niezerowe, ponieważ zdecydowana większość badanych nie inwestuje na giełdzie i nie osiąga żadnych zysków. )
AdultUCI$`capital-gain` <- ordered(cut(AdultUCI[["capital-gain"]], c(-Inf,1,1000000000), labels = c("zero", "non-zero") ))
### Straty kapitałowe (dzielimy na zerowe i niezerowe, ponieważ zdecydowana większość badanych nie inwestuje na giełdzie i nie poniosła z tego tytułu żadnych strat. )
AdultUCI$`capital-loss` <- ordered(cut(AdultUCI[["capital-loss"]], c(-Inf,1,1000000000), labels = c("zero", "non-zero") ))
### Praca w godzinach tygodniowo - podział względem mediany (która - jak nietrudno zauważyć - wynosi 40h)
AdultUCI$`hours-per-week` <- ordered(cut(AdultUCI[["hours-per-week"]], c(0, quantile(AdultUCI$`hours-per-week`, c(0.5)), Inf)), labels=c("lower-half", "upper-half"))

## Konwersja do formy transakcyjnej
AdultUCITR <- as(AdultUCI, "transactions")

#########################################
####          Eksperymenty           ####
#########################################

########################
### ETNICZNOŚĆ BIAŁA ###
########################

#Eksperyment początkowy
v <- rulesExperiment(AdultUCITR, minSupport = 0.01, minConfidence = 0.1, minLift = 1.5, consequent = "marital-status=Divorced", subsetF = "race=White")
#Znaleziono 546 reguł, które jak widać na załączonych przykładach mają często bardzo małe wsparcie, możemy więc zwiększyć minimalne wsparcie do 0.04. 
#Przykłady znalezionych reguł:
#456 {age=after 50,                                                                            
#workclass=Private,                                                                       
#race=White,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero,                                                                       
#native-country=United-States} => {marital-status=Divorced} 0.01561225  0.2524186 1.512455
#457 {education=HS-grad,                                                                       
#race=White,                                                                              
#sex=Female,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero,                                                                       
#native-country=United-States} => {marital-status=Divorced} 0.01441549  0.2634195 1.578370
#458 {age=fourties,                                                                            
#race=White,                                                                              
#sex=Female,                                                                              
#capital-gain=zero,                                                                       
#native-country=United-States,                                                            
#income=small}                 => {marital-status=Divorced} 0.01098841  0.4040000 2.420708
#459 {age=fourties,                                                                            
#race=White,                                                                              
#sex=Female,                                                                              
#capital-loss=zero,                                                                       
#native-country=United-States,                                                            
#income=small}                 => {marital-status=Divorced} 0.01098841  0.4040000 2.420708
#460 {age=fourties,                                                                            
#race=White,                                                                              
#sex=Female,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero,                                                                       
#income=small}                 => {marital-status=Divorced} 0.01180438  0.3747841 2.245651
#461 {age=fourties,                                                                            
#workclass=Private,                                                                       
#sex=Female,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero,                                                                       
#native-country=United-States} => {marital-status=Divorced} 0.01028124  0.3559322 2.132693
#462 {age=fourties,                                                                            
#workclass=Private,                                                                       
#race=White,                                                                              
#sex=Female,                                                                              
#capital-gain=zero,                                                                       
#native-country=United-States} => {marital-status=Divorced} 0.01158679  0.3616299 2.166832
#463 {age=fourties,                                                                            
#workclass=Private,                                                                       
#race=White,                                                                              
#sex=Female,                                                                              
#capital-loss=zero,                                                                       
#native-country=United-States} => {marital-status=Divorced} 0.01174999  0.3517915 2.107883

v <- rulesExperiment(AdultUCITR, minSupport = 0.04, minConfidence = 0.1, minLift = 1.5, consequent = "marital-status=Divorced", subsetF = "race=White")
#Znaleziono 46 reguł wszystkie o zaufaniu pomiędzy 0.2 a 0.3. Przyjmujemy więc minimalne zaufanie 0.2 i możemy nieco zwiększyć minimalne wsparcie do 0.05. 
#Przykłady znalezionych reguł:
#40 {workclass=Private,                                                                       
#race=White,                                                                              
#sex=Female,                                                                              
#capital-loss=zero,                                                                       
#native-country=United-States} => {marital-status=Divorced} 0.04117935  0.2751727 1.648794
#41 {workclass=Private,                                                                       
#race=White,                                                                              
#sex=Female,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero}            => {marital-status=Divorced} 0.04281129  0.2616356 1.567682
#42 {sex=Female,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero,                                                                       
#hours-per-week=lower-half,                                                               
#native-country=United-States} => {marital-status=Divorced} 0.04330088  0.2629666 1.575657
#43 {race=White,                                                                              
#sex=Female,                                                                              
#capital-gain=zero,                                                                       
#hours-per-week=lower-half,                                                               
#native-country=United-States} => {marital-status=Divorced} 0.04689115  0.2609749 1.563723
#44 {race=White,                                                                              
#sex=Female,                                                                              
#capital-loss=zero,                                                                       
#hours-per-week=lower-half,                                                               
#native-country=United-States} => {marital-status=Divorced} 0.04884948  0.2578237 1.544841
#45 {race=White,                                                                              
#sex=Female,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero,                                                                       
#native-country=United-States} => {marital-status=Divorced} 0.06217701  0.2790527 1.672043
#46 {race=White,                                                                              
#sex=Female,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero,                                                                       
#hours-per-week=lower-half,                                                               
#native-country=United-States} => {marital-status=Divorced} 0.04330088  0.2629666 1.575657

v <- rulesExperiment(AdultUCITR, minSupport = 0.05, minConfidence = 0.2, minLift = 1.5, consequent = "marital-status=Divorced", subsetF = "race=White")
#Odnaleziono 20 reguł o wsparciu pomiędzy 0.05 a 0.09 i wsp. podniesienia między 1.6 a 1.8. Te reguły wykorzystamy do analizy porównawczej. 
# Pełna lista odnalezionych reguł:
# 1  {sex=Female}                   => {marital-status=Divorced} 0.08589458  0.2611212 1.564600
# 2  {workclass=Private,                                                                       
# sex=Female}                   => {marital-status=Divorced} 0.05189577  0.2583266 1.547854
# 3  {sex=Female,                                                                              
# native-country=United-States} => {marital-status=Divorced} 0.07599413  0.2735997 1.639369
# 4  {sex=Female,                                                                              
# capital-gain=zero}            => {marital-status=Divorced} 0.07686449  0.2638655 1.581043
# 5  {sex=Female,                                                                              
# capital-loss=zero}            => {marital-status=Divorced} 0.08050917  0.2619005 1.569269
# 6  {race=White,                                                                              
# sex=Female}                   => {marital-status=Divorced} 0.08589458  0.2611212 1.564600
# 7  {workclass=Private,                                                                       
# race=White,                                                                              
# sex=Female}                   => {marital-status=Divorced} 0.05189577  0.2583266 1.547854
# 8  {sex=Female,                                                                              
# hours-per-week=lower-half,                                                               
# native-country=United-States} => {marital-status=Divorced} 0.05243975  0.2564512 1.536617
# 9  {sex=Female,                                                                              
# capital-gain=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.06745363  0.2777778 1.664403
# 10 {sex=Female,                                                                              
# capital-loss=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.07071751  0.2743774 1.644028
# 11 {race=White,                                                                              
# sex=Female,                                                                              
# native-country=United-States} => {marital-status=Divorced} 0.07599413  0.2735997 1.639369
# 12 {sex=Female,                                                                              
# capital-gain=zero,                                                                       
# capital-loss=zero}            => {marital-status=Divorced} 0.07147908  0.2649728 1.587677
# 13 {race=White,                                                                              
# sex=Female,                                                                              
# capital-gain=zero}            => {marital-status=Divorced} 0.07686449  0.2638655 1.581043
# 14 {race=White,                                                                              
# sex=Female,                                                                              
# capital-loss=zero}            => {marital-status=Divorced} 0.08050917  0.2619005 1.569269
# 15 {race=White,                                                                              
# sex=Female,                                                                              
# hours-per-week=lower-half,                                                               
# native-country=United-States} => {marital-status=Divorced} 0.05243975  0.2564512 1.536617
# 16 {sex=Female,                                                                              
# capital-gain=zero,                                                                       
# capital-loss=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.06217701  0.2790527 1.672043
# 17 {race=White,                                                                              
# sex=Female,                                                                              
# capital-gain=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.06745363  0.2777778 1.664403
# 18 {race=White,                                                                              
# sex=Female,                                                                              
# capital-loss=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.07071751  0.2743774 1.644028
# 19 {race=White,                                                                              
# sex=Female,                                                                              
# capital-gain=zero,                                                                       
# capital-loss=zero}            => {marital-status=Divorced} 0.07147908  0.2649728 1.587677
# 20 {race=White,                                                                              
# sex=Female,                                                                              
# capital-gain=zero,                                                                       
# capital-loss=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.06217701  0.2790527 1.672043

#########################
### ETNICZNOŚĆ CZARNA ###
#########################

#Eksperyment początkowy
v <- rulesExperiment(AdultUCITR, minSupport = 0.04, minConfidence = 0.1, minLift = 1.5, consequent = "marital-status=Divorced", subsetF = "race=Black")
#Nie znaleziono żadnych reguł o zadanych parametrach. Nie możemy zmniejszyć współczynnika wsparcia, by nie stracić statystycznej istotności - zatem obniżamy współczynnik podniesienia do 1.1. 

v <- rulesExperiment(AdultUCITR, minSupport = 0.04, minConfidence = 0.1, minLift = 1.1, consequent = "marital-status=Divorced", subsetF = "race=Black")
#Znaleziono 174 reguły. Możemy podnieść wsp. podniesienia do 1.3. 
#Przykłady znalezionych reguł:
#152 {race=Black,                                                                              
#capital-gain=zero,                                                                       
#hours-per-week=lower-half,                                                               
#native-country=United-States,                                                            
#income=small}                 => {marital-status=Divorced} 0.07196178  0.1945117 1.173729
#153 {race=Black,                                                                              
#capital-loss=zero,                                                                       
#hours-per-week=lower-half,                                                               
#native-country=United-States,                                                            
#income=small}                 => {marital-status=Divorced} 0.07166318  0.1920000 1.158573
#154 {race=Black,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero,                                                                       
#hours-per-week=lower-half,                                                               
#income=small}                 => {marital-status=Divorced} 0.07554494  0.1833333 1.106276
#155 {race=Black,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero,                                                                       
#native-country=United-States,                                                            
#income=small}                 => {marital-status=Divorced} 0.08271126  0.1942496 1.172148
#156 {sex=Female,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero,                                                                       
#hours-per-week=lower-half,                                                               
#native-country=United-States,                                                            
#income=small}                 => {marital-status=Divorced} 0.04538668  0.2363919 1.426444
#157 {race=Black,                                                                              
#sex=Female,                                                                              
#capital-gain=zero,                                                                       
#hours-per-week=lower-half,                                                               
#native-country=United-States,                                                            
#income=small}                 => {marital-status=Divorced} 0.04777546  0.2377415 1.434588

v <- rulesExperiment(AdultUCITR, minSupport = 0.04, minConfidence = 0.1, minLift = 1.3, consequent = "marital-status=Divorced", subsetF = "race=Black")
#Znaleziono 118 reguł, większość ma wsp. podniesienia tylko nieco mniejszy od 1.5. Zmieniamy wsp. podniesienia do 1.4 i możemy też podnieść zakładane wsparcie do 0.05. Zaufanie wszystkich reguł jest pomiędzy 0.2 a 0.3, możemy więc zwiększyc zakładane zaufanie do 0.2. 
#Przykłady znalezionych reguł:
#107 {sex=Female,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero,                                                                       
#hours-per-week=lower-half,                                                               
#native-country=United-States,                                                            
#income=small}                 => {marital-status=Divorced} 0.04538668  0.2363919 1.426444
#108 {race=Black,                                                                              
#sex=Female,                                                                              
#capital-gain=zero,                                                                       
#hours-per-week=lower-half,                                                               
#native-country=United-States,                                                            
#income=small}                 => {marital-status=Divorced} 0.04777546  0.2377415 1.434588
#109 {race=Black,                                                                              
#sex=Female,                                                                              
#capital-loss=zero,                                                                       
#hours-per-week=lower-half,                                                               
#native-country=United-States,                                                            
#income=small}                 => {marital-status=Divorced} 0.04747686  0.2338235 1.410946
#110 {race=Black,                                                                              
#sex=Female,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero,                                                                       
#hours-per-week=lower-half,                                                               
#income=small}                 => {marital-status=Divorced} 0.04956703  0.2246279 1.355457
#111 {race=Black,                                                                              
#sex=Female,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero,                                                                       
#native-country=United-States,                                                            
#income=small}                 => {marital-status=Divorced} 0.05434458  0.2469471 1.490137
#112 {workclass=Private,                                                                       
#sex=Female,                                                                              
#capital-gain=zero,                                                                       
#capital-loss=zero,                                                                       
#hours-per-week=lower-half,                                                               
#native-country=United-States} => {marital-status=Divorced} 0.04389370  0.2244275 1.354248

v <- rulesExperiment(AdultUCITR, minSupport = 0.05, minConfidence = 0.2, minLift = 1.4, consequent = "marital-status=Divorced", subsetF = "race=Black")
#Odnaleziono 36 reguł o wsparciu pomiędzy 0.05 a 0.10 i wsp. podniesienia 1.4 do 1.5. Te reguły wykorzystamy do analizy porównawczej. 
# Pełna lista odnalezionych reguł:
# 1  {age=fourties,                                                                            
# native-country=United-States} => {marital-status=Divorced} 0.05135861  0.2330623 1.406353
# 2  {sex=Female,                                                                              
# income=small}                 => {marital-status=Divorced} 0.06419827  0.2321814 1.401037
# 3  {sex=Female,                                                                              
# native-country=United-States} => {marital-status=Divorced} 0.10032846  0.2393162 1.444090
# 4  {age=fourties,                                                                            
# race=Black,                                                                              
# native-country=United-States} => {marital-status=Divorced} 0.05135861  0.2330623 1.406353
# 5  {sex=Female,                                                                              
# native-country=United-States,                                                            
# income=small}                 => {marital-status=Divorced} 0.05912213  0.2435424 1.469592
# 6  {sex=Female,                                                                              
# capital-gain=zero,                                                                       
# income=small}                 => {marital-status=Divorced} 0.06151090  0.2343572 1.414166
# 7  {sex=Female,                                                                              
# capital-loss=zero,                                                                       
# income=small}                 => {marital-status=Divorced} 0.06180950  0.2323232 1.401893
# 8  {race=Black,                                                                              
# sex=Female,                                                                              
# income=small}                 => {marital-status=Divorced} 0.06419827  0.2321814 1.401037
# 9  {workclass=Private,                                                                       
# sex=Female,                                                                              
# native-country=United-States} => {marital-status=Divorced} 0.06151090  0.2365098 1.427155
# 10 {sex=Female,                                                                              
# hours-per-week=lower-half,                                                               
# native-country=United-States} => {marital-status=Divorced} 0.08360705  0.2325581 1.403310
# 11 {sex=Female,                                                                              
# capital-gain=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.09256494  0.2364607 1.426859
# 12 {sex=Female,                                                                              
# capital-loss=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.09555091  0.2388060 1.441011
# 13 {race=Black,                                                                              
# sex=Female,                                                                              
# native-country=United-States} => {marital-status=Divorced} 0.10032846  0.2393162 1.444090
# 14 {sex=Female,                                                                              
# capital-gain=zero,                                                                       
# native-country=United-States,                                                            
# income=small}                 => {marital-status=Divorced} 0.05673335  0.2470741 1.490903
# 15 {sex=Female,                                                                              
# capital-loss=zero,                                                                       
# native-country=United-States,                                                            
# income=small}                 => {marital-status=Divorced} 0.05673335  0.2432778 1.467996
# 16 {race=Black,                                                                              
# sex=Female,                                                                              
# native-country=United-States,                                                            
# income=small}                 => {marital-status=Divorced} 0.05912213  0.2435424 1.469592
# 17 {sex=Female,                                                                              
# capital-gain=zero,                                                                       
# capital-loss=zero,                                                                       
# income=small}                 => {marital-status=Divorced} 0.05912213  0.2345972 1.415614
# 18 {race=Black,                                                                              
# sex=Female,                                                                              
# capital-gain=zero,                                                                       
# income=small}                 => {marital-status=Divorced} 0.06151090  0.2343572 1.414166
# 19 {race=Black,                                                                              
# sex=Female,                                                                              
# capital-loss=zero,                                                                       
# income=small}                 => {marital-status=Divorced} 0.06180950  0.2323232 1.401893
# 20 {workclass=Private,                                                                       
# sex=Female,                                                                              
# capital-gain=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.05673335  0.2345679 1.415438
# 21 {workclass=Private,                                                                       
# sex=Female,                                                                              
# capital-loss=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.05882353  0.2367788 1.428779
# 22 {workclass=Private,                                                                       
# race=Black,                                                                              
# sex=Female,                                                                              
# native-country=United-States} => {marital-status=Divorced} 0.06151090  0.2365098 1.427155
# 23 {race=Black,                                                                              
# sex=Female,                                                                              
# hours-per-week=lower-half,                                                               
# native-country=United-States} => {marital-status=Divorced} 0.08360705  0.2325581 1.403310
# 24 {sex=Female,                                                                              
# capital-gain=zero,                                                                       
# capital-loss=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.08778740  0.2357658 1.422666
# 25 {race=Black,                                                                              
# sex=Female,                                                                              
# capital-gain=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.09256494  0.2364607 1.426859
# 26 {race=Black,                                                                              
# sex=Female,                                                                              
# capital-loss=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.09555091  0.2388060 1.441011
# 27 {sex=Female,                                                                              
# capital-gain=zero,                                                                       
# capital-loss=zero,                                                                       
# native-country=United-States,                                                            
# income=small}                 => {marital-status=Divorced} 0.05434458  0.2469471 1.490137
# 28 {race=Black,                                                                              
# sex=Female,                                                                              
# capital-gain=zero,                                                                       
# native-country=United-States,                                                            
# income=small}                 => {marital-status=Divorced} 0.05673335  0.2470741 1.490903
# 29 {race=Black,                                                                              
# sex=Female,                                                                              
# capital-loss=zero,                                                                       
# native-country=United-States,                                                            
# income=small}                 => {marital-status=Divorced} 0.05673335  0.2432778 1.467996
# 30 {race=Black,                                                                              
# sex=Female,                                                                              
# capital-gain=zero,                                                                       
# capital-loss=zero,                                                                       
# income=small}                 => {marital-status=Divorced} 0.05912213  0.2345972 1.415614
# 31 {workclass=Private,                                                                       
# sex=Female,                                                                              
# capital-gain=zero,                                                                       
# capital-loss=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.05404598  0.2347601 1.416597
# 32 {workclass=Private,                                                                       
# race=Black,                                                                              
# sex=Female,                                                                              
# capital-gain=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.05673335  0.2345679 1.415438
# 33 {workclass=Private,                                                                       
# race=Black,                                                                              
# sex=Female,                                                                              
# capital-loss=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.05882353  0.2367788 1.428779
# 34 {race=Black,                                                                              
# sex=Female,                                                                              
# capital-gain=zero,                                                                       
# capital-loss=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.08778740  0.2357658 1.422666
# 35 {race=Black,                                                                              
# sex=Female,                                                                              
# capital-gain=zero,                                                                       
# capital-loss=zero,                                                                       
# native-country=United-States,                                                            
# income=small}                 => {marital-status=Divorced} 0.05434458  0.2469471 1.490137
# 36 {workclass=Private,                                                                       
# race=Black,                                                                              
# sex=Female,                                                                              
# capital-gain=zero,                                                                       
# capital-loss=zero,                                                                       
# native-country=United-States} => {marital-status=Divorced} 0.05404598  0.2347601 1.416597


#########################################
####            Wnioski              ####
#########################################

# Okazuje się, że niezależnie od etniczności, najsilniejszym predyktorem bycia osobą rozwiedzioną jest bycie kobietą. Niemal wszystkie znalezione reguły posiadają w poprzedniku płeć żeńską. Nie da się tego faktu wytłumaczyć jedynie niereprezentatywnością posiadanych danych: dla etniczności białej kobiety stanowią 33% badanych, a dla etniczności czarnej 47%. Zatem rozwódkami są prawie wyłącznie kobiety. Potencjalne wyjaśnienie tego faktu może opierać się na teorii socjalizacji do narzuconych kulturowo ról płciowych - być może mężczyźni socjalizowani są do nieakcetowania własnych porażek i samych siebie jako osób przegranych, tymczasem rozwód jawi się niemal zawsze jako osobistwa i życiowa porażka. Tak socjalizowani meżczyźni odczuwają presję kulturową na zmianę swego stanu i szybkie znalezienie nowej partnerki po zakończeniu poprzewdniego związku. Natomiast samotne kobiety niekoniecznie muszą być postrzegane jako nieakceptowane - może sie podziwiać je za wytrwałość i wysiłek ponoszony przy samotnym wychowywaniu dziecka. Stereotyp samotnej matki - w porównaniu do steretypu samotnego ojca, który jest niemal nieobecny w europejskiej kulturze - wciąż budzi raczej współczucie i chęć pomocy niż zażenowanie i pogardę. Wszystkie te konstrukty kulturowe mogą wyjaśniać uzyskane wyniki. 

# W obu etnicznościach często pojawiającym się predyktorem rozwodu jest narodowość: Stany Zjednoczone, może to jednak wynikać w faktu, że osoby narodowości amerykańskiej stanowią 84% badanych osób i osoby z innych krajów nie spełniły warunku minimalnego wsparcia. Podobnie rzecz ma się dla dwóch wskaźników "capital-gain=zero" i "capital-loss=zero", które traktowane zbiorczo oznaczają że dana osoba nie inwestuje swoich oszczędności na giełdzie. Ponieważ zdecydowana większość badanych osób nie inwestuje oszczędności, osoby inwestujące mogły nie spełnić warunku na minimalne wsparcie. 

# Dla kobiet białej etniczności pojawiającym się predyktorem jest praca w sektorze prywatnym. Zjawisko to można wyjaśnić, odwołując się do psychologicznych mechanizmów obronnych. W systemie kapitalistycznym, pracodawca premiuje osoby szczególnie premiuje osoby poświęcające wiele czasu i wysiłku swojej pracy zawodowej. Ponieważ rozwód jest zawsze zjawiskiem bolesnym, rozwiedzione kobiety mogą kompensować uczucie smutku poprzez zwiększone zaangażowanie w prace zawodową. W sektorze prywatnym jest to o tyle łatwe, że za zwiększone zaangażowanie otrzymują akceptację i pochwały ze strony przełożonych. Jest to mechanizm obronny służący wyparciu nieprzyjemnych odczuć o rozwodzie i życiu osobistym do poświadomości. Skutkuje on rezygnacją bądź ograniczeniem poszukiwań nowego partnera po rozpadzie poprzedniego związku. 

# Dla kobiet etniczności czarnej częstym predyktorem rozwodu jest za to niski dochód. Zjawisko to częściowo można wyjaśniać rozwarstwieniem społęcznym w Stanach Zjednoczonych - czarne kobiety zarabiają ok. 20% mniej niż białe. Można jednak również rozważać ogólny kapitał kulturowy i społeczny poszczególnych osób. Te o niższym kapitale społecznym, intelektualnym i kulturowym będą częściej zarabiać poniżej średniej, jak i będą mieć trudności w radzeniu sobie w sytuacjach kryzysowych takich jak rozwód i wybierać mniej adaptacyjne sposoby radzenia sobie z emocjami. Wynikające stąd problemy psychiczne i nieumiejętność samodzielnego radzenia sobie z nimi mogą być czynnikami utrudniającymi znalezienie kolejnego partnera po rozpadzie poprzedniego związku. 

# Podsumowując, zarówno bardzo wysokie jak i bardzo niskie kompetencje intelektualne, zawodowe i społeczne mogą być czynnikami wpływającymi na dłuższe pozostawanie w stanie rozwiedzionym. 



## Artur M. Brodzki, 26.11.2017 r. ##





