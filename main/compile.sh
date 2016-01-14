if [ -f main.out ]; then
   rm main.out
fi
g++ main.cpp -std=c++0x -o main.out
