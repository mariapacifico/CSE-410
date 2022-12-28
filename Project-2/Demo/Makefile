all: sample no-sem sample

sample: sample.c
	gcc sample.c -lpthread -o sample

no-sem: no-sem.c
	gcc no-sem.c -lpthread -o no-sem

clean:
	rm sample no-sem
