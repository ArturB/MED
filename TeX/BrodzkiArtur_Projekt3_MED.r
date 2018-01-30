#####################################
####    MED2017Z Klasyfikacja    ####
####       Adam Małkowski        ####
####      Artur M. Brodzki       ####
#####################################

# Klasyfikowane dane: wina białe. 
# Cel klasyfikacji: Uzyskanie klasyfikatora w formie drzewa decyzyjnego, który oceniałby jakość wina na podstawie jego parametrów fizykochemicznych. 
# Przyjmujemy trzy klasy jakości wina, wybrane na podstawie mediany. Ponieważ mediana oceny jakości wina w wykorzystanym zbiorze danych jest równa 6, przyjmujemy następujące klasy jakości:
# - wino słabe (parametr quality={0,1,2,3,4,5})
# - wino średnie (parametr quality={6}) 
# - wino dobre (parametr quality={7,8,9,10}). 
# Walidacja: 65% danych do zbioru treningowego, 35% do zbioru testowego. 
# Wykorzystamy trzy sposoby oceny jakości drzewa jako klasyfikatora:
# - głebokość drzewa - ponieważ szczegółowy pomiar parametrów fizykochemicznych wina może być czasochłonny
# Istotny jest sposob obliczania kosztu pomyłki E: dla poprawnej klasyfikacji E = 0 , dla pomyłki o jedną klasę jakości (dobre <-> średnie lub słabe <-> średnie) E = 1, dla pomyłki o dwie klasy jakości (dobre <-> słabe) E = 2. Oczekiwany koszt pomyłki dla wina ze zbioru testowego nie powinien przekroczyć 0.1. 


# Czyszczenie środowiska
rm(list=ls())

# Ładowanie bibiliotek
library(caret)
library(party)
library(rpart)
library(rpart.plot)
library(e1071)
library(pso)

##############################
#### Przygotowanie danych ####
##############################

download.file('http://archive.ics.uci.edu/ml/machine-learning-databases/wine-quality/winequality-white.csv', 'wine_white.csv')
wineWhite = read.table("wine_white.csv", header = TRUE, sep=";", na.strings= "*")

# Podział parametru quality na 3 kategorie. 
wineWhite$quality <- ordered(cut(wineWhite[["quality"]], c(0,5,6,Inf)), labels = c("poor", "middle", "good"))

# Podział zbioru danych na zbiór treningowy i testowy
set.seed(4805853)
divWine <- sample(2, nrow(wineWhite), replace=TRUE, prob=c(0.65, 0.35))
trainData <- wineWhite[divWine==1,]
testData <- wineWhite[divWine==2,]

table(wineWhite)

##########################
####   Eksperymenty   ####
##########################

# Oczekiwana wartość błędu.
# cm - macierz błędów
# data - dane na których ta macierz błędów jest obliczona
errorFunction = function(cm, data)
{
  error = 0
  error = error + (cm$table[[2,1]] + cm$table[[1,2]] + cm$table[[2,3]] + cm$table[[3,2]])
  error = error + 3*(cm$table[[3,1]] + cm$table[[1,3]])
  error / nrow(data)
}

wineFormula <- quality ~ fixed.acidity + 
  volatile.acidity + 
  citric.acid + 
  residual.sugar + 
  chlorides + 
  free.sulfur.dioxide + 
  total.sulfur.dioxide + 
  density + 
  pH + 
  sulphates + 
  alcohol

control = ctree_control(maxdepth = 4, mincriterion = 0.04, minsplit = 15, minbucket = 16)
wine_ctree <- ctree(wineFormula, data=trainData, controls = control)
cm <-confusionMatrix(testData$quality, predict(wine_ctree), mode="everything")
errorFunction(cm, testData)
cm
print(wine_ctree)
plot(wine_ctree)