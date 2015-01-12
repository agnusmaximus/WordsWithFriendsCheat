all:
	g++-4.9 -O3 -ftree-vectorizer-verbose=9 -msse2 -fopenmp -D_GLIBCXX_PARALLEL=1 -std=c++11 generate_moves.cpp -Iboost_1_57_0 -L/usr/local/boost_1_57_0/stage/lib/ -lboost_serialization-mt

native:
	g++ -O3 -march=native -mavx2 -std=c++11 generate_moves.cpp -I/usr/local/Cellar/boost/1.57.0/include -L/usr/local/Cellar/boost/1.57.0/lib -lboost_serialization
