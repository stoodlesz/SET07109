**NAME OF PROGRAM**

jdoc

**TOOL CHAIN**

gcc
________________________________________________________________________________________________________________________

**FILES**

*Files available:*
- jdoc.c
- makefile

*Files required:*
- Rectangle.java  
- Student.java
________________________________________________________________________________________________________________________

**BUILDING**

*Build from command line inputs:* 
`make jdoc`

*Cleaning folder from command line:*
`make clean`

*Options:*
`jdoc -i [input].java -o [output].txt` -> upon entering the file you want to input and output, this will output the txt file required and compile the .exe from the jdoc associated with that java file.

________________________________________________________________________________________________________________________

**DESCRIPTION**

*An App that:*
- Counts & prints total number of comments in a `.java` file, in console.
- Counts & prints the total number of lines in a `.java` file.
- Counts & prints the total number of non blank lines in a `.java file`.
- Outputs a `.txt` file associated with the `.java` file, with the comments from the `.java` file.
- Prints the javadoc comment details in console output; `@author`, `@params` and `@methods`. As well as `class name`.