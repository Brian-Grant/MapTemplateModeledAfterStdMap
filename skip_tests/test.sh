#!/bin/bash


mydir=$(pwd)

output_dir=$mydir/out


begin=0
end=10000
#end=100

#low=0
#high=1000

low=$1
high=$2
print=0
{
for ((j=$begin;j<=$end;j++));
do	
	echo $j	
	#./skip_test $low $high $j &> testres/out$j.txt
	./skip_test $low $high $print $j > $output_dir/silencer.txt
done
} &> out/master_out.txt


rm $output_dir/silencer.txt

#grep -a -B 1 'Seg' $output_dir/master_out.txt >$output_dir/tmp.txt
#grep -a -x -E '[[:digit:]]+' $output_dir/tmp.txt >$output_dir/seg_$low-$high.txt
#rm $output_dir/master_out.txt $output_dir/tmp.txt
