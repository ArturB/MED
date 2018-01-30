#####################################
#### MED2017Z Reguły sekwencyjne ####
####       Adam Małkowski        ####
####      Artur M. Brodzki       ####
#####################################

# Cel eksperymentu: Szukane reguły sekwencyjne będą znajdować zależności w wyniku których dochodzi do hipoglikemii - co może pomóc diabetykom uniknąć hioglikemii. Poprzednik reguły będzie złożony z informacji nt. podanej insuliny, aktywności fizycznej oraz posiłków - nie będą wykorzystywane informacje nt. pomiarów stanu glukozy.
# Interesujące nas reguły sekwencyjne będą się charakteryzować następującymi parametrami:
# - współczynnik podniesienia > 1.1 - aby była dodatnia korelacja pomiędzy wystąpieniem poprzednika i następnika reguły
# - zaufanie co najmniej 0.2 - problem jest na tyle istotny - tj. wystąpienie hipoglikemii może nieść bardzo poważne konsekwencje - że rozsądnie jest analizować reguły nawet o dość małym zaufaniu. 
# - wsparcie: nieprzekraczalny próg dolny wynosi co najmniej 0.003 - tak, aby reguła miała odniesienie do co najmiej 30 zdarzeń. Będziemy jednak silnie preferować reguły o większym wsparciu, ponieważ dają się one często stosować w praktyce. 
# - reguły nie powinny być też zbyt skomplikowane, aby potencjalnie mogły być zapamiętane i stosowane w praktyce diagnostycznej i profilaktycznej. Przyjmujemy maksymalną długość reguły równą 4 i maksymalną złożoność składnika reguły też równą 4. 
# - istotny jest dobór maksymalnego i minimalnego okna czasowego dla składowych reguł. Uwzględniając specyfikę i tempo metabolizmu w ludzkim organiźmie, nie ma uzasadnienia przyjmowanie okna czasowego krótszego niż 10 min (600 sek). Nie ma również sensu przyjmowanie wartości większej niż ok. 36 godzin (129.600 sek), jako, że zasadniczo pełen cykl metabolizmu u człowieka trwa 24 godziny - zostawiamy sobie pewien margines na wypadek niedokładnych danych. 
# Najlepsze reguły to takie, które mają możliwie największe wsparcie, zaufanie oraz mają sensowną interpretacje dziedzinową - np. związek między poprzednikiem a następnikiem powinien nie wynikać z definicji tych zdarzeń 

# W ramach tego eksperymentu termin zdarzenie odnosi się do wystapienia kodu zdarzenia - w związku z zawartością informacji ilościowych w kodach kolumna z wartościami została pominięta

###################################
####   Ustawienia początkowe   ####
###################################

#Ładowanie bibliotek
library(arulesSequences)

# Wstępne przygotowanie danych
download.file('http://staff.ii.pw.edu.pl/~gprotazi/dydaktyka/dane/diab_trans.data','diab_trans.data')
diab.df <- read.csv("diab_trans.data", header=TRUE, stringsAsFactors = FALSE)
# Usuniecie kolumny wartość
diab.df$value <- NULL
# Usunięcie zdarzeń będących pomiarem poziomu glukozy we krwi
diab.df <- subset(diab.df, code %in% c('id_33',
                                       'id_34',
                                       'id_35',
                                       'id_65',
                                       'id_66',
                                       'id_67',
                                       'id_68',
                                       'id_69',
                                       'id_70',
                                       'id_71',
                                       'id_72')) 
#Konwersja danych do postaci transakcyjnej
write.table(diab.df, "diab_trans2.data", sep = "," , row.names = FALSE, col.names = FALSE )
diabSeq <- read_baskets(con = "diab_trans2.data", sep =",", info = c("sequenceID","eventID"))

