echo "-- stl --";
func a() beg end
dump(64,"2q1", 0xFF, a);
println "default\t" + clock();
println "s\t" + clock("s");
println "ms\t" + clock("ms");
println "us\t" + clock("us");
println "ns\t" + clock("ns");
