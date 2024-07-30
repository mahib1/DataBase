TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default
	#example usage
	@echo "===================================================="
	./$(TARGET) -f ./mynewdb.db -n 
	./$(TARGET) -f ./mynewdb.db -a "Timmy H.,123 Seshire Ln.,122" 
	number=1 ; while [[ $$number -le 2 ]] ; do \
		./$(TARGET) -f ./mynewdb.db -a "Timmy H.,123 Seshire Ln.,120" ; \
		((number = number + 1)) ; \
	done

	./$(TARGET) -f ./mynewdb.db -l 
	./$(TARGET) -f ./mynewdb.db -t 
	./$(TARGET) -f ./mynewdb.db -d "Timmy H." -l
	./$(TARGET) -f ./mynewdb.db -t 
	./$(TARGET) -f ./mynewdb.db -l 
	./$(TARGET) -f ./mynewdb.db -u "Timmy H., 300"
	./$(TARGET) -f ./mynewdb.db -l 


default: $(TARGET)

clean:
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


