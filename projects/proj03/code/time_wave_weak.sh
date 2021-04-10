#!/bin/bash

echo "weak scalability"
echo "=================="
echo "n nt t_total n_threads" > wave_weak.csv

alpha=1
mx=13
my=7
nt=250
NTRIALS=3

for i in {1..$NTRIALS}
do
	./wave_timing 2501 $mx $my $alpha $nt 1 >> wave_weak.csv

	./wave_timing 3537  $mx $my $alpha $nt 2 >> wave_weak.csv

	./wave_timing 5002 $mx $my $alpha $nt 4 >> wave_weak.csv

	./wave_timing 7074 $mx $my $alpha $nt 8 >> wave_weak.csv

	./wave_timing 10004 $mx $my $alpha $nt 16 >> wave_weak.csv

	./wave_timing 14148 $mx $my $alpha $nt 32 >> wave_weak.csv

	./wave_timing 20008 $mx $my $alpha $nt 64 >> wave_weak.csv
done

echo ""
echo ""

