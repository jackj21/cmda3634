echo "------- axpy random access --------"
echo "------- times are average over 100 trials --------"
echo "n, run time"
./bin/axpy_random 100 
./bin/axpy_random 200
./bin/axpy_random 400 
./bin/axpy_random 800 
./bin/axpy_random 1600 
./bin/axpy_random 3200
./bin/axpy_random 6400
./bin/axpy_random 12800
./bin/axpy_random 25600 
./bin/axpy_random 51200
./bin/axpy_random 102400 
./bin/axpy_random 204800
./bin/axpy_random 409600 
./bin/axpy_random 819200
./bin/axpy_random 1638400 
./bin/axpy_random 3276800
./bin/axpy_random 6553600
./bin/axpy_random 13107200
./bin/axpy_random 26214400
./bin/axpy_random 52428800