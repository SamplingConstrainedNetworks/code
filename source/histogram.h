#ifndef pychaos_histogram_h
#define pychaos_histogram_h

#include <vector>
#include <assert.h>
#include <string>
#include <math.h>

#include "io.h"


template <typename T>
class Histogram {
protected:
    T _lowerBound;
    T _upperBound;

    unsigned int _bins;  // number of bins of the histogram
    unsigned int _count; // number of measured samples
    std::vector<unsigned int> _histogram;  // histogram of samples over bins

    inline virtual T v(T value) const {
        return value;
    }
    inline virtual T iv(T value) const {
        return value;
    }

public:
    Histogram(T lowerBound, T upperBound, unsigned int bins) :
            _lowerBound(v(lowerBound)), _upperBound(v(upperBound)), _bins(bins), _histogram(_bins + 1) {
        reset();
    }

    inline unsigned int bins() const {return _bins;}

    inline unsigned int count() const {return _count;}

    unsigned int const& operator[](unsigned int idx) const {
        assert(idx < _histogram.size());
        return _histogram[idx];
    }

    bool invalid_value(T const& value) const {
        return v(value) <= _lowerBound || v(value) >= _upperBound;
    }

    T get_invalid_value() const {
        return iv(_lowerBound) - 1;
    }

    unsigned int bin(T value) const {
        //std::cout << "bin: " << value << "->" << (value - _lowerBound)*_bins/(_upperBound - _lowerBound) << std::endl;
        value = v(value);
        if (value <= _lowerBound)
            return 0;
        if (value >= _upperBound)
            return _bins;

        // 0.000000001 because in roundoffs of integer histograms, we want to ensure round up.
        unsigned int bin = (value - _lowerBound)*_bins/(_upperBound - _lowerBound);
        assert(bin < _bins);
        return bin;
    }

    // inverse of `bin`
    T value(unsigned int bin) const {
        //std::cout << "value: "<< bin << "->" << iv((_upperBound - _lowerBound)*bin/(_bins-1)) << std::endl;
        if (bin >= _bins)
            return iv(_upperBound);
        if (bin <= 0)
            return iv(_lowerBound);
        return iv(_lowerBound + (_upperBound - _lowerBound)*bin/_bins);
    }

    virtual void reset() {
        for (unsigned int bin = 0; bin <= _bins; bin++)
            _histogram[bin] = 0;
        _count = 0;
    }

    virtual void add(T value) {
        _histogram[bin(value)]++;
        _count++;
    }

    void print() const {
        unsigned int sum = 0;
        for (unsigned int bin = 0; bin <= _bins; bin++) {
            sum += _histogram[bin];
            if (_histogram[bin] > 0)
                printf("%e %e %d\n", value(bin), _histogram[bin]*1./_count, bin);
        }
        assert(sum == _count);
    }

    void export_pretty(std::string file_name) const {
        std::vector<std::vector<double> > data;

        for (unsigned int bin = 0; bin <= _bins; bin++)
        {
            if (_histogram[bin] > 0)
            {
                std::vector<double> row(2);
                row[0] = value(bin);
                row[1] = _histogram[bin]*1./_count;
                data.push_back(row);
            }
        }
        io::save(data, file_name);
    }

    virtual void export_histogram(std::string file_name) const {
        std::vector<std::vector<double> > data;

        for (unsigned int bin = 0; bin <= _bins; bin++)
        {
            if (_histogram[bin] > 0) {
                std::vector<double> row(2);
                row[0] = bin;
                row[1] = _histogram[bin]*1./_count;
                data.push_back(row);
            }
        }
        io::save(data, file_name);
    }
};


template <typename T>
class Log2Histogram : public Histogram<T> {
protected:
    virtual T v(T value) const {
        return log2(value);
    }
    virtual T iv(T value) const {
        return pow(2, value);
    }
public:
    Log2Histogram(T lowerBound,
                  T upperBound,
                  unsigned int bins) : Histogram<T>(v(lowerBound), v(upperBound), bins) {}
};


// Histogram that also measures the excitation matrix
template <typename T>
class ExcitationHistogram : public Histogram<T> {
protected:
    std::vector<std::vector<unsigned int> > _matrix;

public:
    ExcitationHistogram(T lowerBound, T upperBound, unsigned int bins) :
            Histogram<T>(lowerBound, upperBound, bins), _matrix(bins, std::vector<unsigned int>(bins))
    {
        reset();
    }

    using Histogram<T>::add;
    void add(T value, T valuePrime)
    {
        _matrix[bin(value)][bin(valuePrime)]++;
        Histogram<T>::add(value);
    }

    virtual void reset() {
        Histogram<T>::reset();
        for (unsigned int i = 0; i <= this->_bins; i++)
            for (unsigned int j = 0; j <= this->_bins; j++)
                _matrix[i][j] = 0;
    }

    void export_matrix(std::string file_name) const {
        std::vector<std::vector<double> > data;

        for (unsigned int i = 0; i <= this->_bins; i++)
        {
            for (unsigned int j = 0; j <= this->_bins; j++)
            {
                double value = 0;
                if (this->_histogram[i] > 0)
                    value = _matrix[i][j]*1./this->_histogram[i];

                std::vector<double> row(3);
                row[0] = i;
                row[1] = j;
                row[2] = value;
                data.push_back(row);
            }
        }
        io::save(data, file_name);
    }
};


template <typename T>
class LogExcitationHistogram : public ExcitationHistogram<T> {
protected:
    virtual T v(T value) const {
        return log2(value);
    }
    virtual T iv(T value) const {
        return pow(2, value);
    }
public:
    LogExcitationHistogram(T lowerBound,
                           T upperBound,
                           unsigned int bins) : ExcitationHistogram<T>(v(lowerBound), v(upperBound), bins) {}
};


#endif
