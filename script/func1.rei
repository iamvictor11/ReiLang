echo "-- func 1 --";
func A()
beg
    var a = 1;
    echo a;
end
func B()
beg
    a := 2;
    A();
end
func C(a)
beg
    print a + ",";
    a -= 1;
    if (a == 0)
        return;
    C(a);
end
func D()
beg
    echo "call D";
end
func E(fn)
beg
    fn();
end
B();
C(5);
E(D);
