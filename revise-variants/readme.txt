Instructions for including a new variant :


(i) First, create a .txt file for the list of characteristic

mutations in the new variant (see alpha.txt, beta.txt, etc.

for the correct file format .. the lines "Nonsynonymous" and 

"Synonymous" are required). 


(ii) Let the name of the file in (i) be e.g. epsilon.txt. In 

the C++ file get-input-list.cpp, there is the following code 

block :



    l_1.push_back("alpha");
    l_1.push_back("beta");
    l_1.push_back("delta");
    l_1.push_back("gamma");
    l_1.push_back("mu");
    l_1.push_back("BA.1");
    l_1.push_back("BA.2");



Add the line 


    l_1.push_back("epsilon"); 


to the end of this code block.


(iii) Generate the combined lists for indels, non-synonymous, 

and synonymous mutations by de-commenting-out one of the 

three #define's above the main() block of code. For example, 

to generate the combined list of non-synonymous mutations, 

the block of #define's should look like this :



//#define read_indels
//#define read_synonymous
#define read_non_synonymous 


 
Then, compile the code as



     g++ -O2 get-input-list.cpp



and run the code as 



    ./a.out > non-synonymous.txt



Similarly for indels and synonymous mutations, sending the

results to files indels.txt, and synonymous.txt resp. (note 

that the files must have the names given above for the next

step, step (iv), to work).



(iv) Create C++ the class structures non-synonymous.cpp, 

indels.cpp, and synonymous.cpp. For example, to create 

non-synonymous.cpp, compile



    g++ -O2 get-non-synonymous-class.cpp



then run



    ./a.out > non-synonymous.cpp



Do this for get-indels-class.cpp and get-synonymous-class.cpp

as well, sending the output to indels.cpp, and synonymous.cpp

respectively (revise-variants.cpp includes these files, so

again, the file names must be as given above).


(v) Finally, re-compile revise-variants.cpp as 



    g++ -O2 revise-variants.cpp -o revise-variants.exe



then copy executable revise-variants.exe to pipeline_output/ 

and follow the instructions there in readme-revise.txt




