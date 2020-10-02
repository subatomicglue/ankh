#!/usr/bin/ruby

$includes = []
$foundargs = true
while $foundargs
   $foundargs = false
   if (ARGV[0] == "-I" || ARGV[0] == "-i") && 2 <= ARGV.size
      ARGV.delete_at( 0 )
      $includes.push( ARGV[0] )
      ARGV.delete_at( 0 )
      $foundargs = true
   end
end

$filesvisited = {}

def justExt( file )
   return file.gsub( /.*\.([^\.]+)$/, "\\1" )
end

def findFile( file, lookInCurrentDirFirst )
   if lookInCurrentDirFirst && File.exists?( file )
      return file
   end
   $includes.each {|incPath|
      #print "checking include path: " + incPath + "\n"
      if File.exists?( File.join( incPath, file ) )
         #print " - found " + File.join( incPath, file ) + "\n"
         return File.join( incPath, file )
      end
   }
   return ""
end

$headers = []
def processResults( result, lookInCurrentDirFirst )
   result.each {|match|
      filepath = findFile( match[0], lookInCurrentDirFirst )
      if filepath != ""
         $headers.push( filepath )
         #print filepath + " "
         readFile( filepath )
      end
   }
end

def readFile( file )
   # TODO: edit this for other filetypes... right now we know how to scan C/C++
   if justExt( file ) =~ /(h|hpp|h\+\+|hxx|cpp|c|cxx|c\+\+)/i && $filesvisited[file] == nil
      $filesvisited[file] = true
      #print "opening " + file + "\n"
      text = IO.readlines( file ).join(" ")
      results = text.scan( /#\s*include\s*["']([^"']+)["']/i )
      processResults( results, true )
      results = text.scan( /#\s*include\s*[<]([^>]+)[>]/i )
      processResults( results, false )
   end
end


readFile( ARGV[0] )
$headers.each {|header|
   print header + " ";
}
print "\n"
