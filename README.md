# Tanslator
This is a translator which converts a program written in a language called UBI developed by me to corresponding C code. The UBI language only has five keywords and the corresponding C code will have only those keywords that are mapped to match the meaniing of C code.
The five keywords in UBI are: 
a) 'store' - Similar to 'int' keyword in C.
b) 'loop' - similar to 'while' keyword in C.
c) 'display' - similar to 'printf' keyword in C.
d) 'ckeckif' - similar to 'if' keyword in C.
e) 'notif' - similar to 'else' keyword in C.

While running the program after compilation the program will ask for the 'input file'. For that try using the below code, copy and paste the code in notepad and save the
file with '.ubi' extension.

//sample ubi program
----------------------------------
store a;
store b=2;
loop(a<10)
{
  b=b*a;
  a++;
}
display("%d", b);
---------------------------------

I have tried to make the translator report all possible errors, despite I might have missed accounting some errors.

You can write your own 'ubi' programs. Though the keywords may be different for the ubi language, the semantics of the language is exactly same as th C language 
So it is just writing a C program but with only the limited set of 5 keywords.
