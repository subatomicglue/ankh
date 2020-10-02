#!/usr/bin/perl

if (open( INFILE, "< oglViewer/vc7.1/actors.out.txt" ) &&
    open( OUTFILE, "< assets.dat" ))
{
   my @inlines = <INFILE>;
   my @outlines = <OUTFILE>;
   close( INFILE );
   close( OUTFILE );

   my $num1 = 0, $num2 = 0;

      foreach $inline (@inlines)
      {
         #print $inline;

         if ($inline =~ m/(\S*)\s*(\S*)\s*(\S*)/)
         {
            my $name = $1;
            my $one = $2;
            my $two = $3;
            my $didnotfind = 1;
            for (my $iter = 0; $iter < @outlines; $iter++)
            {
               if (@outlines[$iter] =~ m/^\s*actor\s*$name\s*/ig)
               {
                  if (@outlines[$iter] =~
                        s/^(\s*actor\s*\S*\s*=\s*{\s*\S*\s*,\s*\S*\s*,\s*)([()0-9*+-\/\s]*\s*,\s*[()0-9*+-\/\s]*)(\s*}\s*;)/$1$one $two $3/ig)
                  {
                     print @outlines[$iter];
                     $num1++;
                     $didnotfind = 0;
                     last;
                  }
               }
            }
            if ($didnotfind)
            {
               print "[error] didn't find the entry\n";
            }
         }
      }

      #print "====================\n";
      foreach $outline (@outlines)
      {
         if ($outline =~ m/^\s*actor\s*(\S*)\s*=\s*{\s*(\S*)\s*,\s*(\S*)\s*,\s*([()0-9*+-\/\s]*)\s*,\s*([()0-9*+-\/\s]*)\s*}\s*;/ig)
         {
            #print $outline;
            $num2++;
         }
      }
      print $num1 . "  " . $num2 . "\n";

   if (open( OUTFILE, "> assets.dat" ))
   {
      foreach $outline (@outlines)
      {
         print OUTFILE $outline;
      }
      close( OUTFILE );
   }
}
