TARGET = bin/dbview
DB = $1
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default
	#example usage
	@echo "===================================================="
	# ./$(TARGET) -f ./$1 -n 
	# ./$(TARGET) -f ./$1 -a "Timmy H.,123 Seshire Ln.,122" 
	# number=1 ; while [[ $$number -le 2 ]] ; do \
	# 	./$(TARGET) -f ./$1 -a "Timmy H.,123 Seshire Ln.,120" ; \
	# 	((number = number + 1)) ; \
	# done
	#
	# ./$(TARGET) -f ./$1 -l 
	# ./$(TARGET) -f ./$1 -t 
	# ./$(TARGET) -f ./$1 -d "Timmy H." -l
	# ./$(TARGET) -f ./$1 -t 
	# ./$(TARGET) -f ./$1 -l 
	# ./$(TARGET) -f ./$1 -u "Timmy H., 300"
	# ./$(TARGET) -f ./$1 -l 
	@echo "===================================================="
	
	# leaks --atExit --list -- ./$(TARGET) -f ./$1 -n -a "MAHIB , HEHE, 300" 
	# leaks --atExit --list -- ./$(TARGET) -f ./$1 -n -a "BHUWAN , HEHE, 250"
	# leaks --atExit --list -- ./$(TARGET) -f ./$1 -n -u "MAHIB, 250"
	# leaks --atExit --list -- ./$(TARGET) -f ./$1 -n -t -l 
	# leaks --atExit --list -- ./$(TARGET) -f ./$1 -n -d "MAHIB" -t -l 
	@echo "===================================================="

	./$(TARGET) 


default: $(TARGET)

clean:
	@echo "===================================================="
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db
	rm -rf .DS_Store
	@echo "===================================================="

$(TARGET): $(OBJ)
	gcc -o $@ $? -g -Iinclude
	@echo "===================================================="
	

obj/%.o : src/%.c
	gcc -g -Iinclude -c $< -o $@ 


