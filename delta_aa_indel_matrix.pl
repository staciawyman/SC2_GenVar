#!/usr/bin/perl

# Read in the list of all mutations in Delta PAUI's used in analysis create aa+indel matrix for indels and nonsynonymous mutaitons.
#	$freq should be >= .02 
#	also prune out mutations that exist in <10 individuals since that wouldn't have enough power to say anything about it

$nonsyn = 0;
$ins = 0;
$del = 0;

open(P,"delta_mutations_list.txt") || die "couldn't open delta_mutations_list.csv";
$count = 0;
$found = 0;
while (<P>) {
    chomp;
    $file = $_;
    open(F,$file) || die "Couldn't open $file";
    $count++;
    while (<F>) { 
	if (/^REG/) { next; } 
	if (/FALSE/) { next; } 
	chomp;
	my ($genome,$pos,$ref,$alt,$a,$b,$c,$x,$y,$z,$freq,$i,$j,$k,$gene,$l,$aa1,$m,$aa2,$indel_mut) = split(/\t/);
	
	$mutation = $indel_mut;
	if ($gene eq "NA") { next;} # skip non-genic mutations
	if ($indel_mut ne "NA") { 
		$mutation = $indel_mut; 
		$mutation =~ s/;/:/; 
	} else {
		$mutation = $gene.":".$ref.$pos.$alt;
	}
	if ($freq >= .02 ) {
	    #if ($alt =~ /[+-]/) { # to include just aa mutations, uncomment this
	      $found = 1;
	      $matrix{$mutation}{$genome} = $freq;
	      $genomes{$genome} += 1;
    	      $mutations{$mutation} += 1;
	    #}
	}
    }
    close(F);
    $found = 0;
}
close(P);

# print out header with all the genome PAUI's
foreach $gen (sort keys %genomes) {
	print "\t$gen";
}
print "\n";

$ins = 0;
$del = 0;
# print out 2D array--for each mutation, print out frequency for each genome
foreach $mut (keys %mutations) {
	$str =  "$mut";
	$count = 0;
	$freqs = "";
	foreach $gen (sort keys %genomes) {
		if ($matrix{$mut}{$gen}) {
		        $str .= "\t$matrix{$mut}{$gen}";
			$freqs .= "\n$matrix{$mut}{$gen}";
		        $count++;
		}else {
		    $str .= "\t0";
		}
	}
	if ($count > 9) {
	    print "$str\n";
	}
}
