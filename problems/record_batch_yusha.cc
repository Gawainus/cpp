#include <optional>

#include "types.h"

enum class Status
{
    Ok = 0,
    End = 1,
    StatusMax
};

using RecordBatchPV = RecordBatch<2>;

template<typename Batch>
class Stream
{
public:

    void append(Batch&& rb) {
        buffer_.emplace(std::move(rb));
    }

    std::tuple<Status, std::optional<Batch>> next() {

        if (!buffer_.empty()) {
            return {Status::End, {}};
        }

        RecordBatch rb = buffer_.front();
        buffer_.pop();
        return {Status::Ok, std::move(rb)};
    }

private:

    std::queue<Batch> buffer_;

};

template <typename Batch>
struct Cmp
{
    bool operator< (
        const Batch& other)

    {

        return *this[0][0] < other[0][0];
    }

    bool operator() (
        const Batch& left,
        const Batch& right)

    {

        return left[0][0] < right[0][0];
    }
};

template<typename BatchType>
class MergeTransform
{
public:
    MergeTransform(
        std::unordered_map<std::string, Stream<BatchType>>* code_to_stream)
        : code_to_stream_(code_to_stream)
    {


    }

    std::tuple<Status, std::optional<BatchType>> next() {
        for (auto&[k, v]: code_to_stream_) {
            auto& v.next();
        }

        return {};
    }

    std::unordered_map<std::string, Stream<BatchType>>* code_to_stream_;

    std::priority_queue<Stream<BatchType>, std::vector<Stream<BatchType>>, Cmp<BatchType>> q_;

};

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

    Stream<RecordBatchPV> s1;
    Stream<RecordBatchPV> s2;

    std::unordered_map<std::string, Stream<RecordBatchPV>> code_to_stream;
    code_to_stream.emplace(
        std::piecewise_construct,
        std::forward_as_tuple("600101"),
        std::forward_as_tuple(std::move(s1)));

    code_to_stream.emplace(
        std::piecewise_construct,
        std::forward_as_tuple("600102"),
        std::forward_as_tuple(std::move(s2)));

    MergeTransform<RecordBatchPV> mt(&code_to_stream);

    const auto& ret = mt.next();
    if (std::get<0>(ret) == Status::Ok) {
        std::get<1>(ret)->print();
    }

    // code, price, time
    // 60518, 996, 09:31:25.00

}