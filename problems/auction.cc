
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <map>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <iostream>

using namespace std;

class Level
{

public:
    Level(int64_t px)
        : px_(px)
        , qty_total_(0)
    {

    }

    void append(int64_t qty) {
        orders_.emplace_back(qty);
        qty_total_ += qty;
    }

    int64_t qty_total() const {
        return qty_total_;
    }


    const int64_t px_;

private:
    int64_t qty_total_;
    vector<int64_t> orders_;
};

class Matcher
{
public:

    Matcher()
        : qty_total_book_(0)
    {

    }

    void add(char side, int64_t qty, int64_t px)
    {
        if (side == 'b') {
            add_to_side(qty, px, book_side_b_, px_to_level_b_);
        } else if (side == 's') {
            add_to_side(qty, px, book_side_a_, px_to_level_a_);
        } else {
            throw runtime_error("Unkown side");
        }

        qty_total_book_ += qty;
    }

    // pretty print the current book
    void pprint()
    {
        std::cout << "Book: " << "\n";
        int64_t scanned_price = std::numeric_limits<int64_t>::max();
        for (auto iter = book_side_a_.rbegin(); iter != book_side_a_.rend(); ++iter) {
            const Level& level_a = iter->second;
            int64_t px_curr = level_a.px_;

            bool printed_bids = false;
            if (px_to_level_b_.contains(px_curr)) {
                const Level& level_b = *px_to_level_b_.at(px_curr);
                cout << level_b.qty_total() << " " << level_b.px_;
                printed_bids = true;
            }

            if (printed_bids) {
                cout << "\t";
            } else {
                cout << "\t\t";
            }

            cout << px_curr << " " << level_a.qty_total() << "\n";

            scanned_price = px_curr;
        }

        auto iter = book_side_b_.upper_bound(scanned_price);
        for (; iter != book_side_b_.end(); ++iter) {

            const Level& level_b = iter->second;
            std::cout << level_b.qty_total() << " " << level_b.px_ << "\n";
        }

        std::cout << "-------" << std::endl;
    }


    /*
    There are two ways of finding the reference price.
    1) Search from the lowest ask level
        a) start from the lowest ask level
        b) find enough qty starting from the highest bid level
        c) move on to the next lowest ask once such qty is found
        d) stop when
            i) there is not enough qty on the bid side at the same level
                the the auction price is on the ask side
            ii) there is still qty on the bid at the same level when b) is satisfied
                the the auction price is on the bid side
            iii) but the book is not crossed anymore
                then the auction price is previous ask
    2) Search from the highest bid level. Same as 1) but go down from the highest bid

    Example:
     qty|  px|        px|  qty
                    14000 2000
    4000 13000      13000 1500
    1000 12000      12000 1000
    1000 11000      11000 2000
    1000 10500

    Method 1)
    1. Search from best ask 2000@11000. The best bid 4000@13000 has it. 2000 lots left.
    2. Move to 1000@12000. The best bid 2000@13000 has it. 1000 lots left.
    3. Move to 1500@13000. The best bid 1000@13000 is 500 lots short.
    4. Move down the bid side to 12000 but 12000 < 13000. STOP on d) i).

    Method 2)
    1. Search from best bid 4000@13000.
    2. There is not enough qty till 1500@13000
    3. STOP on d) ii)



    However, in This example, we see that auction price can be either 11000
        or 13000 depends on from which side we start. We take the higher price
        from the bid side to be the auction price
    1) 13000 if
        Example:
     qty|  px|        px|  qty
                    14000 2000
    4000 13000      13000 1500
    1000 11000      11000 4000
    1000 10500
    */

    // px, vol_matched, vol_unmatched
    // vol_unmatched is total qty added to the book - 2 * vol_matched
    // it needs to be multiplied by two as we need to double count qty from both
    // sides

    template<typename BookSideIter, typename BookSideMatch>
    tuple<int64_t, int64_t, int64_t> process_side(
        const BookSideIter& book_side_iter,
        const BookSideMatch& book_side_match)
    {
            // int64_t auction_px = 0;
        int64_t matched_vol = 0;
        int64_t found_qty = 0;
        auto match_iter = book_side_b_.cbegin();
        for (const auto& iter: book_side_iter) {
            if (found_qty >= 0 && match_iter->first < iter.first) {
                return {match_iter->first, matched_vol, qty_total_book_ - matched_vol * 2};
            }

            int64_t level_qty = iter.second.qty_total();
            while (found_qty < level_qty) {
                found_qty += match_iter->second.qty_total();
                ++match_iter;
                if (match_iter->first < iter.first || match_iter == book_side_b_.cend()) {
                    matched_vol += found_qty;
                    return {iter.first, matched_vol, qty_total_book_ - matched_vol * 2};
                }
            }

            found_qty -= level_qty;
            matched_vol += level_qty;
        }

        return {numeric_limits<int64_t>::min(), matched_vol, qty_total_book_ - matched_vol * 2};
    }

    tuple<int64_t, int64_t, int64_t> process()
    {

        auto t1 = process_side(book_side_a_, book_side_b_);
        auto t2 = process_side(book_side_b_, book_side_a_);
        int64_t matched_vol_1 = get<1>(t1);
        int64_t matched_vol_2 = get<1>(t2);
        if (matched_vol_1 == matched_vol_2) {
            return t2; // return the search from bid side the get the higher auction price
        }

        return matched_vol_1 > matched_vol_2 ? t1 : t2;
    }

private:
    template<typename BookSide>
    void add_to_side(
        int64_t qty,
        int64_t px,
        BookSide& book_side,
        unordered_map<int64_t, Level*>& px_to_level)
    {
        if (!px_to_level.contains(px)) {
            auto pair = book_side.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(px),
                std::forward_as_tuple(px)
            );

            if (!pair.second) {
                throw runtime_error("Unable to create level.");
            }

            Level& level = pair.first->second;
            px_to_level[px] = &level;
        }

        px_to_level.at(px)->append(qty);
    }

private:
    int64_t qty_total_book_;
    // used to store sorted levels for each side of the book
    map<int64_t, Level, std::less<>>    book_side_a_;
    map<int64_t, Level, std::greater<>> book_side_b_;

    // used to quickly fetch a level by price
    unordered_map<int64_t, Level*> px_to_level_a_;
    unordered_map<int64_t, Level*> px_to_level_b_;

};


int main()
{
    vector<tuple<char, int64_t, int64_t>> input = {
        // side, qty, px
        {'s', 1000, 14000},
        {'s', 1000, 14000},
        {'s', 1000, 13000},
        {'s',  500, 13000},
        {'s', 1000, 12000},
        {'s', 1000, 11000},
        {'s',  300, 11000},
        {'s', 2700, 11000},

        {'b', 1000, 13000},
        {'b', 3000, 13000},
        {'b', 1000, 12000},
        {'b', 1000, 11000},
        {'b', 1000, 10500},
    };

    Matcher m;
    for (const auto& [side, qty, px]: input ) {
        m.add(side, qty, px);
        m.pprint();
        auto t = m.process();
        int64_t auction_px = get<0>(t);
        int64_t matched_vol = get<1>(t);
        int64_t unmatched_vol = get<2>(t);

        cout << auction_px << " " << matched_vol << " " << unmatched_vol << endl;
    }


}