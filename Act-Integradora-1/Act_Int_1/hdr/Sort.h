#ifndef SORT_H
#define SORT_H

#include <iostream>
#include <vector>

using std::vector;
using std::swap;

template <class T>
class Sort
{
public:
    Sort() = delete;
    static void quick(vector<T>& vals, const long long int& low, const long long int& high, size_t& comp, size_t& swap);
    static void quick_custom(vector<T>& vals, const long long int& low, const long long int& high,  const int& int_element,  size_t& comp, size_t& swap);
    static void counting(vector<T>& vals, const unsigned short int& max);
    static vector<size_t> counting_custom(vector<T>& vals, const unsigned short int& max, const int& int_element);
    static void insertion();
    static void two_step_sort(vector<T>& vals, const unsigned short int& max, const int& int_element_1, const int& int_element_2,  size_t& comp, size_t& swap);

private:
    static size_t partition(vector<T>& vals, const size_t& low, const size_t& high, size_t& comp, size_t& swap);
    static size_t partition_custom(vector<T>& vals, const size_t& low, const size_t& high,  const int& int_element, size_t& comp, size_t& swap);
};


template <class T>
void Sort<T>::quick(vector<T>& vals, const long long int& low, const long long int& high, size_t& comp, size_t& swap)
{
    if (low < high)
    {
        const size_t pivot = partition(vals, static_cast<size_t>(low), static_cast<size_t>(high), comp, swap);
        quick(vals, low, static_cast<long long int>(pivot) - 1, comp, swap);
        quick(vals, static_cast<long long int>(pivot) + 1, high, comp, swap);
    }
}

template <class T>
void Sort<T>::quick_custom(vector<T>& vals, const long long int& low, const long long int& high, const int& int_element,
                           size_t& comp, size_t& swap)
{
    if (low < high)
    {
        const size_t pivot = partition_custom(vals, static_cast<size_t>(low), static_cast<size_t>(high), int_element, comp, swap);
        quick_custom(vals, low, static_cast<long long int>(pivot) - 1, int_element, comp, swap);
        quick_custom(vals, static_cast<long long int>(pivot) + 1, high, int_element, comp, swap);
    }
}

template <class T>
void Sort<T>::counting(vector<T>& vals, const unsigned short int& max)
{
    const size_t size = vals.size();
    vector<int> countVector(max + 1, 0);
    vector<T> vals_copy = vals;

    for (size_t i = 0; i < size; i++)
    {
        countVector[vals[i]] += 1;
    }

    for (size_t i = 1; i <= max; i++)
    {
        countVector[i] += countVector[i - 1];
    }

    for (int i = static_cast<int>(size) - 1; i >= 0; --i)
    {
        vals[countVector[vals_copy[i]] - 1] = vals_copy[i];
        countVector[vals_copy[i]] -= 1;
    }
}

template <class T>
vector<size_t> Sort<T>::counting_custom(vector<T>& vals, const unsigned short int& max, const int& int_element)
{
    vector<size_t> bounds;
    bounds.reserve(max + 1);
    const size_t size = vals.size();
    vector<int> countVector(max + 1, 0);
    vector<T> vals_copy = vals;

    for (size_t i = 0; i < size; i++)
    {
        countVector[vals[i](int_element)] += 1;
    }

    for (size_t i = 0; i <= max; i++)
    {
        if (countVector[i] > 0)
        {
            bounds.push_back(i);
        }
    }

    for (size_t i = 1; i <= max; i++)
    {
        countVector[i] += countVector[i - 1];
    }

    for (size_t i = 0; i < bounds.size(); i++)
    {
        bounds[i] = countVector[bounds[i]];
    }

    for (int i = static_cast<int>(size) - 1; i >= 0; --i)
    {
        vals[countVector[vals_copy[i](int_element)] - 1] = vals_copy[i];
        countVector[vals_copy[i](int_element)] -= 1;
    }

    return bounds;
}

template <class T>
void Sort<T>::insertion()
{
}

template <class T>
void Sort<T>::two_step_sort(vector<T>& vals, const unsigned short int& max, const int& int_element_1,
    const int& int_element_2,  size_t& comp, size_t& swap)
{
    const vector<size_t> bounds = counting_custom(vals, max, int_element_1);

    quick_custom(vals, 0, bounds[0]-1, int_element_2,  comp, swap);
    for (size_t i = 0; i < bounds.size()-1; i++)
    {
        quick_custom(vals, bounds[i], bounds[i+1]-1, int_element_2,  comp, swap);
    }
}

template <class T>
size_t Sort<T>::partition(vector<T>& vals, const size_t& low, const size_t& high, size_t& comp, size_t& _swap)
{
    const T pivot = vals[high];
    size_t i = low - 1;

    for (size_t j = low; j < high; j++)
    {
        comp++;
        if (vals[j] < pivot)
        {
            i++;
            _swap++;
            swap(vals[i], vals[j]);
        }
    }
    _swap++;
    swap(vals[i + 1], vals[high]);
    return i + 1;
}

template <class T>
size_t Sort<T>::partition_custom(vector<T>& vals, const size_t& low, const size_t& high, const int& int_element, size_t& comp, size_t& _swap)
{
    const int pivot = vals[high](int_element);
    size_t i = low - 1;

    for (size_t j = low; j < high; j++)
    {
        comp++;
        if (vals[j](int_element) < pivot)
        {
            i++;
            _swap++;
            swap(vals[i], vals[j]);
        }
    }
    _swap++;
    swap(vals[i + 1], vals[high]);
    return i + 1;
}
#endif //SORT_H
