echo "-- loop --";
b := 0;
until b>=5
do
    b += 1;
    println b;
    c := 1;
    if b < 3
    then
        c := "c";
        println c;
        continue;
    end
    else
    then
        d:="d";
        println d;
        break;
    end
end