# Funkcja znajdująca interesujące reguły:
generateInerestingRules <- function(
  support_param, 
  confidence_param, 
  mingap_param, 
  maxgap_param, 
  maxlen_param, 
  maxsize_param)
{
  seqParam = new ("SPparameter", 
                  support = support_param, 
                  maxsize = maxsize_param, 
                  mingap = mingap_param, 
                  maxgap = maxgap_param, 
                  maxlen = maxlen_param )
  patSeq = cspade(diabSeq, 
                  seqParam, 
                  control = list(verbose = TRUE, 
                                 tidLists = FALSE, 
                                 summary = FALSE))
  seqRules = ruleInduction(patSeq, confidence = confidence_param)
  
  result <- subset(seqRules,     (lhs(x) %pin% "id_33" 
                                  | lhs(x) %pin% "id_34" 
                                  | lhs(x) %pin% "id_35" 
                                  | lhs(x) %pin% "id_66"
                                  | lhs(x) %pin% "id_67"
                                  | lhs(x) %pin% "id_68"
                                  | lhs(x) %pin% "id_69"
                                  | lhs(x) %pin% "id_70"
                                  | lhs(x) %pin% "id_71"
                                  | lhs(x) %pin% "id_72") 
                   & rhs(x) %pin% "id_65" 
                   & lift > 1.0)
}

#############################
####     Eksperymenty    ####
#############################

#Definicja parametrów
test_count = 14

support_test =    	c(0.01, 0.05, 0.1, 0.2, 0.3, 0.15, 0.15, 0.15, 0.2, 0.2, 0.2, 0.3, 0.3, 0.3)
confidence_test = 	c(0.90, 0.90, 0.40, 0.40, 0.20, 0.6, 0.7, 0.8, 0.6, 0.7, 0.8, 0.6, 0.7, 0.8)
mingap_test = 		600
maxgap_test = 		129600
maxlen_test = 		4
maxsize_test = 		4

selected_rules <- list()

for(i in 1:test_count)
{
  selected_rules[i] = generateInerestingRules(support_test[i], confidence_test[i], mingap_test, maxgap_test, maxlen_test, maxsize_test)
}

#Analizujemy reguły znalezione w kolejnych testach. 
for(i in 1:test_count){
  #print(length(selected_rules[[i]]))
  inspect(head(selected_rules[[i]], 15))
}

#######################
####    Wnioski    ####
#######################

# 1. Okazuje się, że jednym z najsilniejszych predyktorów wystąpienia hipoglikemii jest wystąpienie hipoglikemii wcześniej (zdarzenie 65). Reguły z hipoglikemią w poprzedniku mają bardzo wysokie zaufanie - rzędu >0.9. 
# 2. Innym silnym predyktorem (zaufanie 70-80%) wystąpienia hipoglikemii jest przyjmowanie dawek insuliny: "NPH insulin dose" oraz "Regular insulin dose". Interesujące jest, że nie odnaleziono niemal wcale reguł z insuliną UltraLente. Wynika to zapewne z faktu, że UltraLente jest łagodniejszą, dłużej działającą formą insuliny a NPH to insulina szybciej podlegająca metabolizmowi w organizmie. 
# 3. Również często w poprzednikach reguł występuje "More-than-usual meal ingestion" - sugeruje to uważać na hipoglikemie w przypadku sporzywania większej ilości pokarmów w ciągu 1-2 dni. Wartość "Typical meal ingestion" występowała rzadziej (ale jednak standardowy tryb jedzenia nie chroni przed wystąpieniem hipoglikemii). 
# 4. "Unspecified special event" występuje w malej liczbie reguł (około 10%), ale za to najczęściej jest silnym predyktorem hipoglikemii. Ponieważ nie znamy charakteru tego zdarzenia, nie możemy mieć pewności z czego wynikają takie reguły - ale należy podejrzewać, że "Special event" został odnotowany w danych ponieważ jest istotny w kontekście cukrzycy i dlatego koreluje z hipoglikemią. 

######################################
#### Przykłady uzyskanych wyników ####
######################################

