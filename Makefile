CC = g++
CXXFLAGS = -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17 -Wfatal-errors -w
app: TileMapEditor.cpp  ; $(CC) $(CXXFLAGS)  -o app TileMapEditor.cpp World.cpp olcPixelGameEngine.cpp\
		Object.cpp Button.cpp Counter.cpp

clean:  ; -rm $(objects) app error.txt log

debug:  ; $(CC) $(CXXFLAGS) -o app -g TileMapEditor.cpp World.cpp olcPixelGameEngine.cpp\
		Object.cpp Button.cpp Counter.cpp

log:  ; $(CC) $(CXXFLAGS) 2> error.txt -o app TileMapEditor.cpp World.cpp olcPixelGameEngine.cpp\
		Object.cpp Button.cpp Counter.cpp
