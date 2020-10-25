# main: arraylist.cc pmochila.cpp 
# 	g++ -O3 arraylist.cc pmochila.cpp -o main

main: arraylist.cc grasp.cpp 
	g++ -O3 arraylist.cc grasp.cpp -o main

clean :
	rm main