# support 0.01 condifence 0.90
# lhs            rhs            support confidence     lift 
# 1 <{"id_66"},                   
# {"id_33"},                   
# {"id_72"}> => <{"id_65"}> 0.04615385        1.0 1.756757 
# 2 <{"id_66"},                   
# {"id_34"},                   
# {"id_72"}> => <{"id_65"}> 0.04615385        1.0 1.756757 
# 3 <{"id_69"},                   
# {"id_34"},                   
# {"id_72"}> => <{"id_65"}> 0.01538462        1.0 1.756757 
# 4 <{"id_66"},                   
# {"id_72"}> => <{"id_65"}> 0.04615385        1.0 1.756757 
# 5 <{"id_33"},                   
# {"id_66"},                   
# {"id_72"}> => <{"id_65"}> 0.04615385        1.0 1.756757 
# 6 <{"id_34"},                   
# {"id_66"},                   
# {"id_72"}> => <{"id_65"}> 0.04615385        1.0 1.756757 
# 7 <{"id_66"},                   
# {"id_66"},                   
# {"id_72"}> => <{"id_65"}> 0.01538462        1.0 1.756757 
# 8 <{"id_69"},                   
# {"id_66"},                   
# {"id_72"}> => <{"id_65"}> 0.01538462        1.0 1.756757 
# 9 <{"id_70"},                   
# {"id_66"},                   
# {"id_72"}> => <{"id_65"}> 0.01538462        1.0 1.756757 
# 10 <{"id_71"},                   
# {"id_66"},                   
# {"id_72"}> => <{"id_65"}> 0.03076923        1.0 1.756757 
# 11 <{"id_66"},                   
# {"id_67"},                   
# {"id_72"}> => <{"id_65"}> 0.04615385        1.0 1.756757 
# 12 <{"id_68"},                   
# {"id_67"},                   
# {"id_72"}> => <{"id_65"}> 0.01538462        1.0 1.756757 
# 13 <{"id_34"},                   
# {"id_69"},                   
# {"id_72"}> => <{"id_65"}> 0.01538462        1.0 1.756757 
# 14 <{"id_66"},                   
# {"id_69"},                   
# {"id_72"}> => <{"id_65"}> 0.01538462        1.0 1.756757 
# 15 <{"id_69"},                   
# {"id_69"},                   
# {"id_72"}> => <{"id_65"}> 0.01538462        1.0 1.756757 
# 
# support 0.05 condifence 0.90
# lhs            rhs            support confidence     lift 
# 1 <{"id_65"},                   
# {"id_33"},                   
# {"id_71"}> => <{"id_65"}> 0.13846154        0.9 1.581081 
# 2 <{"id_65"},                   
# {"id_34"},                   
# {"id_71"}> => <{"id_65"}> 0.13846154        0.9 1.581081 
# 3 <{"id_66"},                   
# {"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.07692308        1.0 1.756757 
# 4 <{"id_66"},                   
# {"id_34"},                   
# {"id_65"}> => <{"id_65"}> 0.06153846        1.0 1.756757 
# 5 <{"id_66"},                   
# {"id_65"}> => <{"id_65"}> 0.06153846        1.0 1.756757 
# 6 <{"id_33"},                   
# {"id_66"},                   
# {"id_65"}> => <{"id_65"}> 0.06153846        1.0 1.756757 
# 7 <{"id_69"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.06153846        1.0 1.756757 
# 8 <{"id_66"},                   
# {"id_65"},                   
# {"id_33"}> => <{"id_65"}> 0.06153846        1.0 1.756757 
# 9 <{"id_69"},                   
# {"id_65"},                   
# {"id_33"}> => <{"id_65"}> 0.06153846        1.0 1.756757 
# 
# support 0.1 condifence 0.40
# lhs            rhs           support confidence     lift 
# 1 <{"id_65"},                   
# {"id_33"},                   
# {"id_71"}> => <{"id_65"}> 0.1384615  0.9000000 1.581081 
# 2 <{"id_33"},                   
# {"id_34"},                   
# {"id_71"}> => <{"id_65"}> 0.1692308  0.5789474 1.017070 
# 3 <{"id_34"},                   
# {"id_34"},                   
# {"id_71"}> => <{"id_65"}> 0.1692308  0.5789474 1.017070 
# 4 <{"id_65"},                   
# {"id_34"},                   
# {"id_71"}> => <{"id_65"}> 0.1384615  0.9000000 1.581081 
# 5 <{"id_65"},                   
# {"id_33"},                   
# {"id_70"}> => <{"id_65"}> 0.1076923  0.7000000 1.229730 
# 6 <{"id_34"},                   
# {"id_70"}> => <{"id_65"}> 0.1538462  0.5882353 1.033386 
# 7 <{"id_65"},                   
# {"id_34"},                   
# {"id_70"}> => <{"id_65"}> 0.1076923  0.7000000 1.229730 
# 8 <{"id_65"},                   
# {"id_33"},                   
# {"id_67"}> => <{"id_65"}> 0.1846154  0.7058824 1.240064 
# 9 <{"id_34"},                   
# {"id_67"}> => <{"id_65"}> 0.2000000  0.5909091 1.038084 
# 10 <{"id_33"},                   
# {"id_34"},                   
# {"id_67"}> => <{"id_65"}> 0.2000000  0.5909091 1.038084 
# 11 <{"id_34"},                   
# {"id_34"},                   
# {"id_67"}> => <{"id_65"}> 0.2000000  0.6190476 1.087516 
# 12 <{"id_65"},                   
# {"id_34"},                   
# {"id_67"}> => <{"id_65"}> 0.1692308  0.7857143 1.380309 
# 13 <{"id_72"},                   
# {"id_34"},                   
# {"id_67"}> => <{"id_65"}> 0.1230769  0.7272727 1.277641 
# 14 <{"id_65"},                   
# {"id_67"}> => <{"id_65"}> 0.1846154  0.6666667 1.171171 
# 15 <{"id_33"},                   
# {"id_65"},                   
# {"id_67"}> => <{"id_65"}> 0.1846154  0.7058824 1.240064 
# 
# support 0.2 condifence 0.40
# lhs            rhs           support confidence     lift 
# 1 <{"id_34"},                   
# {"id_67"}> => <{"id_65"}> 0.2000000  0.5909091 1.038084 
# 2 <{"id_33"},                   
# {"id_34"},                   
# {"id_67"}> => <{"id_65"}> 0.2000000  0.5909091 1.038084 
# 3 <{"id_34"},                   
# {"id_34"},                   
# {"id_67"}> => <{"id_65"}> 0.2000000  0.6190476 1.087516 
# 4 <{"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.3076923  0.6060606 1.064701 
# 5 <{"id_33"},                   
# {"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.3076923  0.6060606 1.064701 
# 6 <{"id_34"},                   
# {"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.2769231  0.6666667 1.171171 
# 7 <{"id_34"},                   
# {"id_65"}> => <{"id_65"}> 0.2769231  0.6666667 1.171171 
# 8 <{"id_33"},                   
# {"id_34"},                   
# {"id_65"}> => <{"id_65"}> 0.2769231  0.6666667 1.171171 
# 9 <{"id_34"},                   
# {"id_34"},                   
# {"id_65"}> => <{"id_65"}> 0.2769231  0.6666667 1.171171 
# 10 <{"id_65"},                   
# {"id_33"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 11 <{"id_71"},                   
# {"id_33"},                   
# {"id_34"}> => <{"id_65"}> 0.2153846  0.6363636 1.117936 
# 12 <{"id_65"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 13 <{"id_67"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.2153846  0.6086957 1.069330 
# 14 <{"id_71"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.2000000  0.5909091 1.038084 
# 15 <{"id_72"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.2000000  0.5909091 1.038084 
# 
# support 0.3 condifence 0.20
# lhs            rhs           support confidence     lift 
# 1 <{"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.3076923  0.6060606 1.064701 
# 2 <{"id_33"},                   
# {"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.3076923  0.6060606 1.064701 
# 3 <{"id_65"},                   
# {"id_33"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 4 <{"id_65"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 5 <{"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.7777778 1.366366 
# 6 <{"id_33"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 7 <{"id_34"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 8 <{"id_65"},                   
# {"id_33"},                   
# {"id_33"}> => <{"id_65"}> 0.3692308  0.7272727 1.277641 
# 9 <{"id_65"},                   
# {"id_34"},                   
# {"id_33"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 10 <{"id_65"},                   
# {"id_33"}> => <{"id_65"}> 0.3538462  0.6969697 1.224406 
# 11 <{"id_33"},                   
# {"id_65"},                   
# {"id_33"}> => <{"id_65"}> 0.3538462  0.7187500 1.262669 
# 12 <{"id_34"},                   
# {"id_65"},                   
# {"id_33"}> => <{"id_65"}> 0.3230769  0.7777778 1.366366 
# 
# support 0.15 condifence 0.6
# lhs            rhs           support confidence     lift 
# 1 <{"id_65"},                   
# {"id_33"},                   
# {"id_67"}> => <{"id_65"}> 0.1846154  0.7058824 1.240064 
# 2 <{"id_34"},                   
# {"id_34"},                   
# {"id_67"}> => <{"id_65"}> 0.2000000  0.6190476 1.087516 
# 3 <{"id_65"},                   
# {"id_34"},                   
# {"id_67"}> => <{"id_65"}> 0.1692308  0.7857143 1.380309 
# 4 <{"id_65"},                   
# {"id_67"}> => <{"id_65"}> 0.1846154  0.6666667 1.171171 
# 5 <{"id_33"},                   
# {"id_65"},                   
# {"id_67"}> => <{"id_65"}> 0.1846154  0.7058824 1.240064 
# 6 <{"id_34"},                   
# {"id_65"},                   
# {"id_67"}> => <{"id_65"}> 0.1846154  0.8571429 1.505792 
# 7 <{"id_34"},                   
# {"id_67"},                   
# {"id_67"}> => <{"id_65"}> 0.1692308  0.6111111 1.073574 
# 8 <{"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.3076923  0.6060606 1.064701 
# 9 <{"id_33"},                   
# {"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.3076923  0.6060606 1.064701 
# 10 <{"id_34"},                   
# {"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.2769231  0.6666667 1.171171 
# 11 <{"id_71"},                   
# {"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.1538462  0.6666667 1.171171 
# 12 <{"id_34"},                   
# {"id_65"}> => <{"id_65"}> 0.2769231  0.6666667 1.171171 
# 13 <{"id_33"},                   
# {"id_34"},                   
# {"id_65"}> => <{"id_65"}> 0.2769231  0.6666667 1.171171 
# 14 <{"id_34"},                   
# {"id_34"},                   
# {"id_65"}> => <{"id_65"}> 0.2769231  0.6666667 1.171171 
# 15 <{"id_71"},                   
# {"id_34"},                   
# {"id_65"}> => <{"id_65"}> 0.1538462  0.7692308 1.351351 
# 
# support 0.15 condifence 0.7
# lhs            rhs           support confidence     lift 
# 1 <{"id_65"},                   
# {"id_33"},                   
# {"id_67"}> => <{"id_65"}> 0.1846154  0.7058824 1.240064 
# 2 <{"id_65"},                   
# {"id_34"},                   
# {"id_67"}> => <{"id_65"}> 0.1692308  0.7857143 1.380309 
# 3 <{"id_33"},                   
# {"id_65"},                   
# {"id_67"}> => <{"id_65"}> 0.1846154  0.7058824 1.240064 
# 4 <{"id_34"},                   
# {"id_65"},                   
# {"id_67"}> => <{"id_65"}> 0.1846154  0.8571429 1.505792 
# 5 <{"id_71"},                   
# {"id_34"},                   
# {"id_65"}> => <{"id_65"}> 0.1538462  0.7692308 1.351351 
# 6 <{"id_65"},                   
# {"id_33"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 7 <{"id_65"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 8 <{"id_70"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.1846154  0.7058824 1.240064 
# 9 <{"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.7777778 1.366366 
# 10 <{"id_33"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 11 <{"id_34"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 12 <{"id_67"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.1538462  0.7692308 1.351351 
# 13 <{"id_65"},                   
# {"id_33"},                   
# {"id_33"}> => <{"id_65"}> 0.3692308  0.7272727 1.277641 
# 14 <{"id_65"},                   
# {"id_34"},                   
# {"id_33"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 15 <{"id_33"},                   
# {"id_65"},                   
# {"id_33"}> => <{"id_65"}> 0.3538462  0.7187500 1.262669 
# 
# support 0.15 condifence 0.8
# lhs            rhs           support confidence     lift 
# 1 <{"id_34"},                   
# {"id_65"},                   
# {"id_67"}> => <{"id_65"}> 0.1846154  0.8571429 1.505792 
# 2 <{"id_65"},                   
# {"id_33"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 3 <{"id_65"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 4 <{"id_33"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 5 <{"id_34"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 6 <{"id_65"},                   
# {"id_34"},                   
# {"id_33"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 
# support 0.2 condifence 0.6
# lhs            rhs           support confidence     lift 
# 1 <{"id_34"},                   
# {"id_34"},                   
# {"id_67"}> => <{"id_65"}> 0.2000000  0.6190476 1.087516 
# 2 <{"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.3076923  0.6060606 1.064701 
# 3 <{"id_33"},                   
# {"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.3076923  0.6060606 1.064701 
# 4 <{"id_34"},                   
# {"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.2769231  0.6666667 1.171171 
# 5 <{"id_34"},                   
# {"id_65"}> => <{"id_65"}> 0.2769231  0.6666667 1.171171 
# 6 <{"id_33"},                   
# {"id_34"},                   
# {"id_65"}> => <{"id_65"}> 0.2769231  0.6666667 1.171171 
# 7 <{"id_34"},                   
# {"id_34"},                   
# {"id_65"}> => <{"id_65"}> 0.2769231  0.6666667 1.171171 
# 8 <{"id_65"},                   
# {"id_33"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 9 <{"id_71"},                   
# {"id_33"},                   
# {"id_34"}> => <{"id_65"}> 0.2153846  0.6363636 1.117936 
# 10 <{"id_65"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 11 <{"id_67"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.2153846  0.6086957 1.069330 
# 12 <{"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.7777778 1.366366 
# 13 <{"id_33"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 14 <{"id_34"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 15 <{"id_65"},                   
# {"id_33"},                   
# {"id_33"}> => <{"id_65"}> 0.3692308  0.7272727 1.277641 
# 
# support 0.2 condifence 0.7
# lhs            rhs           support confidence     lift 
# 1 <{"id_65"},                   
# {"id_33"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 2 <{"id_65"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 3 <{"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.7777778 1.366366 
# 4 <{"id_33"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 5 <{"id_34"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 6 <{"id_65"},                   
# {"id_33"},                   
# {"id_33"}> => <{"id_65"}> 0.3692308  0.7272727 1.277641 
# 7 <{"id_65"},                   
# {"id_34"},                   
# {"id_33"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 8 <{"id_33"},                   
# {"id_65"},                   
# {"id_33"}> => <{"id_65"}> 0.3538462  0.7187500 1.262669 
# 9 <{"id_34"},                   
# {"id_65"},                   
# {"id_33"}> => <{"id_65"}> 0.3230769  0.7777778 1.366366 
# 
# support 0.2 condifence 0.8
# lhs            rhs           support confidence     lift 
# 1 <{"id_65"},                   
# {"id_33"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 2 <{"id_65"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 3 <{"id_33"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 4 <{"id_34"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 5 <{"id_65"},                   
# {"id_34"},                   
# {"id_33"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 
# support 0.3 condifence 0.6
# lhs            rhs           support confidence     lift 
# 1 <{"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.3076923  0.6060606 1.064701 
# 2 <{"id_33"},                   
# {"id_33"},                   
# {"id_65"}> => <{"id_65"}> 0.3076923  0.6060606 1.064701 
# 3 <{"id_65"},                   
# {"id_33"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 4 <{"id_65"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 5 <{"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.7777778 1.366366 
# 6 <{"id_33"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 7 <{"id_34"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 8 <{"id_65"},                   
# {"id_33"},                   
# {"id_33"}> => <{"id_65"}> 0.3692308  0.7272727 1.277641 
# 9 <{"id_65"},                   
# {"id_34"},                   
# {"id_33"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 10 <{"id_65"},                   
# {"id_33"}> => <{"id_65"}> 0.3538462  0.6969697 1.224406 
# 11 <{"id_33"},                   
# {"id_65"},                   
# {"id_33"}> => <{"id_65"}> 0.3538462  0.7187500 1.262669 
# 12 <{"id_34"},                   
# {"id_65"},                   
# {"id_33"}> => <{"id_65"}> 0.3230769  0.7777778 1.366366 
# 
# support 0.3 condifence 0.7
# lhs            rhs           support confidence     lift 
# 1 <{"id_65"},                   
# {"id_33"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 2 <{"id_65"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 3 <{"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.7777778 1.366366 
# 4 <{"id_33"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 5 <{"id_34"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 6 <{"id_65"},                   
# {"id_33"},                   
# {"id_33"}> => <{"id_65"}> 0.3692308  0.7272727 1.277641 
# 7 <{"id_65"},                   
# {"id_34"},                   
# {"id_33"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 8 <{"id_33"},                   
# {"id_65"},                   
# {"id_33"}> => <{"id_65"}> 0.3538462  0.7187500 1.262669 
# 9 <{"id_34"},                   
# {"id_65"},                   
# {"id_33"}> => <{"id_65"}> 0.3230769  0.7777778 1.366366 
# 
# support 0.3 condifence 0.8
# lhs            rhs           support confidence     lift 
# 1 <{"id_65"},                   
# {"id_33"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 2 <{"id_65"},                   
# {"id_34"},                   
# {"id_34"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 
# 3 <{"id_33"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 4 <{"id_34"},                   
# {"id_65"},                   
# {"id_34"}> => <{"id_65"}> 0.3230769  0.8076923 1.418919 
# 5 <{"id_65"},                   
# {"id_34"},                   
# {"id_33"}> => <{"id_65"}> 0.3384615  0.8148148 1.431431 