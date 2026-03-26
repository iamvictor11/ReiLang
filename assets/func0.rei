echo "-- func 0 --";
a := 0;
func f1()
beg
    echo "FN1";
    echo a;
    a := 1111111111;
    echo a;
    return;
end
func nf() beg end
func hf(i,j,k)
beg
    echo i;
    echo j;
    echo k;
    return: i + j + k;
end
echo f1();
echo nf();
echo nf() + "haha";
hf();
hf(1, 2);
hf(1, 2, 1+1);
println hf(1, 2, 3, 4);
