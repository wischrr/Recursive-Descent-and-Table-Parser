# Recursive-Descent-and-Table-Parser

Elias Haddad 
Mason Wischhover 

How to build:
gcc -std=c99 -Wall -Werror ./bits.c -o bits

How to run:
./bits



LL(1) grammar of bitstring expressions:

⟨E⟩ → ⟨T⟩ ⟨ET⟩
⟨ET⟩ → | ⟨E⟩ | ϵ
⟨T⟩ → ⟨F⟩ ⟨TT⟩
⟨TT⟩ → & ⟨T⟩ | ϵ
⟨F⟩ → ~ ⟨F⟩ | ( ⟨E⟩ ) | ⟨S⟩
⟨S⟩ → ⟨B⟩ ⟨ST⟩
⟨ST⟩ → ⟨S⟩ | ϵ
⟨B⟩ → 0 | 1

Sample input of valid expressions (for both parts):

1
101010
~010
~~100
10&01
1|00&111
(101|001)&(11|00)
~(101|010)



Part 1: Recursive-descent parser

This part of the project is built from the syntactic categories of the bitstring LL(1) grammar. Each syntactic category corresponds to a specific function that will attempt to build a parsing tree based on user input, and as a result, each of these functions returns a tree data structure. To build these trees, based on the syntactic category function, various helper functions are called (ie makenode0, 1, …) to create different types of trees to be added to the main parsing tree being built. The first method to be called therefore is the first syntactic category (E() in this case). If a parsing tree is successfully created, the program will take the resulting parse tree from this method and output it to the user. If the parsing tree creation fails (“FAILED” in the program is synonymous with NULL), no tree is outputted and the program will notify the user that the input is not part of the bitstring grammar. Finally, there is a lookahead function to check if the current input pointer matches some char x, and then a match function which will call this lookahead function to determine if there is a match between the current pointer and char x, the pointer is incremented to the next user input. The user is also given the option to quit the program at any point, in which case part 2 will be executed.

Part 2: Table-driven parser

For part two of the project, a matrix (2D array) is initialized which represents the parsing table of the LL(1) grammar of simple bitstring expressions. The user is prompted to input a bitstring expression which will be parsed by the table in the function tableParsing(), which contains the table and the bitstring input as parameters. In addition to this, there’s a stack which initially contains the starting symbol. Once the first character in the bitstring input is read, that character and whatever symbol gets popped out of the stack are used as coordinates in the matrix. If the coordinates lead to a ‘filled’ space (one that doesn’t contain a 0), then the (14 unique) grammars corresponding to that number is pushed into the stack. This procedure is printed onto the screen, and will continue to unless the input characters read are either not contained in the table or don’t match a production. 
