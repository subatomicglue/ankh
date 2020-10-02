#!/usr/bin/perl

sub round
{
    my($number) = shift;
    return int($number + .5);
}
sub floor
{
    my($number) = shift;
    return int($number);
}

if (open( FILE, "< assets.dat" ))
{
	my @lines = <FILE>;
	close( FILE );

	if (open( FILE, ">data.h") && open( CPPFILE, ">data.cpp"))
	{
		print FILE "#ifndef GAME_ASSETS\n";
		print FILE "#define GAME_ASSETS\n";

		print FILE "#include <assert.h>\n";
		print FILE "#include \"Vec2.h\"\n";
		print FILE "#include \"MapAsset.h\"\n";
		print FILE "//#include \"gba/Sprite.h\"\n";
		print FILE "#include \"Actor.h\"\n";
		print FILE "#include \"UIScreenAsset.h\"\n";
		#print FILE "#include \"AnkhActor.h\"\n";
		print FILE "class AnkhActor;\n";
		print FILE "class PanPad;\n";

		print FILE "/////////////////////////////////////////////////////////\n";
		print FILE "// readonly game data\n";
		print FILE "///////////////////////////////////////////////////////////\n";

		print CPPFILE "#include \"AnkhActor.h\"\n";
		print CPPFILE "#include \"Behaviours.h\"\n";
		print CPPFILE "#include \"PanPad.h\"\n";
		print CPPFILE "#include \"data.h\"\n\n";

      my $bmpdatabase = "const gba::BitmapAsset* const gAllBitmaps[] = { ";
      my $bmpdatabasecount = 0;

      my $actordatabase = "AnkhActor* gAllActors[] = { ";
      my $actordatabasecount = 0;


      my $roomoffsetx = 0, $roomoffsety = 0;
	   my $tilesizex = 16, $tilesizey = 16;
		my $roomsizex = $tilesizex * 12, $roomsizey = $tilesizey * 12;
		foreach $line (@lines)
		{
			#print $line;
			if ($line =~ m/^\s*palette\s*(\S*)\s*;/ig)
			{
				print FILE "extern \"C\" { extern const unsigned short $1\[\]; };\n";
			}
		
			if ($line =~ m/^\s*bitmap\s*(\S*)\s*=\s*{\s*([()0-9*+-\/\s]*)\s*,\s*([()0-9*+-\/\s]*)\s*,\s*([()0-9*+-\/\s]*)\s*,\s*([()0-9*+-\/\s]*)\s*,\s*(\S*)\s*,\s*(\S*)\s*}\s*;/ig)
			{
				print FILE "extern \"C\" { extern const unsigned char $6\[\]; };\n";
				print FILE "extern const gba::BitmapAsset $1;\n\n";
            print CPPFILE "const gba::BitmapAsset $1 = { $2, $3, $4, $5, $6, $7 };\n";
            $bmpdatabase .= "&" . $1 . ", ";
            $bmpdatabasecount++;
			}
			
			if ($line =~ m/^\s*sprite\s*(\S*)\s*=\s*{\s*([()0-9*+-\/\s]*)\s*,\s*([()0-9*+-\/\s]*)\s*,\s*(\S*)\s*}\s*;/ig)
			{
				print FILE "extern const gba::SpriteAsset $1Sprite;\n";
				print CPPFILE "const gba::SpriteAsset $1Sprite = { gba::Vec<s32>( $2, $3 ), $4 };\n";
			}

			if ($line =~ m/^\s*map\s*(\S*)\s*=\s*{\s*([()0-9*+-\/\s]*)\s*,\s*([()0-9*+-\/\s]*)\s*,\s*(\S*)\s*}\s*;/ig)
			{
				print FILE "extern \"C\" { extern const unsigned short $4\[\]; };\n";
				print FILE "const gba::MapAsset $1 = { $2, $3, $4 };\n\n";
			}

			if ($line =~ m/^\s*uiscreen\s*(\S*)\s*=\s*{\s*(\S*)\s*,\s*(\S*)\s*}\s*;/ig)
			{
				print FILE "extern const UIScreenAsset $1;\n";
				print CPPFILE "const UIScreenAsset $1 = { $2, &$3 };\n";
			}

			
			if ($line =~ m/^\s*\/\/\s*room\s*([0-9]*)\s*,\s*([0-9]*)/ig)
			{
				$roomoffsetx = scalar( $1 );
				$roomoffsety = scalar( $2 );
			}

			if ($line =~ m/^\s*actor\s*(\S+)\s*=\s*{\s*([A-Za-z0-9]+)(\((\s*[^\)]*\s*)\))?\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,?\s*([A-Za-z_ |]*)\s*}\s*;/ig)
			{
				#print "bok - - - [" . $1 . "] [" . $2 . "] [" . $3 . "] [" . $4 . "] [" . $5 . "] [" . $6 . "]\n";
				my $actorname = $1;
				my $behaviortype = $2;
				my $behaviorarg = $4; # optional  $3 has the parens around it, so ignore it...
				my $spritename = $5;
				my $roomposx = scalar( $6 );
				my $roomposy = scalar( $7 );
				my $karmapoints = $8;
				my $shootpoints = $9;
				my $flags = $10;
				if ($flags eq "")
				{ $flags = "ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP"; }
				else
				{ $flags =~ s/([A-Z_]+)/ACTOR_$1/ig; }

				my $posxtext = $roomposx . " * " . $tilesizex . " + " . $roomoffsetx . " * $roomsizex - $tilesizex";
				my $posytext = $roomposy . " * " . $tilesizey . " + " . $roomoffsety . " * $roomsizey - $tilesizey";
				my $posx = $roomposx * $tilesizex + $roomoffsetx * $roomsizex - $tilesizex;
				my $posy = $roomposy * $tilesizey + $roomoffsety * $roomsizey - $tilesizey;
				if ($roomposx =~ s/([0-9]+)px/$1/)
				{
					$posxtext = $roomposx . "px";
					$posx = $roomposx;
				}
				if ($roomposy =~ s/([0-9]+)px/$1/)
				{
					$posytext = $roomposy . "px";
					$posy = $roomposy;
				}

				$posx =~ s/\..*$//;
				$posy =~ s/\..*$//;

				print FILE "extern const ActorInit ${actorname}Init; // ($posxtext), ($posytext)\n";
				print CPPFILE "const ActorInit ${actorname}Init = { \"${actorname}\", ActorInit::$behaviortype, &${spritename}Sprite, $posx, $posy, $roomoffsetx, $roomoffsety, $karmapoints, $shootpoints, $flags }; // ($posxtext), ($posytext)\n";
				print FILE "extern AnkhActor ${actorname};\n";
				my $behavior = $behaviortype;
				$behavior =~ s/^(.)/\U$1/;

				if ($behaviorarg eq "")
				{
					print CPPFILE "${behavior}Method ${actorname}BehaviourMethod;\n";
				}
				else
				{
					print CPPFILE "${behavior}Method ${actorname}BehaviourMethod($behaviorarg);\n";
				}
				print CPPFILE "AnkhActor $actorname( ${actorname}Init, &${actorname}BehaviourMethod );\n";
	        
				$actordatabase .= "&" . $actorname . ", ";
				$actordatabasecount++;
			}
			
		}

      print FILE "\n\n$bmpdatabase };\n";
      print FILE "const unsigned int gBitmapCount = $bmpdatabasecount;\n";

      print FILE "\n\nextern AnkhActor* gAllActors[" . $actordatabasecount . "];\n";
      $actordatabase =~ s/\[\]/[$actordatabasecount]/;
      print CPPFILE "\n\n$actordatabase };\n";
      print FILE "const unsigned int gActorCount = $actordatabasecount;\n";


      print FILE "#endif\n\n";	
	}
}



#print "\n\nhit a key...\n";
#my $pause = <STDIN>;
