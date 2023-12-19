DO LABÓW INFO
pokazywanie sugestii działa jako miganie tylko w trybie edycji, w create nie!

UWAGA: 
normal mapa dziala dla pow. funkcyjnej oraz dla pow. beziera, ale w przypadku pow. 
beziera jakikolwiek punkt kontrolny musi byc rozny od ZERO (nie wiem czemu tak jest)




uruchomienie: 

wybierz z folderu bezier-surface.sln (to uruchomi projekt)

general:
w properties bezier-surface projektu wybierz:
w general c++ language standard iso c++ 20 standard
C language standard  c17 (2018)

vc++ directories
include directories: dodaj folder includes z folderu dependencies (dependencies folder znajduje sie w zewnetrzym folderze bezier-surface)
library directories: dodaj folder lib z folderu dependencies

c/c++ general: 
additional include directories: dodaj folder imgui (znajduje się w folderze gl-figure-editor)

Linker input: 
Additional dependencies dodaj: 
glfw3.lib
opengl32.lib
assimp-vc143-mtd.lib <- znajduje sie w DLLs w dependencies folderze

Klawiszologia:

zmiany control pointow do powierzchni beziera sa zapisywane dopiero po odkliknieciu checkboxa Configure Points. 
