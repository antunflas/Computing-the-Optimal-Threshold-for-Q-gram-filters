if [ main-recursive.cpp -nt main-recursive.out ]; then
	./compile-recursive.sh;
fi

if [ -f main-recursive.out ]; then
	./main-recursive.out "$@";
fi

