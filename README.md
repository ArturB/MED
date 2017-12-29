# MED

Projekt MED 2017Z. 

# Kompilacja:

Projekt jest budowany i kompilowany z użyciem CMake. Pozwala to na pełną przenośność i możliwość zmiany kompilatora z MSVC na gcc w dowolnym momencie. Może to mieć znaczenie dla tak mocno wymaganej przez prof. Kryszkiewicz wydajności, bo moje doświadczenia są takie, że różnice w szybkości wykonania pomiędzy tymi dwoma kompilatorami mogą być spore. 

Budowanie projektu: aby zbudować projekt, należy mieć zainstalowany CMake https://cmake.org/download/ . CMake generuje dwa rodzaje solucji: dla Visual Studio i dla Unix Makefile. 

## Visual Studio:

W folderze ./build znajduje się solucja Visual Studio, plik MED.sln, która zawiera trzy projekty: ALL_BUILD, MED oraz ZERO_CHECK. Właściwy kod źródłowy projektu znajduje się w projekcie MED. Póki co jest tam tylko jeden plik main.cpp, demonstracyjnie obliczający pierwiastek kwadratowy z zadanej liczby. 

## Unix Makefile:

W folderze ./build jest też plik Makefile. 

# Kontakt

Artur M. Brodzki, Mateusz Orzoł. 


