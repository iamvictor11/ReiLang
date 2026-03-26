echo "-- break --";
a := 3;
i := 0;
until i >= a
do
    i += 1;
    print "i ";
    println i;
    j := 0;
    while j < a
    do
        j += 1;
        print "j ";
        println j;
        k := 0;
        until false
        do
            k += 1;
            print "k ";
            println k;
            if (k == 3)
                break 2;
            u := 0;
            loop u < a
            do
                u += 1;
                print "u ";
                println u;
                if (k == 2)
                    continue 2;
            end
            println "hajimi";
        end
    end
end
