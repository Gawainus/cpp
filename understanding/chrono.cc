#include <chrono>

#include <iostream>

using namespace std;
using namespace std::chrono;

using Clock = high_resolution_clock;
using Timepoint = time_point<Clock>;

int main()
{

    time_point<high_resolution_clock> tp = high_resolution_clock::now();
    cout << tp.time_since_epoch().count() << " nanos" << endl;

    year_month_day ymd(
        year(2024),
        month(5),
        day(11));

    Timepoint tp2(sys_days{ymd});
    tp2 += hours(2);

    cout << tp2.time_since_epoch().count() << " years" << endl;


}