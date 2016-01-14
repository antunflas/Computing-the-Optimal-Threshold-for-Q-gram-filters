if [ -f main.out ]; then
   rm main-recursive.out
fi
g++ main-recursive.cpp -std=c++0x -o main-recursive.out
