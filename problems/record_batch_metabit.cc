#include <cmath>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <queue>
#include <unordered_map>
#include <vector>

enum class DataType
{
    String, // stock
    Int64, // time
    Double, // values
};

enum class Status
{
    Ok = 0,
    End = 1,
    StatusMax
};


struct Field
{
    std::string name_;
    DataType data_type_;
};


struct Scalar {
    template <typename T>
    Scalar (const T& v) {

    }

    template <typename T>
    T& as() {

    }

    bool operator < (const Scalar& other) const
    {
        return *this < other;
    }


};


class Array
{
    Array(DataType dp);
    virtual ~Array() {

    }

    virtual size_t size() const;
    virtual const Scalar& value(size_t i) const;

    virtual void append(const Scalar& v);
    virtual void append(Scalar&& v);
};

struct RecorBatchIter
{

};

struct RecordBatch
{
    RecordBatch()
    {

    }

    RecordBatch(
        std::vector<std::shared_ptr<Field>> schema,
        std::vector<std::shared_ptr<Array>> arrays
    ) {

    }

    size_t num_rows() const;
    RecorBatchIter iter() const;


    // members
    std::vector<std::shared_ptr<Field>> schema;
    std::vector<std::shared_ptr<Field>> arrays;
};

class Stream
{
public:
    virtual ~Stream() {}
    virtual std::shared_ptr<RecordBatch> try_next() = 0;

};

template <typename RecordBatch>
struct Cmp
{
    bool operator< (
        const RecordBatch& other)

    {

        return *this[0][0] < other[0][0];
    }

    bool operator() (
        const RecordBatch& left,
        const RecordBatch& right)

    {

        return left[0][0] < right[0][0];
    }
};

class MergeTransform
{
public:
    MergeTransform(
        const std::unordered_map<std::string, std::shared_ptr<Stream>>& code_to_stream)
        : code_to_stream_(code_to_stream)
    {

        q_.emplace();

    }

    std::tuple<Status, RecordBatch> try_next() {

        return {};
    }

    std::unordered_map<std::string, std::shared_ptr<Stream>>code_to_stream_;

    std::priority_queue<Stream, std::vector<Stream>, Cmp<RecordBatch>> q_;

};

int main()
{

    MergeTransform mt({});

    std::tuple<Status, RecordBatch> ret = mt.try_next();

    // code, price, time
    // 60518, 996, 09:31:25.00

}