main: main.c Lab3IO.c
	gcc main.c Lab3IO.c -o main -lm

datagen: datagen.c Lab3IO.c
	gcc datagen.c Lab3IO.c -o datagen

serialtester: serialtester.c Lab3IO.c
	gcc serialtester.c Lab3IO.c -o serialtester -lm

all: main.c Lab3IO.c serialtester.c datagen.c
	gcc main.c Lab3IO.c -o main -lm
	gcc datagen.c Lab3IO.c -o datagen
	gcc serialtester.c Lab3IO.c -o serialtester -lm

clean:
	rm main -f
	rm datagen -f
	rm serialtester -f
	rm data_input -f
	rm data_output -f