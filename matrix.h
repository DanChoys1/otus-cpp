#include <unordered_map>
#include <optional>
#include <stdexcept>

using namespace std;

/////////////////////////////////// MatrixCellHelper ///////////////////////////////////
template<typename T, T DefVal>
class MatrixCellHelper
{
    using cellIteratorType = typename unordered_map<size_t, T>::iterator;
    using cellConstIteratorType = typename unordered_map<size_t, T>::const_iterator;
public:
    operator T() const
    {
        optional<cellConstIteratorType> currValItOpt = tryGetCurrCellIt();
        return currValItOpt ? currValItOpt.value()->second : DefVal;
    }

    operator tuple<size_t, size_t, T>() const
    {
        return make_tuple(_rowIndex, _cellIndex, T(*this));
    }

    MatrixCellHelper& operator= (const T& val)
    {
        if (DefVal == val)
        {
            auto currValItOpt = tryGetCurrCellIt();
            if (currValItOpt)
                _matrix.at(_rowIndex).erase(*currValItOpt);
        }
        else
        {
            _matrix[_rowIndex][_cellIndex] = val;
        }

        return *this;
    }

private:
    MatrixCellHelper(size_t rowIndex, size_t cellIndex, unordered_map<size_t, unordered_map<size_t, T>>& matrix) :
        _rowIndex(rowIndex), _cellIndex(cellIndex), _matrix(matrix)
    {}

    MatrixCellHelper(const MatrixCellHelper&) = delete;
    void operator= (const MatrixCellHelper&) = delete;
    MatrixCellHelper(MatrixCellHelper&&) = delete;
    void operator= (MatrixCellHelper&&) = delete;

    optional<cellIteratorType> tryGetCurrCellIt()
    {
        auto rowIt = _matrix.find(_rowIndex);
        if (rowIt != _matrix.end())
        {
            auto& row = rowIt->second;
            cellIteratorType cellIt = row.find(_cellIndex);
            if (cellIt != row.end())
            {
                return cellIt;
            }
        }

        return optional<cellIteratorType>();
    }

    optional<cellConstIteratorType> tryGetCurrCellIt() const
    {
        auto rowIt = _matrix.find(_rowIndex);
        if (rowIt != _matrix.end())
        {
            const auto& row = rowIt->second;
            cellConstIteratorType cellIt = row.find(_cellIndex);
            if (cellIt != row.end())
            {
                return cellIt;
            }
        }

        return optional<cellConstIteratorType>();
    }

private:
    size_t _rowIndex;
    size_t _cellIndex;
    unordered_map<size_t, unordered_map<size_t, T>>& _matrix;

    template<typename U, U DefV> friend class Matrix;
    template<typename U, U DefV> friend class MatrixRowHelper;
};

/////////////////////////////////// MatrixRowHelper ///////////////////////////////////
template<typename T, T DefVal>
class MatrixRowHelper
{
    using cellIteratorType = typename unordered_map<size_t, T>::iterator;
public:
    MatrixCellHelper<T, DefVal> operator[] (size_t cellIndex)
    {
        return MatrixCellHelper<T, DefVal> { _rowIndex, cellIndex, _matrix };
    }

    size_t size() const
    {
        return _matrix.at(_rowIndex).size();
    }
    
private:
    MatrixRowHelper(size_t rowIndex, unordered_map<size_t, unordered_map<size_t, T>>& matrix) :
        _rowIndex(rowIndex), _matrix(matrix)
    {}

    MatrixRowHelper(const MatrixRowHelper&) = delete;
    void operator= (const MatrixRowHelper&) = delete;
    MatrixRowHelper(MatrixRowHelper&&) = delete;
    void operator= (MatrixRowHelper&&) = delete;

private:
    size_t _rowIndex;
    unordered_map<size_t, unordered_map<size_t, T>>& _matrix;

    template<typename U, U DefV> friend class Matrix;

};

/////////////////////////////////// Matrix ///////////////////////////////////
template<typename T, T DefVal>
class Matrix
{
public:
    MatrixRowHelper<T, DefVal> operator[] (size_t rowIndex)
    {
        return MatrixRowHelper<T, DefVal> { rowIndex, _matrix };
    }

    size_t size() const
    {
        size_t s = 0;
        for(const auto& row : _matrix)
            s += row.second.size();

        return s;
    }

private:
    /////////////////////////////////// iterator ///////////////////////////////////
    class iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = MatrixCellHelper<T, DefVal>;
        using difference_type = std::ptrdiff_t;
        using pointer = MatrixCellHelper<T, DefVal>;
        using reference = MatrixCellHelper<T, DefVal>;

        using rowIteratorType = typename unordered_map<size_t, unordered_map<size_t, T>>::iterator;
        using cellIteratorType = typename unordered_map<size_t, T>::iterator;

        iterator(unordered_map<size_t, unordered_map<size_t, T>>& matrix, bool isEnd = false) : 
            _matrix(matrix)
        {
            _rowEnd = _matrix.end();
            _rowCurr = isEnd ? _rowEnd : _matrix.begin();
            if (_rowCurr != _rowEnd)
                initCellIts();
        }

        reference operator*() const { return MatrixCellHelper<T, DefVal> { _rowCurr->first, _cellCurr->first, _matrix }; }
        pointer operator->() { return MatrixCellHelper<T, DefVal> { _rowCurr->firste, _cellCurr->firste, _matrix }; }

        iterator& operator++() 
        {
            if (++_cellCurr == _cellEnd)
            {
                if (++_rowCurr != _rowEnd)
                    initCellIts();
            }

            return *this;
        }

        bool operator==(const iterator& other) const 
        {
             return _rowCurr == other._rowCurr && (_rowCurr == _rowEnd || _cellCurr == other._cellCurr);
        }

        bool operator!=(const iterator& other) const { return !operator==(other); }

    private:
        void initCellIts()
        {
            _cellCurr = _rowCurr->second.begin();
            _cellEnd = _rowCurr->second.end();
        }

    private:
        unordered_map<size_t, unordered_map<size_t, T>>& _matrix;

        rowIteratorType _rowCurr;
        rowIteratorType _rowEnd;

        cellIteratorType _cellCurr;
        cellIteratorType _cellEnd;

    };
    ///////////////////////////////////////////////////////////////////////////////

public:
    iterator begin() { return iterator{ _matrix }; }
    iterator end() { return iterator{ _matrix, true }; }

private:
    unordered_map<size_t, unordered_map<size_t, T>> _matrix;

};