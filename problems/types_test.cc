#include "types.h"

int main()
{
    Field f1 ("price", DataType::Double);
    Field f2 ("volume", DataType::Double);


    IntArray timestamps1(DataType::Int64);
    DoubleArray prices1(DataType::Double);
    DoubleArray volume1(DataType::Double);

    timestamps1.append(1);
    prices1.append(10);
    volume1.append(100);

    timestamps1.append(2);
    prices1.append(10.1);
    volume1.append(1000);

    timestamps1.append(3);
    prices1.append(9.9);
    volume1.append(300);

    RecordBatch<2> rb(
        "600101",
        timestamps1,
        {f1, f2},
        {prices1, volume1});

    IntArray timestamps2(DataType::Int64);
    DoubleArray prices2(DataType::Double);
    DoubleArray volume2(DataType::Double);

    timestamps2.append(3);
    prices2.append(100);
    volume2.append(2500);

    timestamps2.append(3);
    prices2.append(100.1);
    volume2.append(4300);

    timestamps2.append(4);
    prices2.append(99.9);
    volume2.append(5000);

    RecordBatch<2> rb2(
        "600102",
        timestamps2,
        {f1, f2},
        {prices2, volume2});


    rb.print();
    rb2.print();

    return 0;
}