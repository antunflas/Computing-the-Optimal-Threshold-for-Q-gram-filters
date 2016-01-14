if [ main.cpp -nt main.out ]; then
	./compile.sh;
fi

if [ -f main.out ]; then
	./main.out "$@";
fi

