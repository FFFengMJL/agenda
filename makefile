CC := g++
INCLUDE := include
SRC := src
FLAGS := -std=c++11 -w -g

main: main.o Date.o User.o Meeting.o Storage.o Service.o UI.o
	$(CC) $(FLAGS) -I./$(INCLUDE) main.o Date.o User.o Meeting.o Storage.o Service.o UI.o -o $@

main.o: main.cpp
	$(CC) $(FLAGS) -I./$(INCLUDE) -c main.cpp -o $@

# support.o: Date.o User.o Meeting.o Storage.o
# 	$(CC) $(FLAGS) -I./$(INCLUDE) -c  Date.o User.o Meeting.o Storage.o -o $@

Storage.o: $(INCLUDE)/Storage.hpp $(SRC)/Storage.cpp
	$(CC) $(FLAGS) -I./$(INCLUDE) -c $(SRC)/Storage.cpp -o $@

Date.o: $(INCLUDE)/Date.hpp $(SRC)/Date.cpp
	$(CC) $(FLAGS) -I./$(INCLUDE) -c $(SRC)/Date.cpp -o $@

User.o: $(INCLUDE)/User.hpp $(SRC)/User.cpp
	$(CC) $(FLAGS) -I./$(INCLUDE) -c $(SRC)/User.cpp -o $@

Meeting.o: $(INCLUDE)/Meeting.hpp $(SRC)/Meeting.cpp
	$(CC) $(FLAGS) -I./$(INCLUDE) -c $(SRC)/Meeting.cpp -o $@

Service.o: $(INCLUDE)/AgendaService.hpp $(SRC)/AgendaService.cpp
	$(CC) $(FLAGS) -I./$(INCLUDE) -c $(SRC)/AgendaService.cpp -o $@

UI.o: $(INCLUDE)/AgendaUI.hpp $(SRC)/AgendaUI.cpp
	$(CC) $(FLAGS) -I./$(INCLUDE) -c $(SRC)/AgendaUI.cpp -o $@

clean:
	@rm -f *.gch
	@rm -f *.o
	@rm -f main
	@echo "cleaned"