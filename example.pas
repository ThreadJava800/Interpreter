program example;
var 
    summa: integer;
    comparator: integer;
    i: integer;
begin
    summa := (123 + 10) * 3
    comparator := not(25 >= 9 and 25 < 9)
    writeln(summa)
    writeln(comparator)

    if (123 = 10) then
    begin
        writeln(42)
    end
    else
    begin
        writeln(52)
    end

    while i <= 5 do
    begin
        writeln(i)
        i := i + 1
    end
    
    writeln(0)

    for i := 20 downto 10 do 
    begin
        writeln(i)
    end

end.