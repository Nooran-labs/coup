CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -fPIC
QT_CFLAGS = -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB \
  -I/usr/include/x86_64-linux-gnu/qt5 \
  -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets \
  -I/usr/include/x86_64-linux-gnu/qt5/QtGui \
  -I/usr/include/x86_64-linux-gnu/qt5/QtCore

QT_LIBS = -lQt5Widgets -lQt5Gui -lQt5Core

SRC = Game.cpp Player.cpp Governor.cpp Spy.cpp Baron.cpp \
      Judge.cpp General.cpp Merchant.cpp \
      MainWindow.cpp coup_gui_main.cpp

OBJ = $(SRC:.cpp=.o)
MOC_SRC = MainWindow.hpp
MOC_GEN = moc_MainWindow.cpp
MOC_OBJ = moc_MainWindow.o

TARGET = coup_gui

all: $(TARGET)

$(TARGET): $(OBJ) $(MOC_OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(MOC_OBJ) $(QT_LIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(QT_CFLAGS) -c $< -o $@

$(MOC_GEN): $(MOC_SRC)
	moc $(MOC_SRC) -o $(MOC_GEN)

$(MOC_OBJ): $(MOC_GEN)
	$(CXX) $(CXXFLAGS) $(QT_CFLAGS) -c $(MOC_GEN) -o $(MOC_OBJ)

run-gui: $(TARGET)
	./$(TARGET)
	
# Main (terminal version)
main: main.cpp Game.o Player.o Governor.o Spy.o Baron.o Judge.o General.o Merchant.o
	$(CXX) $(CXXFLAGS) main.cpp Game.o Player.o Governor.o Spy.o Baron.o Judge.o General.o Merchant.o -o main

test: Game.o Player.o Governor.o Spy.o Baron.o Judge.o General.o Merchant.o test_coup.cpp
	$(CXX) $(CXXFLAGS) $^ -o test

# Valgrind test
valgrind: main
	valgrind --leak-check=full --track-origins=yes ./main


clean:
	rm -f *.o coup_gui main test moc_*.cpp


