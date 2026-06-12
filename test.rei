start_time := clock("ms");
sum := 0;
i := 0;
LOOP i < 10'0000'0000
DO
    sum += i;
    i += 1;
END
end_time := clock();
ECHO "执行时间 " + (end_time - start_time) + " ms.";
ECHO "sum: " + sum + " i: " + i;