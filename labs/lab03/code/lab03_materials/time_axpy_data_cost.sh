echo "------- axpy with array vs repeated scalar multiply-add --------"
echo "------- times are average over 100 trials --------"
echo "n, without array, with array"
./bin/axpy_data_cost 100
./bin/axpy_data_cost 400
./bin/axpy_data_cost 1600
./bin/axpy_data_cost 3200
./bin/axpy_data_cost 12800
./bin/axpy_data_cost 51200
./bin/axpy_data_cost 204800
./bin/axpy_data_cost 819200
./bin/axpy_data_cost 3276800
./bin/axpy_data_cost 13107200
./bin/axpy_data_cost 52428800