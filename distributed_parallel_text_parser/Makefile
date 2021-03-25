build:
	mpic++ main.cpp master.cpp worker.cpp helpers.cpp genre_functions.cpp -o main

run:
	mpirun -oversubscribe -np 5 ./main

clean:
	rm main
