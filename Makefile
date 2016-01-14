all:
	g++ main.cpp -std=c++0x -o main.out

clean:
	@ rm main.out

run:
	@ if [ -f main.out ]; then ./main.out; fi

