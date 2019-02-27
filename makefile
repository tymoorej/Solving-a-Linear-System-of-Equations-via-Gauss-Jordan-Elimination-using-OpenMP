main: main.c Lab3IO.c
	gcc main.c Lab3IO.c -o main -lm -fopenmp -O4 -w

serial: serial.c Lab3IO.c
	gcc serial.c Lab3IO.c -o serial -lm -fopenmp


datagen: datagen.c Lab3IO.c
	gcc datagen.c Lab3IO.c -o datagen

serialtester: serialtester.c Lab3IO.c
	gcc serialtester.c Lab3IO.c -o serialtester -lm

all: main.c Lab3IO.c serialtester.c datagen.c
	gcc main.c Lab3IO.c -o main -lm -fopenmp -O4 -w
	gcc datagen.c Lab3IO.c -o datagen 
	gcc serialtester.c Lab3IO.c -o serialtester -lm 
	gcc serial.c Lab3IO.c -o serial -lm -fopenmp 


clean:
	rm main -f
	rm datagen -f
	rm serialtester -f
	rm data_input -f
	rm data_output -f
	rm serial -f
