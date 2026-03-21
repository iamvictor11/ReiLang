echo "-- func 1 --";
func A()
{
    var a = 1;
    echo a;
}
func B()
{
    a := 2;
    A();
}
func C(a)
{
    print a + ",";
    a -= 1;
    if (a == 0)
        return;
    C(a);
}
{
var a = 3;
{
var a = 4;
B();
}
}
C(5);