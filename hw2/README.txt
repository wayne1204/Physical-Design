2008 Physical Design


1. Author             : R98000001 NAME
2. Problem            : Global Routing
3. Programming        : C++ 
4. Platform           : Linux, Sun OS/Solaris
5. Compiler           : GNU g++
6. Compression way    : tar zcvf r98000001_pa2.tar.gz pa2 
7. De-compression way : tar zxvf r98000001_pa2.tar.gz

8. Files Description:
	pa2/                       : program dir
	pa2/gr                     : executable binary
	pa2/*.cpp                  : source code
	pa2/*.h                    : source code
	pa2/lib/*.cpp              : source code (library)
	pa2/lib/*.h                : source code (library)
	pa2/*.out                  : output file
	pa2/a1~a5, n1~n3, example  : script files
	pa2/input/*.gr             : input files
	pa2/Makefile               : Makefile
	pa2/README.txt             : this file
	pa2/eval2008.pl            : verification file
	pa2/example_gold.out       : golden output file for input/example.gr
	

9. How to compile        	
	    1) Type "make" to compile.
	    2) The "gr" executable binary would be generated.
	    3) If you want to re-compile, type "make clean" followed by "make".
	    
	

10. How to execute
            syntax:
            ./gr [input_file] [output_file]

            examples:
            ./gr input/adaptec1.capo70.2d.35.50.90.gr a1.out
	    ./gr input/example.gr example.out

	    or you can directly use the given scripts 
	    "a1~a5", "n1~n3", "example" to run the given inputs.
		        

10. How to verify
            syntax:
	    ./eval.pl [input_file] [output_file]

	    example:
	    ./eval.pl input/example.gr example_gold.out



