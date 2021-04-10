#!/bin/bash

echo "strong scalability"
echo "=================="
echo "n nt t_total n_threads" > wave_strong.csv

n=2501
alpha=1
mx=13
my=7
nt=8838
NTRIALS=3

for i in {1..$NTRIALS}
do
	./wave_timing $n $mx $my $alpha $nt 1 >> wave_strong.csv
	./wave_timing $n $mx $my $alpha $nt 2 >> wave_strong.csv
	./wave_timing $n $mx $my $alpha $nt 4 >> wave_strong.csv
	./wave_timing $n $mx $my $alpha $nt 8 >> wave_strong.csv
	./wave_timing $n $mx $my $alpha $nt 16 >> wave_strong.csv
	./wave_timing $n $mx $my $alpha $nt 32 >> wave_strong.csv
	./wave_timing $n $mx $my $alpha $nt 64 >> wave_strong.csv
done

echo ""
echo ""

