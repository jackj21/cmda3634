echo "------- axpy regular access --------"
echo "------- times are average over 100 trials --------"
echo "n, run time"
./bin/axpy_normal 100 
./bin/axpy_normal 200
./bin/axpy_normal 400 
./bin/axpy_normal 800 
./bin/axpy_normal 1600 
./bin/axpy_normal 3200
./bin/axpy_normal 6400
./bin/axpy_normal 12800
./bin/axpy_normal 25600 
./bin/axpy_normal 51200
./bin/axpy_normal 102400 
./bin/axpy_normal 204800
./bin/axpy_normal 409600 
./bin/axpy_normal 819200
./bin/axpy_normal 1638400 
./bin/axpy_normal 3276800
./bin/axpy_normal 6553600
./bin/axpy_normal 13107200
./bin/axpy_normal 26214400
./bin/axpy_normal 52428800