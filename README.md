Synopsis

Approximate string matching which compares q-grams (substrings of length q) and text is well-studied and very popular. However, as shown in paper written by Burkhardt S. and Kärkkäinen J. gapped q-grams can provide more efficient filtering. Main objective of this project is, based on mentioned paper, calculating filter parameter called optimal threshold. 

Motivation

Project is created as assignment within the course Bioinformatics on Faculty of Electrical Engineering and Computing, University of Zagreb. (https://www.fer.unizg.hr/en/course/bio)

Installation

Depending on parameters sent in program different data is shown. If inputs are size of shape Q (q) and Hamming distance (k) then optimal threshold is printed for span (s) up to 45 with string length (m) of 50. Alternatively, except for q and k, it is possible to define both s and m. With these 4 parameters their optimal threshold is printed.
Run program:
_____.exe m k q s   
 Or
_____.exe k q

Example
For: 
_____.exe m k q s   
output is: xxxxxxxxxxxxxxxxxxxxxxxx
For:
_____.exe k q
output is: xxxxxxxxxxxxxxxxxxxxxxxx

License

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
