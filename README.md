#Filtering with Gapped q-Grams

#Synopsis
Approximate string matching which compares q-grams (substrings of length q) and text is well-studied and very popular. However, as shown in paper written by Burkhardt S. and Kärkkäinen J. gapped q-grams can provide more efficient filtering. Main objective of this project is, based on mentioned paper, calculating filter parameter called optimal threshold. 

#Motivation
Project is created as assignment within the course [Bioinformatics](https://www.fer.unizg.hr/en/course/bio) on Faculty of Electrical Engineering and Computing, University of Zagreb.

## Installation
Open command window inside folder and for compile write `make` or run `compile.sh`

#Usage
Depending on parameters sent in program different data is shown. If inputs are size of shape Q (q) and Hamming distance (k) then optimal threshold is printed for span (s) up to 45 with string length (m) of 50. Alternatively, except for q and k, it is possible to define both s and m. With these 4 parameters their optimal threshold is printed.

##Run program with:
run script `run.sh` with 2 or 4 arguments as mentioned before.

##Example

Run with 4 arguments: `run.sh m k q s`
```
command: run.sh 50 5 2 2
output: 
39
```

Run with 2 arguments: `run.sh k q`
```
command: run.sh 4 2
output: 
s: 2 threshold: 41 satisfied: 1
s: 3 threshold: 40 satisfied: 1
s: 4 threshold: 39 satisfied: 1
s: 5 threshold: 38 satisfied: 1
s: 6 threshold: 37 satisfied: 1
s: 7 threshold: 36 satisfied: 1
s: 8 threshold: 35 satisfied: 1
... 
```



## Contributing
1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request 

#License

The MIT License (MIT)

Copyright (c) 2016 Antun Flaš, Manuela Kajkara, Marija Kaselj

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
