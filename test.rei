start_time := clock("ms");
sum := 0;
i := 0;
loop i < 10'0000'0000
do
    sum += i;
    i += 1;
end
end_time := clock();
echo "执行时间 " + (end_time - start_time) + " ms.";
echo "sum: " + sum + " i: " + i;