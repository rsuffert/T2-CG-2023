# Makefile para Windows

PROG = Paredao.exe
FONTES = ListaDeCoresRGB.cpp Ponto.cpp ProgramaBasico3D.cpp Temporizador.cpp LeitorObjeto3D.cpp SOIL/*.c
OBJETOS = $(FONTES:.cpp=.o)
CPPFLAGS = -O3 -g -Iinclude/GL # -Wall -g  # Todas as warnings, infos de debug
# Se estiver usando mingw64, troque lib por lib\x64 abaixo
LDFLAGS = -Llib -lfreeglut -lopengl32 -lglu32 -lm
CC = g++

$(PROG): $(OBJETOS)
	$(CC) $(CPPFLAGS) $(OBJETOS) -o $@ $(LDFLAGS)

clean:
	-@ del ListaDeCoresRGB.o Ponto.o ProgramaBasico3D.o Temporizador.o LeitorObjeto3D.o Paredao.exe