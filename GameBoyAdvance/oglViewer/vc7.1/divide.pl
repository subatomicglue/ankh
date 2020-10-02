#!/usr/bin/perl
open( FILE, "< actors.out.txt" );
@array = <FILE>;
close( FILE );
open( FILE, "> actors.out.txt" );
foreach $item (@array)
{
   if ($item =~ m/(\S*)\s*(\S*),\s*(\S*)\s*\n/)
   {
      my $num1 = $1;
      my $num2 = scalar( $2 ) / 2.0;
      my $num3 = scalar( $3 ) / 2.0;
      print FILE "$num1 $num2, $num3\n";
   }
   
}
close( FILE );
