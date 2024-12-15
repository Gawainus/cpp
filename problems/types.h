#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <iomanip>
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



struct Field
{
    Field(const std::string_view name, DataType dp)
        : name_(name)
        , data_type_(dp)
    {

    }

    const std::string name_;
    const DataType data_type_;
};


template <typename T>
struct Scalar {
    Scalar (const T& v)
        : data_(v)
    {

    }

    Scalar (T&& v)
        : data_(std::move(v))
    {

    }

    T& as() {

    }

    bool operator < (const Scalar& other) const
    {
        return *this < other;
    }

    T data_;

};

using Int64  = Scalar<int64_t>;
using Double = Scalar<double>;

template<typename Scalar>
class Array
{

public:
    Array(DataType dp) : dp_(dp) {

    }

    virtual ~Array() {

    }

    Scalar operator[] (size_t index)
    {
        return data_[index];
    }

    Scalar operator[] (size_t index) const
    {
        return data_[index];
    }

    const Scalar& value(size_t i) const;

    size_t size() const
    {
        return data_.size();
    }

    void append(const Scalar& v) {
        data_.push_back(v);
    }

    void append(Scalar&& v) {
        data_.emplace_back(std::move(v));
    }


private:
    DataType dp_;
    std::vector<Scalar> data_;
};

using IntArray = Array<Int64>;
using DoubleArray = Array<Double>;

struct RecorBatchIter
{

};

struct RecordBatchIntrfc
{
    virtual ~RecordBatchIntrfc() {}

    virtual void print() const = 0;

};

constexpr static size_t PP_COLUMN_SIZE = 16;

template<size_t NUM_COLUMNS>
struct RecordBatch: RecordBatchIntrfc
{
    RecordBatch()
    {

    }

    RecordBatch(
        const std::string& symbol,
        const IntArray& timestamps,
        const std::array<Field, NUM_COLUMNS>& schema,
        const std::array<DoubleArray, NUM_COLUMNS>& arrays)
        : symbol_(symbol)
        , timestamps_(timestamps)
        , schema_(schema)
        , arrays_(arrays)
    {

    }

    size_t num_rows() const {
        return arrays_[0].size();
    }

    void print() const override
    {
        std::cout << symbol_ << std::right << std::setw(PP_COLUMN_SIZE - symbol_.size()) << "ts";
        for (const auto& f: schema_) {
            std::cout << std::right << std::setw(PP_COLUMN_SIZE) << f.name_ ;
        }

        std::cout << "\n";

        for (size_t i = 0; i < num_rows(); ++i) {
            std::cout << symbol_ << std::right << std::setw(PP_COLUMN_SIZE - symbol_.size()) << timestamps_[i].data_;

            for (const auto& arr: arrays_) {
                std::cout << std::right << std::setw(PP_COLUMN_SIZE) << arr[i].data_;
            }

            std::cout << "\n";
        }

            std::cout << std::endl;

    }

    RecorBatchIter iter() const;


    // members
    const std::string symbol_;
    const IntArray timestamps_;
    const std::array<Field, NUM_COLUMNS> schema_;
    const std::array<DoubleArray, NUM_COLUMNS> arrays_;
};
